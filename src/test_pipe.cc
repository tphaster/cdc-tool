
#include <stdio.h>
#include <iostream>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>

using namespace std;
using namespace boost::iostreams;
 
int main (void)
{
    int fdin = fileno(popen("./get_includes.pl test", "r"));
    assert(fdin != -1);
 
    {
        file_descriptor_source src(fdin);
        stream<file_descriptor_source> input_file(src);
        input_file >> noskipws;

        istream_iterator<char> begin(input_file), end;
        copy(begin, end, ostream_iterator<char>(cout));
    }
 
    close(fdin);
    return 0;
}

