/***********************************************************************

  FileName    [L_shape.cpp]

  Author      [Yang Chien Yi]

  This file implements the L_shape router.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <climits>
#include <cmath>

#include "router3d.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

bool isOdd(int a) {
    return a & 01;
}

bool isEven(int a) {
    return !isOdd(a);
}

bool Router3D::L_shape(const unsigned srow,
                       const unsigned scol,
                       const unsigned slay,
                       const unsigned erow,
                       const unsigned ecol,
                       const unsigned elay,
                       const GridNet& net,
                       IdxList& ans) {
    // H-V L shape
    safe::vector<int> minSupplyTable1(_pm.getNumLayers(),
                                     -1);  // -1 means not available
    safe::vector<unsigned> cost1(_pm.getNumLayers(),
                                UINT_MAX);  // MAX means not available
    Rout_H(slay, srow, scol, ecol, net, minSupplyTable1, cost1);
    Rout_V(elay, ecol, erow, srow, net, minSupplyTable1, cost1);



    // V_H L shape
    safe::vector<int> minSupplyTable2(_pm.getNumLayers(),
                                     -1);  // -1 means not available
    safe::vector<unsigned> cost2(_pm.getNumLayers(),
                                UINT_MAX);  // MAX means not available
    Rout_V(slay, scol, srow, erow, net, minSupplyTable2, cost2);
    Rout_H(elay, erow, ecol, scol, net, minSupplyTable2, cost2);

}

inline bool Router3D::Layer_Assignment_H(const unsigned scol,
                                  const unsigned ecol,
                                  const unsigned row,
                                  const unsigned lay,
                                  const GridNet& net,
                                  int& minSupply) {
    for (unsigned i = scol; i <= ecol; (scol < ecol) ? ++i : --i) {
        Grid& grid = _pm.getGrid(lay, _pm.getIdx(row, i));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}

inline bool Router3D::Layer_Assignment_V(const unsigned srow,
                                  const unsigned erow,
                                  const unsigned col,
                                  const unsigned lay,
                                  const GridNet& net,
                                  int& minSupply) {
    for (unsigned i = srow; i <= erow; (srow < erow) ? ++i : --i) {
        Grid& grid = _pm.getGrid(lay, _pm.getIdx(i, col));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}

inline bool Router3D::Via_Assignment(const unsigned slay,
                              const unsigned elay,
                              const unsigned row,
                              const unsigned col,
                              const GridNet& net,
                              int& minSupply) {
    for (unsigned i = slay; i <= elay; (slay < elay) ? ++i : --i) {
        Grid& grid = _pm.getGrid(i, _pm.getIdx(row, col));
        if (!grid.routable(net)) {
            return false;
        }
        int supply = grid.getSupply();
        if (supply < minSupply) {
            minSupply = supply;
        }
    }
    return true;
}

inline void Router3D::Rout_H(const unsigned lay,
                             const unsigned row,
                             const unsigned scol,
                             const unsigned ecol,
                             const GridNet& net,
                             safe::vector<int>& minSupplyTable,
                             safe::vector<unsigned>& cost) {
    // H down
    for (int i = lay, j = lay; i >= net.getMinlayer();
         (isEven(i)) ? i -= 1 : i -= 2) {
        if (isEven(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, row, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, row, i, net, minSupply)) {
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
    // H up
    for (int i = lay, j = lay; i < _pm.getNumLayers() && i >= net.getMinlayer();
         (isEven(i)) ? i += 1 : i += 2) {
        if (isEven(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, row, scol, net, minSupply) &&
            Layer_Assignment_H(scol, ecol, row, i, net, minSupply)) {
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
}

inline void Router3D::Rout_V(const unsigned lay,
                             const unsigned col,
                             const unsigned srow,
                             const unsigned erow,
                             const GridNet& net,
                             safe::vector<int>& minSupplyTable,
                             safe::vector<unsigned>& cost) {
    // V down
    for (int i = lay, j = lay; i >= net.getMinlayer();
         (isOdd(i)) ? i -= 2 : i -= 1) {
        if (isOdd(i)) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, erow, col, net, minSupply) &&
            Layer_Assignment_V(srow, erow, col, i, net, minSupply)) {
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
    // V up
    for (int i = lay, j = lay; i < _pm.getNumLayers() && i >= net.getMinlayer();
         (isOdd(i)) ? i += 2 : i += 1) {
        if (!(isOdd(i))) {
            continue;
        }
        int minSupply = INT_MAX;
        if (Via_Assignment(j, i, erow, col, net, minSupply) &&
            Layer_Assignment_V(srow, erow, col, i, net, minSupply)) {
            minSupplyTable[i] = minSupply;
            cost[i] = abs(i - lay);
            j = i;
        }
    }
}