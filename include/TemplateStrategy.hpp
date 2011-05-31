/**
 * @file TemplateStrategy.hpp
 * @brief Template for creating new strategies
 *
 * An example strategy of the strategy pattern.
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */


#ifndef __TEMPLATE_STRATEGY_HPP
#define __TEMPLATE_STRATEGY_HPP

#include <iostream>
#include "DepCheckStrategy.hpp"

/**
 * An example of strategy pattern.
 */
class TemplateStrategy : public DepCheckStrategy
{
public:
	/**
	 * A public default constructor.
	 */
    TemplateStrategy (void) { }

    /**
     * A public default destructor.
     */
    ~TemplateStrategy (void) { }

    /**
     * Public method, according to the strategy pattern.
     * In this case it is meant to check dependencies between
     * C++ src/include files. Specialized methods will be
     * provided by specific strategies.
     *
     * @param[in] dep graph to be checked for dependencies
     */
    void check_dep (const Graph& dep) {
        std::cout << "Template Strategy..." << std::endl;
    }
};

#endif  /* __TEMPLATE_STRATEGY_HPP */

