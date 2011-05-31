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
#include "DepCheckStrategy.hpp"

class CircularStrategy : public DepCheckStrategy
{
private:
    /* ======= Typedefs ======= */
    typedef std::list<int> Cycle;
    typedef std::vector<Cycle> CyclesVec;
    typedef std::vector<int> CompVec;

    /* ======= Helper methods ======= */
    static void print_vname (const Cycle::value_type el, const_NameMap& names) {
        std::cout << names[el] << " ";
    }

public:
    /* ======= Lifecycle ======= */
    CircularStrategy (void) { }
    ~CircularStrategy (void) { }

    /* ======= Operations ======= */
    void check_dep (const Graph& dep);
};

#endif  /* __CIRCULAR_STRATEGY_HPP */

