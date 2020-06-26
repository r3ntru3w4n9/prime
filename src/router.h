/***********************************************************************

  FileName    [router.h]

  Author      [Yang Chien Yi]

  This file difines the router set. A_star.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <queue>
#include <utility>
#include <vector>
#include "grid.h"
#include "PrimeMan.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

enum cost_type {
    Manhattan,

};

class MyComp {
   public:
    MyComp() {}
    bool operator()(const Grid* lhs, const Grid* rhs) const {
        return ((lhs->get_estimated()) > (rhs->get_estimated()));
    }
};

// typedef
typedef std::vector<grid*> GridList;
typedef std::vector<unsigned> IdxList;
typedef std::priority_queue<Grid*, std::vector<Grid*>, MyComp> priority_grid;

// initialization

class Router {
   public:
    // friend class of cost functions
    friend class CostGen;
    friend class ManDist;
    friend priority_grid;

    // public functions
    Router(PrimeMan& pm);
    ~Router();
    int A_star(const unsigned);

    // cost functions
    int get_cost(unsigned a, unsigned b);
    int ManDist(unsigned a, unsigned b) const;

   private:
    PrimeMan& _pm;
    GridList _GridList;
    priority_grid* _PriorityGrid;
    cost_type _CostType;
    // constant
    const unsigned _area;
    //

    // private functions
    bool propagate(unsigned, unsigned);
    bool sub_propagate(int, unsigned, unsigned, unsigned);
    void init_cost(cost_type);
    void backtrace(unsigned, IdxList&);
    void reset();
    void use_grid(unsigned);
    void clear_space(const unsigned);
    void revive(const unsigned);
    void revive_grid(const unsigned);
    void init_clear(const unsigned, const unsigned);
    void init_grid(const unsigned, const unsigned);
    // operator overloading
};