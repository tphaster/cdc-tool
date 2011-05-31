/**
 * @file FilesDep.hpp
 * @brief Class for storing and finding abnormal file dependencies
 *
 * Here we define the project's main class.
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */

#ifndef __FILES_DEP_HPP
#define __FILES_DEP_HPP

#include <map>
#include <string>
#include <utility>
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
    typedef boost::filesystem::path DirPath; /**< Type definition for boost::filesystem::path */
    typedef boost::regex Regex;              /**< Type definition for boost::regex */


    /* ======= Static Constants ======= */
    static const char CCPP_SRC_REGEX[];     /**< C/C++ source filename regex */
    static const char CCPP_INCLUDE_REGEX[]; /**< Include directives regex */

    /* ======= Exception classes ======= */
    /**
     * A runtime_error thrown when no strategy has been set.
     */
    class StrategyNotSet : public std::runtime_error {
    public:
    	/**
    	 * A public default constructor.
    	 */
        StrategyNotSet (void) : runtime_error("no strategy set") { }
    };
    /**
      * A runtime_error thrown when no directory has been loaded.
      */
    class NoDirectoryLoaded : public std::runtime_error {
    public:
    	/**
    	 * A public default constructor.
    	 */
        NoDirectoryLoaded (void) : runtime_error("no directory loaded") { }
    };
    /**
     * A runtime_error thrown when the specified directory is empty.
     */
    class EmptyDirectory : public std::runtime_error {
    public:
    	/**
    	 * A public default constructor.
    	 */
        EmptyDirectory (void) : runtime_error("empty directory") { }
    };

    /* ======= Lifecycle ======= */
    FilesDep (void) : _strategy(), _includes(CCPP_INCLUDE_REGEX),
          _files(CCPP_SRC_REGEX)
          { }
    /**
     * A public constructor.
     */
    FilesDep (DepCheckStrategy *strategy) : _strategy(strategy),
          _includes(CCPP_INCLUDE_REGEX), _files(CCPP_SRC_REGEX)
          { }
    /**
     * A public constructor.
     */
    FilesDep (boost::shared_ptr<DepCheckStrategy> strategy)
        : _strategy(strategy), _includes(CCPP_INCLUDE_REGEX),
          _files(CCPP_SRC_REGEX)
          { }
    ~FilesDep (void) { }

    /* ======= Operations ======= */
    /**
     * Directory loader & analyzer method. Optional recursive behaviour.
     * @param[in] dir sourcecode directory to be analyzed
     * @param[in] rec toggle recursive mode
     *
     * @throws EmptyDirectory
     */
    void load_dir (const DirPath& dir, bool rec);

    /**
     * Prints found dependencies in nice (i.e. human readable) form.
     *
     * @throws NoDirectoryLoaded
     */
    void print_dep (void) const;

    /**
     * Checks this instance of FilesDep for dependencies. Uses strategy-dependent
     * @ref DepCheckStrategy::check_dep(const Graph& deps)
     *
     * @throws NoDirectoryLoaded
     * @throws StrategyNotSet
     */
    void check_dep (void) const;

    /* ======= Mutators ======= */
    /**
     * Setter for FilesDep::_files
     * @param[in] files const reference to source Regex
     */
    void set_files_regex (const Regex& files) { _files = files; }
    /**
     * Setter for FilesDep::_includes
     * @param[in] includes const reference to source Regex
     */
    void set_includes_regex (const Regex& includes) { _includes = includes; }
    /**
     * (re)Setter for FilesDep::_strategy
     * @param[in] strategy pointer to Strategy
     */
    void set_strategy (DepCheckStrategy *strategy) {
        _strategy.reset(strategy);
    }
    /**
     * Yet another setter for FilesDep::_strategy
     * @param[in] strategy boost::shared_ptr pointer to Strategy
     */
    void set_strategy (boost::shared_ptr<DepCheckStrategy> strategy) {
        swap(strategy, _strategy);
    }

private:
    /* ======= Typedefs ======= */
    typedef std::map<std::string, int> FilesMap;
    typedef std::pair<std::string, int> FileDesc;

    void process_file (const FilesMap::value_type& file_desc, NameMap& names);

    /* ======= Data Members ======= */
    boost::shared_ptr<DepCheckStrategy> _strategy;  /** Pointer to Strategy */
    Regex _includes;        /**< Regular expression for #include directives */
    Regex _files;           /**< Regular expression for source files */
    DirPath _dir;           /**< Project's directory */
    FilesMap _files_map;    /**< Mapping filenames to vertex numbers */
    Graph _files_dep;       /**< File dependency graph  */
};

#endif /* __FILES_DEP_HPP */

