/**
 *    Filename:  TemplateStrategy.hpp
 * Description:  Template for creating new strategies
 *    Compiler:  g++
 *      Author:  Tomasz Pieczerak
 */

#ifndef __TEMPLATE_STRATEGY_HPP
#define __TEMPLATE_STRATEGY_HPP

#include <iostream>
#include "DepCheckStrategy.hpp"

class TemplateStrategy : public DepCheckStrategy
{
public:
    TemplateStrategy (void) { }
    ~TemplateStrategy (void) { }

    void check_dep (const Graph& dep) {
        std::cout << "Template Strategy..." << std::endl;
    }
};

#endif  /* __TEMPLATE_STRATEGY_HPP */

