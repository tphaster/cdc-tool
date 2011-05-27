/**
 *    Filename:  DepCheckStrategy.hpp
 * Description:  
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak (tphaster) <tphaster a gmail dot com>
 */

#ifndef __DEP_CHECK_STRATEGY_HPP
#define __DEP_CHECK_STRATEGY_HPP

class DepCheckStrategy {
    public:
        void check_dep (Graph& deps, size_t size) = 0;
}

#endif  /* __DEP_CHECK_STRATEGY_HPP */

