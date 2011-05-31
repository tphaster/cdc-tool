/**
 * @file DepCheckStrategy.hpp
 *
 * Interface for dependency check strategies (algorithms)
 *
 * @author Tomasz Pieczerak
 * @author Maciej Rubikowski
 */

#ifndef __DEP_CHECK_STRATEGY_HPP
#define __DEP_CHECK_STRATEGY_HPP

#include "FilesDep_Graph.hpp"

/**
 * Interface for dependency check strategies (algorithms)
 */
class DepCheckStrategy
{
public:
    /* ======= Lifecycle ======= */
	/**
	 * A virtual public default destructor.
	 */
    virtual ~DepCheckStrategy (void) { }

    /* ======= Operations ======= */
    /**
     * A pure virtual method according to the strategy pattern
     * (will be defined in particular strategies)
     *
     * @param[in] deps a dependency graph build previously with FilesDep class
     */
    virtual void check_dep (const Graph& deps) = 0;
};

#endif  /* __DEP_CHECK_STRATEGY_HPP */

