/**
 *    Filename:  CircularStrategy.hpp
 * Description:  Strategy for finding circular file dependencies
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak
 */

#ifndef __CIRCULAR_STRATEGY_HPP
#define __CIRCULAR_STRATEGY_HPP

#include <list>
#include <vector>
#include <iostream>
#include <boost/graph/strong_components.hpp>
#include "DepCheckStrategy.hpp"

class CircularStrategy : public DepCheckStrategy
{
private:
    typedef std::list<int> Cycle;
    typedef std::vector<Cycle> CyclesVec;
    typedef std::vector<int> CompVec;

public:
    CircularStrategy (void) { }
    ~CircularStrategy (void) { }

    void check_dep (Graph& dep);
};

void CircularStrategy::check_dep (Graph& deps)
{
    Graph::vertices_size_type size = boost::num_vertices(deps);
    CompVec component(size);

    size_t cnum = boost::strong_components(deps, &component[0]);

    if (cnum == size) {
        std::cout << "No cycles detected." << std::endl;
    }
    else {
        CyclesVec cycles = CyclesVec(cnum);

        for (CompVec::size_type i = 0; i != component.size(); ++i)
            cycles[component[i]].push_back(i);

        NameMap names = boost::get(boost::vertex_name, deps);

        std::cout << "Cycle(s) detected...\n";
        for (CompVec::size_type i = 0; i < cnum; ++i) {
            if (cycles[i].size() > 1) {
                std::cout << "Cycle found: ";
                for (Cycle::const_iterator it = cycles[i].begin();
                     it != cycles[i].end(); ++it)
                {
                    std::cout << names[*it] << " ";
                }
                std::cout << "\n";
            }
        }
        std::cout << std::endl;
    }
}

#endif  /* __CIRCULAR_STRATEGY_HPP */

