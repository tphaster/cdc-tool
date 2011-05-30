/**
 *    Filename:  FilesDep.hpp
 * Description:  Class for storing and finding abnormal file dependencies
 *    Compiler:  g++ -lboost_regex -lboost_filesystem
 *      Author:  Tomasz Pieczerak
 */

#ifndef __FILES_DEP_HPP
#define __FILES_DEP_HPP

#include <map>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <boost/regex.hpp>
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

#include "DepCheckStrategy.hpp"
#include "FilesDep_Graph.hpp"

class FilesDep
{
public:
    typedef boost::filesystem::path DirPath;
    typedef boost::regex Regex;

    static const char CCPP_SRC_REGEX[];
    static const char CCPP_INCLUDE_REGEX[];

    class StrategyNotSet : public std::runtime_error {
    public:
        StrategyNotSet (void) : runtime_error("no strategy set") { }
    };
    class NoDirectoryLoaded : public std::runtime_error {
    public:
        NoDirectoryLoaded (void) : runtime_error("no directory loaded") { }
    };
    class EmptyDirectory : public std::runtime_error {
    public:
        EmptyDirectory (void) : runtime_error("empty directory") { }
    };

    FilesDep (void) : _strategy(), _includes(CCPP_SRC_REGEX),
          _files(CCPP_INCLUDE_REGEX)
          { }

    FilesDep (DepCheckStrategy *strategy) : _strategy(strategy),
          _includes(CCPP_SRC_REGEX), _files(CCPP_INCLUDE_REGEX)
          { }
    FilesDep (boost::shared_ptr<DepCheckStrategy> strategy)
        : _strategy(strategy), _includes(CCPP_SRC_REGEX),
          _files(CCPP_INCLUDE_REGEX)
          { }
    ~FilesDep (void) { }

    void load_dir (const DirPath& dir, bool rec);
    void print_dep (void) const;
    void check_dep (void) const;

    void set_files_regex (const Regex& files) { _files = files; }
    void set_includes_regex (const Regex& includes) { _includes = includes; }
    void set_strategy (DepCheckStrategy *strategy) {
        _strategy.reset(strategy);
    }
    void set_strategy (boost::shared_ptr<DepCheckStrategy> strategy) {
        swap(strategy, _strategy);
    }

private:
    typedef std::map<std::string, int> FilesMap;
    typedef std::pair<std::string, int> FileDesc;

    void process_file (const FilesMap::iterator& file_desc);

    boost::shared_ptr<DepCheckStrategy> _strategy;
    Regex _includes;
    Regex _files;
    DirPath _dir;
    FilesMap _files_map;
    Graph _files_dep;
};

const char FilesDep::CCPP_SRC_REGEX[] = "^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++"
                                        "|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$";
const char FilesDep::CCPP_INCLUDE_REGEX[] = "^#include[\t ]*\"(.*)\"[\t ]*$";

void FilesDep::load_dir (const DirPath& dir, bool rec)
{
    size_t file_counter = 0;
    _dir = (dir / boost::filesystem::path("cd")).parent_path();
    _files_map.clear();

    if (rec) {
        for (boost::filesystem::recursive_directory_iterator iter(_dir), end;
             iter != end; ++iter)
        {
            if (boost::filesystem::is_regular_file(iter->path()) &&
                boost::regex_match(iter->path().filename(), _files))
            {
                std::string rel_path =
                    iter->path().string().substr(_dir.string().length()+1);
                _files_map.insert(FileDesc(rel_path, file_counter++));
            }
        }
    }
    else {
        for (boost::filesystem::directory_iterator iter(_dir), end;
             iter != end; ++iter)
        {
            if (boost::filesystem::is_regular_file(iter->path()) &&
                boost::regex_match(iter->path().filename(), _files))
            {
                _files_map.insert(FileDesc(iter->filename(), file_counter++));
            }
        }
    }
    if (_files_map.size() == 0)
        throw EmptyDirectory();

    _files_dep = Graph(_files_map.size());
    NameMap name = get(boost::vertex_name, _files_dep);

    for (FilesMap::iterator it = _files_map.begin();
         it != _files_map.end(); ++it)
    {
        process_file(it);
        name[it->second] = it->first;
    }
    std::cout << std::endl;
}

void FilesDep::print_dep (void) const
{
    if (_files_map.size() == 0)
        throw NoDirectoryLoaded();

    const_NameMap name = get(boost::vertex_name, _files_dep);

    boost::graph_traits<Graph>::edge_iterator ei, ei_end;
    std::cout << "Detected file dependencies...\n";
    for (boost::tie(ei, ei_end) = boost::edges(_files_dep); ei != ei_end; ++ei)
        std::cout << "" << name[source(*ei, _files_dep)]
                  << " -> " << name[target(*ei, _files_dep)] << "\n";
    std::cout << std::endl;
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
    std::ifstream is((_dir / file_desc->first).string().c_str());

    if (is.fail()) {
        std::cerr << "warning: unable to open file " << file_desc->first
                  << std::endl;
        return;
    }

    std::string line;
    boost::smatch what;
    FilesMap::const_iterator to;

    while (std::getline(is, line)) {
        if (boost::regex_match(line, what, _includes)) {
            to = _files_map.find(std::string(what[1].first, what[1].second));
            if (to != _files_map.end())
                boost::add_edge(file_desc->second, to->second, _files_dep);
            else
                std::cerr << "warning: no such file as '"
                          << std::string(what[1].first, what[1].second)
                          << "'\n";
        }
    }
}

#endif /* __FILES_DEP_HPP */

