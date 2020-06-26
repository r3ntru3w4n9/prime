/***********************************************************************

  FileName    [PrimeMan.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file difines the data manager for prime.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <fstream>

#include "Cell.h"
#include "Grid.h"
#include "QuadTree.h"
// #include "TreeNet.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class grid {
   public:
    grid() {}
    ~grid() {}
    
    bool routable(const unsigned) const {
        return _global_search != _search;
    }

    int get_cost() const {
        return _cost;
    }

    int get_estimated() const {
        return _estimated_cost;
    }

    void assign_estimated(unsigned _pi, int) {

    }
    void assign_cost(int);
    void reset();

    // functions

    static int _global_search;
   private:
    int _cost;
    unsigned _pi;
    unsigned _pin;
    int _estimated_cost;
    int _search;
};

