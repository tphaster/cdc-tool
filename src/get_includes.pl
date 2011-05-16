#!/usr/bin/perl -w

# Filename:     get_includes.pl [DIR]
# Description:  Script traverses given directory searching for C/C++ source
#               files and analyses each of them to find #include "filename"
#               directives. Results are printed to standard output.
#               Output format:
#                 File: relative/path/to/csource_file1
#                 included_file1
#                 included_file2
#                 ...
#                 <empty line>
#                 File: relative/path/to/csource_file2
#                 ...
#               (paths are relative to given directory)
#
# Author:       Tomasz Pieczerak (tphaster)

use File::Find qw(find);

die("No directory given: Bad number of arguments") unless (0 == $#ARGV);

# accept all GCC compatible C/C++ source filenames
my $pattern =
    '^.+\.(c|i|ii|h|cc|cp|cxx|cpp|CPP|c++|C|hh|H|hp|hxx|hpp|HPP|h++|tcc)$';
my $dir     = shift;

find (sub {get_includes("$File::Find::name") if /$pattern/}, $dir);

sub get_includes {
    my $filename = shift;

    open(my $cfile, $filename) or die("Can't open file '$filename': $!");
    $filename =~ s/^\Q$dir\E//;
    print "File: $filename\n";

    while (<$cfile>) {
        chomp;
        if(s/^#include[\t ]*"(.*)"[\t ]*$/$1/) {
            print "$_\n" unless ($_ eq "");
        }
    }
    print "\n";
}

