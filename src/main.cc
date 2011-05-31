/**
 *    Filename:  main.cc
 * Description:  cdc-tool main() function, this is Where It All Begins
 *    Compiler:  g++ -lboost_program_options -lboost_filesystem
 *                   -lboost_regex
 *      Author:  Tomasz Pieczerak
 */

#include <iostream>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include "CircularStrategy.hpp"
#include "FilesDep.hpp"

namespace fs = boost::filesystem;
namespace po = boost::program_options;
using namespace std;

const string VERSION("cdc-tool 0.3");   /* version string constant */

/**
 * This is the main() function.
 */
int main (int argc, char **argv)
{
    try {
        /* parse given program options using boost::program_options */
        po::options_description opts("Allowed options");
        po::options_description hidden("Hidden options");

        opts.add_options()  /* options visible in list */
            ("help,h", "print this help")
            ("version,V", "display the version of cdc-tool and exit")
            ("print,p", "print files' dependencies")
            ("recursive,r", "recursive directory traversal")
            ("files,f", po::value<string>(), "regex for C/C++ source files");
        hidden.add_options()    /* positional arguments */
            ("dir", po::value<string>(), "project's main directory");

        /* add defined options */
        po::options_description cmdline;
        cmdline.add(opts).add(hidden);

        /* define positional arguments */
        po::positional_options_description pos;
        pos.add("dir", -1);     /* 'dir' is the only positional argument */

        /* parse */
        po::variables_map vm;        
        po::store(po::command_line_parser(argc, argv).
                  options(cmdline).positional(pos).run(), vm);
        po::notify(vm);    

        /* depending on what user typed do.. (order is important) */
        /* print help message and exit */
        if (vm.count("help")) {
            cout << VERSION << ", Circular Dependency Check Tool for C/C++\n"
                 << "Usage: cdc_tool [OPTION]... [DIR]\n\n"
                 << opts << endl;
            return EXIT_SUCCESS;
        }
        /* print program version and copyright information and exit */
        if (vm.count("version")) {
            cout << VERSION << ", Circular Dependency Check Tool for C/C++\n\n"
                 << "Copyright (C) 2011 Maciej Rubikowski, Tomasz Pieczerak\n"
                 << "License GPLv3+: GNU GPL version 3 or later\n"
                 << "<http://www.gnu.org/licenses/gpl.html>.\n"
                 << "This is free software: you are free to change and redistribute it.\n"
                 << "There is NO WARRANTY, to the extent permitted by law.\n\n"
                 << "Written by Maciej Rubikowski and Tomasz Pieczerak."
                 << endl;
            return EXIT_SUCCESS;
        }

        /* As we get so far, it means user actually wants to analyse file
         * dependencies in his project, so let's create FilesDep object.
         * We use here CircularStrategy as we want program to detect circular
         * dependencies -- this is the point where you may want to choose
         * different (your) strategy.
         */
        FilesDep fd(new CircularStrategy);

        /* set non-default regex for filenames */
        if (vm.count("files")) {
            FilesDep::Regex files(vm["files"].as<string>());
            cout << "info: files regex set to '" << files << "'\n";
            fd.set_files_regex(files);
        }
        /* set and load project directory */
        if (vm.count("dir")) {
            fs::path dir_path(vm["dir"].as<string>());

            if (fs::exists(dir_path)) {
                if (fs::is_directory(dir_path)) {
                    cout << "info: project's directory set to " << dir_path
                         << "\n";

                    /* load directory (recursively or not) */
                    fd.load_dir(dir_path, vm.count("recursive"));
                }
                else {
                    cerr << "error: " << dir_path
                         << " is not a valid directory" << endl;
                    return EXIT_FAILURE;
                }
            }
            else {
                cerr << "error: " << dir_path << " no such directory"
                     << endl;
                return EXIT_FAILURE;
            }
        }
        /* no directory set, it's an error */
        else {
            cerr << "cdc-tool: error: directory was not set\n"
                 << "Usage: cdc_tool [OPTION]... [DIR]" << endl;
            return EXIT_FAILURE;
        }

        /* print detected file dependencies if needed */
        if (vm.count("print"))
            fd.print_dep();

        /* analyse dependencies */
        fd.check_dep();
    }
    catch (const fs::filesystem_error& ex) {
        cerr << "error: (filesystem) " << ex.what() << endl;
        return EXIT_FAILURE;
    }
    catch (exception& e) {
        cerr << "error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "error: exception of unknown type!" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;

}   /* end of main() */

