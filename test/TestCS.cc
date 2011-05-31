/**
 *    Filename:  CS_test.cc
 * Description:  Tests for CircularStrategy class
 *    Compiler:  g++ -lboost_unit_test_framework
 *      Author:  Tomasz Pieczerak
 */

#define BOOST_TEST_MAIN
#define BOOST_TEST_DYN_LINK

#include <sstream>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_log.hpp>
#include "CircularStrategy.hpp"
#include "FilesDep_Graph.hpp"

using namespace std;
using namespace boost;
using namespace boost::unit_test;
using namespace boost::unit_test_framework;

BOOST_AUTO_TEST_SUITE(CircularStrategyTest)

BOOST_AUTO_TEST_CASE(SimpleGraphTest1)
{
    CircularStrategy cs;
    Graph g(10);
    NameMap names = get(vertex_name, g);

    stringstream redirectStream;
    streambuf *backup = cout.rdbuf();
    cout.rdbuf(redirectStream.rdbuf());

    names[0] = "0";
    names[1] = "1";
    names[2] = "2";
    names[3] = "3";
    names[4] = "4";
    names[5] = "5";
    names[6] = "6";
    names[7] = "7";
    names[8] = "8";
    names[9] = "9";

    add_edge(0, 1, g);
    add_edge(1, 2, g);
    add_edge(1, 3, g);
    add_edge(3, 5, g);
    add_edge(5, 7, g);
    add_edge(2, 1, g);
    add_edge(9, 8, g);
    add_edge(8, 3, g);
    add_edge(0, 9, g);
    add_edge(6, 4, g);
    add_edge(3, 9, g);

    BOOST_REQUIRE_NO_THROW(cs.check_dep(g));

    string str;
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle(s) detected...");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle found: 3 8 9 ");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle found: 1 2 ");

    cout.rdbuf(backup);
}

BOOST_AUTO_TEST_CASE(SimpleGraphTest2)
{
    CircularStrategy cs;
    Graph g(5);
    NameMap names = get(vertex_name, g);

    stringstream redirectStream;
    streambuf *backup = cout.rdbuf();
    cout.rdbuf(redirectStream.rdbuf());

    names[0] = "0";
    names[1] = "1";
    names[2] = "2";
    names[3] = "3";
    names[4] = "4";

    add_edge(0, 1, g);
    add_edge(1, 0, g);
    add_edge(1, 2, g);
    add_edge(0, 2, g);
    add_edge(4, 3, g);
    add_edge(3, 4, g);
    add_edge(2, 3, g);

    BOOST_REQUIRE_NO_THROW(cs.check_dep(g));

    string str;
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle(s) detected...");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle found: 3 4 ");
    str.clear();
    getline(redirectStream, str);
    BOOST_CHECK(str == "Cycle found: 0 1 ");

    cout.rdbuf(backup);
}

BOOST_AUTO_TEST_CASE(EmptyGraphTest)
{
    CircularStrategy cs;
    Graph g;

    stringstream redirectStream;
    streambuf *backup = cout.rdbuf();
    cout.rdbuf(redirectStream.rdbuf());

    BOOST_REQUIRE_NO_THROW(cs.check_dep(g));

    string str;
    getline(redirectStream, str);
    BOOST_CHECK(str == "No cycles detected.");

    cout.rdbuf(backup);
}

BOOST_AUTO_TEST_SUITE_END()

