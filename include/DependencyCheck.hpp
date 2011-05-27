/**
 *    Filename:  DependencyCheck.hpp
 * Description:  Base class for finding abnormal file dependencies
 *    Compiler:  g++ -lboost_regex -lboost_filesystem
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#ifndef __DEPENDENCYCHECK_HPP
#define __DEPENDENCYCHECK_HPP

#include <map>
#include <iterator>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "filedesc_graph.hpp"

class DependencyCheck
{
private:
    void process_file (const std::string& filename);

public:
    typedef std::map<std::string, size_t> FilesMap;
    typedef std::pair<std::string, size_t> FileDesc;
    typedef boost::property_map<Graph, boost::vertex_name_t>::type NameMap;
    typedef boost::filesystem::path DirPath;
    typedef boost::regex Regex;

    DependencyCheck (void) : _includes("^#include[\t ]*\"(.*)\"[\t ]*$"),
                             _files("^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$")
                             { }
    ~DependencyCheck (void) { }

    void load_dir (const DirPath& dir);
    void load_dep (void);
    void print_dep (void)
    {
        NameMap name = get(boost::vertex_name, _files_dep);

        boost::graph_traits<Graph>::edge_iterator ei, ei_end;
        for (boost::tie(ei, ei_end) = boost::edges(_files_dep); ei != ei_end; ++ei)
            std::cout << "" << name[source(*ei, _files_dep)]
                << " -> " << name[target(*ei, _files_dep)] << "\n";
        std::cout << std::endl;
    }

protected:
    Regex _includes;
    Regex _files;
    DirPath _dir;
    FilesMap _files_map;
    Graph _files_dep;
    size_t _file_counter;
};

void DependencyCheck::load_dir (const DirPath& dir)
{
    _dir = dir;
    _file_counter = 0;
    _files_map.clear();

    std::cout << "debug: set dir to: "<< _dir << std::endl;

    for (boost::filesystem::directory_iterator iter(_dir), end;
         iter != end; ++iter)
    {
        if (boost::filesystem::is_regular_file(iter->path()) &&
            boost::regex_match(iter->path().filename(), _files))
        {
            _files_map.insert(FileDesc(iter->filename(),
                              _file_counter++));
            std::cout << "debug: added file: " << iter->filename()
                      << " (" << _file_counter-1 << ")\n";
        }
    }
}


void DependencyCheck::process_file (const std::string& name)
{
    std::ifstream is((_dir / name).string().c_str());

    if (is.fail()) {
        std::cerr << "cdc-tool: error: unable to open file "
                  << name << std::endl;
        return;
    }
    std::cout << "debug: analysing file " << name << std::endl;

    std::string line;
    boost::smatch what;
    while (std::getline(is, line)) {
        if (boost::regex_match(line, what, _includes)) {
            /* TODO: check whether files are in map */
            boost::add_edge(_files_map.find(name)->second,
                  _files_map.find(std::string(what[1].first, what[1].second))->second,
                  _files_dep);
            std::cout << "debug: added edge: " << name << " -> "
                      << std::string(what[1].first, what[1].second)
                      << std::endl;
        }
    }
}

void DependencyCheck::load_dep (void)
{
    _files_dep = Graph(_files_map.size());
    NameMap name = get(boost::vertex_name, _files_dep);

    for (FilesMap::iterator it = _files_map.begin();
         it != _files_map.end(); ++it)
    {
        process_file(it->first);
        name[it->second] = it->first;
    }
}

#endif /* __DEPENDENCYCHECK_HPP */

