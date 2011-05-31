/**
 * @file CircularStrategy.cc
 *
 * CircularStrategy methods implementation
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */

#include <iostream>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/graph/strong_components.hpp>
#include "CircularStrategy.hpp"

using namespace std;
using namespace boost;

void CircularStrategy::check_dep (const Graph& deps)
{
    Graph::vertices_size_type size = num_vertices(deps);/* number of vertices */
    CompVec component(size);    /* each vertex will get it's component number */

    /* algorithm for finding strong components solves the problem */
    size_t cnum = strong_components(deps, &component[0]);

    /* every component contains one vertex => no cycles */
    if (cnum == size) {
        cout << "No cycles detected." << endl;
    }
    /* if not, there is at last one cycle */
    else {
        /* create a list for each component */
        CyclesVec cycles = CyclesVec(cnum);
        for (CompVec::size_type i = 0; i != component.size(); ++i)
            cycles[component[i]].push_back(i);

        /* output should be human-readable, so let's get vertices' names */
        const_NameMap names = get(vertex_name, deps);

        cout << "Cycle(s) detected...\n";
        /* output every component that contains more then one vertex */
        for (CompVec::size_type i = 0; i < cnum; ++i) {
            if (cycles[i].size() > 1) {
                cout << "Cycle found: ";
                for_each(cycles[i].begin(), cycles[i].end(),
                         boost::bind(&CircularStrategy::print_vname, _1, names));
                cout << "\n";
            }
        }
        cout << endl;
    }
}

