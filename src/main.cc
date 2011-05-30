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

const string VERSION("cdc-tool 0.2");


int main (int argc, char **argv)
{
    try {
        po::options_description opts("Allowed options");
        po::options_description hidden("Hidden options");

        opts.add_options()
            ("help,h", "print this help")
            ("version,V", "display the version of cdc-tool and exit")
            ("files,f", po::value<string>(), "regex for C/C++ source files")
            ("print,p", "print files' dependencies")
            ("recursive,r", "recursive directory traversal");
        hidden.add_options()
            ("dir", po::value<string>(), "project's main directory");

        po::options_description cmdline;
        cmdline.add(opts).add(hidden);

        po::positional_options_description pos;
        pos.add("dir", -1);

        po::variables_map vm;        
        po::store(po::command_line_parser(argc, argv).
                  options(cmdline).positional(pos).run(), vm);
        po::notify(vm);    

        FilesDep fd(new CircularStrategy);

        if (vm.count("help")) {
            cout << VERSION << ", Circular Dependency Check Tool for C/C++\n"
                 << "Usage: cdc_tool [OPTION]... [DIR]\n\n"
                 << opts << endl;
            return EXIT_SUCCESS;
        }
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
        if (vm.count("files")) {
            FilesDep::Regex files(vm["files"].as<string>());
            cout << "info: files regex set to '" << files << "'\n";
            fd.set_files_regex(files);
        }
        if (vm.count("dir")) {
            fs::path dir_path(vm["dir"].as<string>());

            if (fs::exists(dir_path)) {
                if (fs::is_directory(dir_path)) {
                    cout << "info: project's directory set to " << dir_path
                         << "\n";

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
        else {
            cerr << "cdc-tool: error: directory was not set\n"
                 << "Usage: cdc_tool [OPTION]... [DIR]" << endl;
            return EXIT_FAILURE;
        }

        if (vm.count("print"))
            fd.print_dep();

        fd.check_dep();
    }
    catch (const fs::filesystem_error& ex) {
        cerr << "error: " << ex.what() << endl;
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
}

