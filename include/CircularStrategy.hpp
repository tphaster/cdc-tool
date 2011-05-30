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

    void check_dep (const Graph& dep);
};

#endif  /* __CIRCULAR_STRATEGY_HPP */

