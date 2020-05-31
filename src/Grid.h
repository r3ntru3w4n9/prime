/***********************************************************************

  FileName    [Cell.h]

  Author      [Yang, Chien Yi]

  This file defines the cells and their elements, pins and nets.

***********************************************************************/

#ifndef CELL_H
#define CELL_H

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "MasterCell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class NormalGrid : public Grid
{
public:
    NormalGrid(unsigned i):Grid(i),_cost(0),_search(0),_nearPin(0) {}
    NormalGrid(unsigned i,unsigned pi):Grid(i),_cost(0),_search(0),_nearPin(1),_pin(pi) {}
    ~NormalGrid() {}
    GridType type() const override { return Normal_Grid; }
    bool routable(const unsigned target) const override
    {
        if(_nearPin&&target != _pin) return false;
        return !_search;
    }
    int get_cost() const override { return _cost; }
    int get_estimated() const override { return _estimated_cost; }
    unsigned get_target() const override { return _pi; }//should be get_pi, only for virtual convenience
    void assign_cost(int cost) override { _cost = cost; }
    void assign_estimated(unsigned pi,int cost) override { _pi = pi; _estimated_cost = cost; _search = true; }
    void reset() override { _search = false; }
private:
    int                 _cost;
    unsigned            _pi;
    bool                _nearPin;
    unsigned            _pin;
    int                 _estimated_cost;
    bool                _search;
    //bool                _nearby;
    //bool                _init;
};

#endif