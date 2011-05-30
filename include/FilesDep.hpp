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

/**
 * Class for storing and finding abnormal file dependencies
 *
 * FilesDep was made for storing file dependencies (using Boost Graph Library)
 * and has appropriate methods for analyzing them. It implements Strategy
 * design pattern in context of analyzing file dependencies.
 */
class FilesDep
{
public:
    /* ======= Typedefs ======= */
    typedef boost::filesystem::path DirPath;
    typedef boost::regex Regex;

    /* ======= Static Constants ======= */
    static const char CCPP_SRC_REGEX[];     /** C/C++ source filename regex */
    static const char CCPP_INCLUDE_REGEX[]; /** Include directives regex */

    /* ======= Exception classes ======= */
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

    /* ======= Lifecycle ======= */
    FilesDep (void) : _strategy(), _includes(CCPP_INCLUDE_REGEX),
          _files(CCPP_SRC_REGEX)
          { }
    FilesDep (DepCheckStrategy *strategy) : _strategy(strategy),
          _includes(CCPP_INCLUDE_REGEX), _files(CCPP_SRC_REGEX)
          { }
    FilesDep (boost::shared_ptr<DepCheckStrategy> strategy)
        : _strategy(strategy), _includes(CCPP_INCLUDE_REGEX),
          _files(CCPP_SRC_REGEX)
          { }
    ~FilesDep (void) { }

    /* ======= Operations ======= */
    void load_dir (const DirPath& dir, bool rec);
    void print_dep (void) const;
    void check_dep (void) const;

    /* ======= Mutators ======= */
    void set_files_regex (const Regex& files) { _files = files; }
    void set_includes_regex (const Regex& includes) { _includes = includes; }
    void set_strategy (DepCheckStrategy *strategy) {
        _strategy.reset(strategy);
    }
    void set_strategy (boost::shared_ptr<DepCheckStrategy> strategy) {
        swap(strategy, _strategy);
    }

private:
    /* ======= Typedefs ======= */
    typedef std::map<std::string, int> FilesMap;
    typedef std::pair<std::string, int> FileDesc;

    /* ======= Helper methods ======= */
    void process_file (const FilesMap::iterator& file_desc);

    /* ======= Data Members ======= */
    boost::shared_ptr<DepCheckStrategy> _strategy;  /** Pointer to Strategy */
    Regex _includes;        /** Regular expression for #include directives */
    Regex _files;           /** Regular expression for source files */
    DirPath _dir;           /** Project's directory */
    FilesMap _files_map;    /** Mapping filenames to vertex numbers */
    Graph _files_dep;       /** File dependency graph  */
};

#endif /* __FILES_DEP_HPP */

