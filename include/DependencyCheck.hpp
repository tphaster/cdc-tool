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


class DependencyCheck
{
private:
    void process_file (const std::string& filename);

public:
    typedef std::map<std::string, size_t> FilesMap;
    typedef std::pair<std::string, size_t> FileDesc;
    typedef boost::filesystem::path DirPath;
    typedef boost::regex Regex;

    DependencyCheck (void) : _includes("^#include[\t ]*\"(.*)\"[\t ]*$"),
                             _files("^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$")
                             { }
    ~DependencyCheck (void) { }

    bool load_dir (const DirPath& dir);
    bool load_dep (void);

private:
    Regex _includes;
    Regex _files;
    DirPath _dir;
    FilesMap _files_map;
    size_t _file_counter;
};

bool DependencyCheck::load_dir (const DirPath& dir)
{
    _dir = dir;
    _file_counter = 0;
    _files_map.clear();

    std::cout << "debug: set dir to: "<<_dir << std::endl;

    /* TODO: directory validity check */

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

    return true;
}


void DependencyCheck::process_file (const std::string& name)
{
    std::ifstream is((_dir / name).string().c_str());

    if (is.fail()) {
        std::cerr << "cdc-tool: error: unable to open file "
                  << name << std::endl;
        return;
    }

    std::cout << "File: " << name << "\n";

    std::string line;
    boost::smatch what;
    while (std::getline(is, line)) {
        if (boost::regex_match(line, what, _includes)) {
            std::cout << std::string(what[1].first, what[1].second)
                      << std::endl;
        }
    }
    std::cout << std::endl;
}

bool DependencyCheck::load_dep (void)
{
    for (FilesMap::iterator it = _files_map.begin();
         it != _files_map.end(); ++it)
    {
        process_file(it->first);
    }
    return true;
}

#endif /* __DEPENDENCYCHECK_HPP */

