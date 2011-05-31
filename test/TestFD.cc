/**
 *    Filename:  CS_test.cc
 * Description:  Tests for CircularStrategy class
 *    Compiler:  g++ -lboost_unit_test_framework -lboost_regex
 *                   -lboost_filesystem
 *      Author:  Tomasz Pieczerak
 */

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <iostream>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include "FilesDep.hpp"
#include "FilesDep_Graph.hpp"

using namespace std;
using namespace boost;
using namespace boost::filesystem;
using namespace boost::unit_test;
using namespace boost::unit_test_framework;


BOOST_AUTO_TEST_SUITE(FilesDepTest)

BOOST_AUTO_TEST_CASE(NotInitialized)
{
    FilesDep fd;

    BOOST_REQUIRE_THROW(fd.check_dep(), FilesDep::NoDirectoryLoaded);
    BOOST_REQUIRE_THROW(fd.print_dep(), FilesDep::NoDirectoryLoaded);
    BOOST_REQUIRE_THROW(fd.load_dir(path("./empty"), true), FilesDep::EmptyDirectory);
    BOOST_REQUIRE_THROW(fd.load_dir(path("./empty"), false), FilesDep::EmptyDirectory);
    BOOST_CHECK_NO_THROW(fd.load_dir(path("./single"), true));
    BOOST_CHECK_NO_THROW(fd.load_dir(path("./single"), false));
    BOOST_REQUIRE_THROW(fd.check_dep(), FilesDep::StrategyNotSet);
}

BOOST_AUTO_TEST_CASE(NoPerm)
{
    FilesDep fd;

    /* mkdir noperm_dir && chmod 000 noperm_dir */
    BOOST_REQUIRE_THROW(fd.load_dir(path("./noperm_dir"), false), std::runtime_error);

    stringstream redirectStream;
    streambuf *backup = cerr.rdbuf();
    cerr.rdbuf(redirectStream.rdbuf());

    /* mkdir noperm/{header.hpp,main.cc} && chmod 000 noperm/{header.hpp,main.cc} */
    BOOST_CHECK_NO_THROW(fd.load_dir(path("./noperm"), false));

    string str;
    getline(redirectStream, str);
    BOOST_CHECK(str == "warning: unable to open file header.hpp");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "warning: unable to open file main.cc");

    cerr.rdbuf(backup);
}

BOOST_AUTO_TEST_CASE(Project)
{
    FilesDep fd;
    string correct_output[] = {
        "Detected file dependencies...",
        "blah.c -> AnotherClass.hpp",
        "blah.c -> blah.h",
        "h2.h -> h1.h",
        "test.c -> header.h",
        "test.c -> SomeClass.hpp",
        "AnotherClass.hpp -> SomeClass.hpp",
        "SomeClass.hpp -> blah.h",
        "SomeClass.hpp -> header.h",
        "blah.h -> AnotherClass.hpp",
        "main.cc -> AnotherClass.hpp",
        "main.cc -> header.h",
        "h1.h -> h2.h"
    };

    stringstream redirectStream;
    streambuf *backup = cerr.rdbuf();
    cerr.rdbuf(redirectStream.rdbuf());

    BOOST_CHECK_NO_THROW(fd.load_dir(path("./project"), false));

    string str;
    getline(redirectStream, str);
    BOOST_CHECK(str == "warning: no such file as 'no_exist.h'");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "warning: no such file as 'no_exist.hpp'");
    str.clear();

    cerr.rdbuf(backup);

    backup = cout.rdbuf();
    cout.rdbuf(redirectStream.rdbuf());

    BOOST_CHECK_NO_THROW(fd.print_dep());

    int i = 0;
    while(getline(redirectStream, str)) {
        if (str == "")
            continue;
        BOOST_CHECK(str == correct_output[i]);
        str.clear();
        ++i;
    }

    cout.rdbuf(backup);
}

BOOST_AUTO_TEST_CASE(Recursive)
{
    FilesDep fd;
    string correct_output[] = {
        "Detected file dependencies...",
        "h2.h -> blah/h1.h",
        "blah/AnotherClass.hpp -> blah/SomeClass.hpp",
        "blah/some.h -> blah/AnotherClass.hpp",
        "blah/SomeClass.hpp -> blah/some.h",
        "blah/SomeClass.hpp -> header.h",
        "blah/h1.h -> h2.h",
        "test.c -> header.h",
        "test.c -> blah/SomeClass.hpp",
        "main.cc -> blah/AnotherClass.hpp",
        "main.cc -> header.h",
        "some.c -> blah/AnotherClass.hpp",
        "some.c -> blah/some.h"
    };

    BOOST_CHECK_NO_THROW(fd.load_dir(path("./recursive"), true));

    stringstream redirectStream;
    streambuf *backup = cout.rdbuf();
    cout.rdbuf(redirectStream.rdbuf());

    BOOST_CHECK_NO_THROW(fd.print_dep());

    string str;
    int i = 0;
    while(getline(redirectStream, str)) {
        if (str == "")
            continue;
        BOOST_CHECK(str == correct_output[i]);
        str.clear();
        ++i;
    }

    cout.rdbuf(backup);
}

BOOST_AUTO_TEST_SUITE_END()

