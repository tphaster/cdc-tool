/**
 *    Filename:  DepCheckStrategy.hpp
 * Description:  Interface for dependency check strategies (algorithms)
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak
 */

#ifndef __DEP_CHECK_STRATEGY_HPP
#define __DEP_CHECK_STRATEGY_HPP

#include "FilesDep_Graph.hpp"

class DepCheckStrategy
{
public:
    virtual ~DepCheckStrategy (void) { }

    virtual void check_dep (const Graph& deps) = 0;
};

#endif  /* __DEP_CHECK_STRATEGY_HPP */

