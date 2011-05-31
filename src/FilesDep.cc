/**
 * @file FilesDep.cc
 *
 * FilesDep's methods implementation
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */

#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/bind.hpp>
#include "FilesDep.hpp"

namespace fs = boost::filesystem;
using namespace std;
using namespace boost;

/* ======= Static Members Initialization ======= */
const char FilesDep::CCPP_SRC_REGEX[] = "^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++"
                                        "|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$";
const char FilesDep::CCPP_INCLUDE_REGEX[] = "^#include[\t ]*\"(.*)\"[\t ]*$";

void FilesDep::load_dir (const DirPath& dir, bool rec)
{
    size_t file_counter = 0;    /* helper file counter */
    _dir = (dir / fs::path("cd")).parent_path();    /* project's directory */
    _files_map.clear();

    /* traverse directory recursively... */
    if (rec) {
        for (fs::recursive_directory_iterator it(_dir), end; it != end; ++it) {
            /* relative (to project's dir) path identifies each file */
            string fpath = it->path().string().substr(_dir.string().length()+1);
            if (fs::is_regular_file(it->path()) && regex_match(fpath, _files)) {
                _files_map.insert(FileDesc(fpath, file_counter++));
            }
        }
    }
    /* ...or not recursively */
    else {
        for (fs::directory_iterator it(_dir), end; it != end; ++it) {
            /* filename identifies each file */
            string fname = it->path().filename();
            if (fs::is_regular_file(it->path()) && regex_match(fname, _files)) {
                _files_map.insert(FileDesc(fname, file_counter++));
            }
        }
    }
    /* if no files were found (or none matches regex), something goes wrong */
    if (file_counter  == 0)
        throw EmptyDirectory();

    /* create graph with proper numer of vertices */
    _files_dep = Graph(file_counter);
    NameMap names = get(vertex_name, _files_dep);

    /* analyze each file and find dependencies */
    for_each(_files_map.begin(), _files_map.end(),
             boost::bind(&FilesDep::process_file, this, _1, names));
    cout << endl;
}

void FilesDep::print_dep (void) const
{
    if (num_vertices(_files_dep) == 0)
        throw NoDirectoryLoaded();

    const_NameMap names = get(vertex_name, _files_dep);

    cout << "Detected file dependencies...\n";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(_files_dep); ei != ei_end; ++ei)
        cout << "" << names[source(*ei, _files_dep)] << " -> "
             << names[target(*ei, _files_dep)] << "\n";
    cout << endl;
}

void FilesDep::check_dep (void) const
{
    if (num_vertices(_files_dep) == 0)
        throw NoDirectoryLoaded();

    /* call concrete strategy (if one was set) */
    if (_strategy)
        _strategy->check_dep(_files_dep);
    else
        throw StrategyNotSet();
}

void FilesDep::process_file (const FilesMap::value_type& file_desc,
                             NameMap& names)
{
    /* name vertex */
    names[file_desc.second] = file_desc.first;

    ifstream is((_dir / file_desc.first).string().c_str());
    if (is.fail()) {
        cerr << "warning: unable to open file " << file_desc.first << endl;
        return;
    }

    /* analyze each line using regex, to find included files */
    string line;
    smatch what;
    FilesMap::const_iterator to;
    while (getline(is, line)) {
        if (regex_match(line, what, _includes)) {
            to = _files_map.find(string(what[1].first, what[1].second));
            /* check whether this file actually exists */
            if (to != _files_map.end())
                add_edge(file_desc.second, to->second, _files_dep);
            else
                cerr << "warning: no such file as '"
                     << string(what[1].first, what[1].second) << "'\n";
        }
    }

}

