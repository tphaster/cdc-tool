/**
 *    Filename:  CircularDep.hpp
 * Description:  Class for finding circular file dependencies
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak (tphaster)
 */

#ifndef __CIRCULARDEP_HPP
#define __CIRCULARDEP_HPP

#include <list>
#include <vector>
#include <boost/graph/strong_components.hpp>
#include "DependencyCheck.hpp"

class CircularDep : public DependencyCheck {
    typedef std::vector<std::list<int> > CyclesVec;
public:
    CircularDep (void) : DependencyCheck() {}
    ~CircularDep (void) {}

    void check_dep (void)
    {
        std::vector<int> component(_files_map.size());
        size_t num = boost::strong_components(_files_dep, &component[0]);

        if (num == _files_map.size()) {
            _cycles.clear();
        }
        else {
            _cycles = CyclesVec(num);

            for (std::vector<int>::size_type i = 0; i != component.size(); ++i)
                _cycles[component[i]].push_back(i);
        }
    }

    void print_cycles (void)
    {
        if (_cycles.size() == 0)
            std::cout << "No cycles detected." << std::endl;
        else {
            NameMap d = get(boost::vertex_name, _files_dep);

            for (size_t i = 0; i < _cycles.size(); ++i) {
                if (_cycles[i].size() > 1) {
                    std::cout << "Cycle: ";
                    for (std::list<int>::iterator it = _cycles[i].begin();
                         it != _cycles[i].end(); ++it)
                    {
                        std::cout << d[*it] << ", ";
                    }
                    std::cout << "\n";
                }
            }
            std::cout << std::endl;
        }
    }
private:
    CyclesVec _cycles;
};

#endif /* __CIRCULARDEP_HPP */

