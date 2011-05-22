/**
 *    Filename:  filedesc_graph.hpp
 * Description:  
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#ifndef __FILEDESC_GRAPH_HPP
#define __FILEDESC_GRAPH_HPP

#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<vecS, vecS, directedS,
                              property<vertex_name_t, string> > Graph;
typedef std::pair<int, int> Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

#endif  /* __FILEDESC_GRAPH_HPP */

