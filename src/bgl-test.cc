/**
 *    Filename:  bgl-test.cc
 * Description:  BGL strongly connected components algorithm usage example
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#include <iostream>
#include <utility>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/strong_components.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS,
                       property<vertex_name_t, string> > Graph;
typedef std::pair<int, int> Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

enum { A, B, C, D, E, F, G, H, I, J,    /* vertices' labels */
       N                                /* number of vertices */
     };


int main (void)
{
    const char* name = "abcdefghij";
    const size_t num_vertices = N;

    Edge edge_array[] = {
            Edge(A,B), Edge(A,C), Edge(A,D),
            Edge(E,F), Edge(E,G),
            Edge(C,J),
            Edge(J,A)
        };
    string names[] = { "file1.c", "header1.h", "header2.h", "header3.h",
                       "file2.c", "header4.h", "header5.h", "file3.cc",
                       "file4.cc", "file5.cc" };
    const size_t num_edges = sizeof(edge_array)/sizeof(edge_array[0]);

    /* declare a graph object */
    Graph graph;

    /* add vertices to the graph object */
    for (size_t i = 0; i < num_vertices; ++i)
        add_vertex(names[i], graph);

    /* add edges to the graph object */
    for (size_t i = 0; i < num_edges; ++i)
        add_edge(edge_array[i].first, edge_array[i].second, graph);

    /* find strongly connected components */
    vector<int> component(num_vertices);
    int num = strong_components(graph, &component[0]);

    property_map<Graph, vertex_name_t>::type d = get(vertex_name, graph);

    /* print what algorithm has found */
    cout << "Total number of components: " << num << endl;
    for (vector<int>::size_type i = 0; i != component.size(); ++i)
        cout << "Vertex '" << d[i]
             << "' is in component " << component[i] << endl;

    return 0;
}

