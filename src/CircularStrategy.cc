/**
 *    Filename:  CircularStrategy.cc
 * Description:  CircularStrategy's methods implementation
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak
 */

#include "CircularStrategy.hpp"

using namespace std;
using namespace boost;

void CircularStrategy::check_dep (const Graph& deps)
{
    Graph::vertices_size_type size = num_vertices(deps);
    CompVec component(size);

    size_t cnum = strong_components(deps, &component[0]);

    if (cnum == size) {
        cout << "No cycles detected." << endl;
    }
    else {
        CyclesVec cycles = CyclesVec(cnum);

        for (CompVec::size_type i = 0; i != component.size(); ++i)
            cycles[component[i]].push_back(i);

        const_NameMap names = get(vertex_name, deps);

        cout << "Cycle(s) detected...\n";
        for (CompVec::size_type i = 0; i < cnum; ++i) {
            if (cycles[i].size() > 1) {
                cout << "Cycle found: ";
                for (Cycle::const_iterator it = cycles[i].begin();
                     it != cycles[i].end(); ++it)
                {
                    cout << names[*it] << " ";
                }
                cout << "\n";
            }
        }
        cout << endl;
    }
}

