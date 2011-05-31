/**
 * @file CircularStrategy.hpp
 *
 * Strategy for finding circular file dependencies
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */

#ifndef __CIRCULAR_STRATEGY_HPP
#define __CIRCULAR_STRATEGY_HPP

#include <list>
#include <vector>
#include <iostream>
#include "DepCheckStrategy.hpp"

/**
 * Strategy for finding circular file dependencies
 */
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
    /**
     * Implements the interface specified in DepCheckStrategy.
     * @see DepCheckStrategy::check_dep (const Graph& deps)
     */
    void check_dep (const Graph& dep);
};

#endif  /* __CIRCULAR_STRATEGY_HPP */

