/**
 *    Filename:  DependencyCheck.hpp
 * Description:  Base class for finding abnormal file dependencies
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#ifndef __DEPENDENCYCHECK_HPP
#define __DEPENDENCYCHECK_HPP

#include <map>
#include <iterator>
#include <string>
#include <utility>
#include <boost/scoped_ptr.hpp>

class DependencyCheck {
public:
    typedef std::map<std::string, size_t> FilesMap;
    typedef std::pair<std::string, size_t> FileDesc;

    DependencyCheck (void) : _file_counter(0), _files_map(new FilesMap) { }

    std::pair<FilesMap::iterator,bool> addfile (const std::string& rel_file) {
        return _files_map->insert(FileDesc(rel_file, _file_counter++));
    }

private:
    boost::scoped_ptr<FilesMap> _files_map;
    size_t _file_counter;
};

#endif /* __DEPENDENCYCHECK_HPP */

