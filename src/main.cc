/**
 *    Filename:  main.cc
 * Description:  cdc-tool main() function, this is Where It All Begins
 *    Compiler:  g++ -lboost_program_options
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#include <iostream>
#include <string>
#include <iterator>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;

const string VERSION("cdc-tool 0.0.1");


int main (int argc, char **argv)
{
    try {
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "print this help")
            ("version,V", "display the version of cdc-tool and exit")
            ("dir", po::value<string>(), "project's main directory")
            ("verbose,v", "enable verbose made");

        po::positional_options_description pos;
        pos.add("dir", -1);

        po::variables_map vm;        
        po::store(po::command_line_parser(argc, argv).
                  options(desc).positional(pos).run(), vm);
        po::notify(vm);    

        if (vm.count("help")) {
            cout << VERSION << ", Circular Dependency Check Tool for C/C++\n"
                 << "Usage: cdc_tool [OPTION]... [DIR]\n\n"
                 << desc << endl;
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
        if (vm.count("verbose")) {
            cout << "cdc-tool: verbose mode set" << endl;
        }
        if (vm.count("dir")) {
            cout << "cdc-tool: project's directory set to " 
                 << vm["dir"].as<string>() << ".\n";
        } else {
            cout << "cdc-tool: error: directory was not set" << endl;
            return EXIT_FAILURE;
        }

        /* TODO: rest of code goes here */
    }
    catch (exception& e) {
        cerr << "cdc-tool: error: " << e.what() << endl;
        return EXIT_FAILURE;
    }
    catch (...) {
        cerr << "cdc-tool: error: exception of unknown type!" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
