/**
 * @file  FilesDep_Graph.hpp
 * @brief Various graph typedefs for cdc-tool
 * 
 * Here we define some graph-connected types used project-wide. This way the code 
 * is much cleaner and easier to read when it comes to use BGL.
 *
 * @author:  Tomasz Pieczerak (tphaster)
 */

#ifndef __FILEDESC_GRAPH_HPP
#define __FILEDESC_GRAPH_HPP

#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
                              boost::property<boost::vertex_name_t, std::string> >
                              Graph;
typedef std::pair<int, int> Edge;
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;
typedef boost::property_map<Graph, boost::vertex_name_t>::type NameMap;
typedef boost::property_map<Graph, boost::vertex_name_t>::const_type
            const_NameMap;

#endif  /* __FILEDESC_GRAPH_HPP */

