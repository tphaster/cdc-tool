/**
 *    Filename:  get_includes.cc
 * Description:  Program traverses a directory searching for C/C++ source
 *               files and analyses each of them to find #include "filename"
 *               directives. Results are printed to standard output.
 *               Output format:
 *                 File: relative/path/to/csource_file1
 *                 included_file1
 *                 included_file2
 *                 ...
 *                 <empty line>
 *                 File: relative/path/to/csource_file2
 *                 ...
 *    Compiler:  g++ -lboost_filesystem -lboost_regex
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

namespace fs = boost::filesystem;
using namespace std;

void process_stream (istream& is)
{
    static const boost::regex from_reg("^#include[\t ]*\"(.*)\"[\t ]*$");
    string line;

    while (getline(is, line))
    {
        boost::smatch what;
        if (regex_match(line,what,from_reg)) {
            cout << string(what[1].first, what[1].second) << "\n";
        }
    }
}

void process_file (const string& name)
{
    ifstream is(name.c_str());

    if (is.bad()) {
        cerr << "error: unable to open file " << name << endl;
    }
    else {
        cout << "File: " << name << "\n";
        process_stream(is);
        cout << endl;
    }
}

int main (void)
{
    fs::path current_dir("./test");
    boost::regex pattern("^.+\\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$");

    for (fs::recursive_directory_iterator iter(current_dir), end;
         iter != end; ++iter)
    {
        if (regex_match(iter->path().filename(), pattern))
            process_file(iter->path().string());
    }

    return 0;
}

