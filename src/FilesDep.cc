/**
 *    Filename:  FilesDep.cc
 * Description:  FilesDep's methods implementation
 *    Compiler:  g++ -lboost_regex -lboost_filesystem
 *      Author:  Tomasz Pieczerak
 */

#include "FilesDep.hpp"

namespace fs = boost::filesystem;
using namespace std;
using namespace boost;

const char FilesDep::CCPP_SRC_REGEX[] = "^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++"
                                        "|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$";
const char FilesDep::CCPP_INCLUDE_REGEX[] = "^#include[\t ]*\"(.*)\"[\t ]*$";

void FilesDep::load_dir (const DirPath& dir, bool rec)
{
    size_t file_counter = 0;
    _dir = (dir / fs::path("cd")).parent_path();
    _files_map.clear();

    if (rec) {
        for (fs::recursive_directory_iterator it(_dir), end; it != end; ++it) {
            string fpath = it->path().string().substr(_dir.string().length()+1);
            if (fs::is_regular_file(it->path()) && regex_match(fpath, _files)) {
                _files_map.insert(FileDesc(fpath, file_counter++));
            }
        }
    }
    else {
        for (fs::directory_iterator it(_dir), end; it != end; ++it) {
            string fname = it->path().filename();
            if (fs::is_regular_file(it->path()) && regex_match(fname, _files)) {
                _files_map.insert(FileDesc(fname, file_counter++));
            }
        }
    }
    if (file_counter  == 0)
        throw EmptyDirectory();

    _files_dep = Graph(file_counter);
    NameMap name = get(vertex_name, _files_dep);

    for (FilesMap::iterator it = _files_map.begin();
         it != _files_map.end(); ++it)
    {
        process_file(it);
        name[it->second] = it->first;
    }
    cout << endl;
}

void FilesDep::print_dep (void) const
{
    if (_files_map.size() == 0)
        throw NoDirectoryLoaded();

    const_NameMap name = get(vertex_name, _files_dep);

    cout << "Detected file dependencies...\n";
    graph_traits<Graph>::edge_iterator ei, ei_end;
    for (tie(ei, ei_end) = edges(_files_dep); ei != ei_end; ++ei)
        cout << "" << name[source(*ei, _files_dep)] << " -> "
             << name[target(*ei, _files_dep)] << "\n";
    cout << endl;
}

void FilesDep::check_dep (void) const
{
    if (_files_map.size() == 0)
        throw NoDirectoryLoaded();

    if (_strategy)
        _strategy->check_dep(_files_dep);
    else
        throw StrategyNotSet();
}

void FilesDep::process_file (const FilesMap::iterator& file_desc)
{
    ifstream is((_dir / file_desc->first).string().c_str());

    if (is.fail()) {
        cerr << "warning: unable to open file " << file_desc->first << endl;
        return;
    }

    string line;
    smatch what;
    FilesMap::const_iterator to;

    while (getline(is, line)) {
        if (regex_match(line, what, _includes)) {
            to = _files_map.find(string(what[1].first, what[1].second));
            if (to != _files_map.end())
                add_edge(file_desc->second, to->second, _files_dep);
            else
                cerr << "warning: no such file as '"
                     << string(what[1].first, what[1].second) << "'\n";
        }
    }
}

