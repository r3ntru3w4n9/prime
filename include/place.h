/***********************************************************************

  FileName    [place.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file defines the placement method considering only the pins on
  the bounding box.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Chip.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

typedef safe::vector<std::pair<unsigned, unsigned>> argList;  // arg, val

class BoundingNet {
   public:
    // constructor
    BoundingNet(Chip& chp, GridNet& net);
    BoundingNet(BoundingNet&& other);
    BoundingNet(const BoundingNet& other) = delete;

    // access function
    unsigned getLeftmost() const;
    unsigned getRightmost() const;
    unsigned getTopmost() const;
    unsigned getBottommost() const;
    unsigned getLeftRange() const;
    unsigned getRightRange() const;
    unsigned getUpRange() const;
    unsigned getDownRange() const;

   private:
    unsigned _leftmost, _leftnext;
    unsigned _rightmost, _rightnext;
    unsigned _topmost, _topnext;
    unsigned _bottommost, _bottomnext;
    unsigned _leftmostpin;
    unsigned _rightmostpin;
    unsigned _topmostpin;
    unsigned _bottommostpin;

    // private function
    void updatePos(const unsigned row,
                   const unsigned column,
                   const unsigned pin);
};

class moveCell {
   public:
    moveCell();
    moveCell(moveCell&& other);
    moveCell(const moveCell& other) = delete;

    // set range
    void setleftRange(const unsigned range);
    void setrightRange(const unsigned range);
    void setupRange(const unsigned range);
    void setdownRange(const unsigned range);

    // access function
    int getHgain() const;
    int getVgain() const;
    int getH() const;
    int getV() const;
    unsigned getRightRange() const;
    unsigned getLeftRange() const;
    unsigned getDownRange() const;
    unsigned getUpRange() const;

   private:
    int _H;
    int _V;
    unsigned _rightRange;
    unsigned _leftRange;
    unsigned _upRange;
    unsigned _downRange;
};

class Place {
   public:
    Place(Chip& chp);

   private:
    Chip& _chp;
    safe::vector<BoundingNet> _nets;
    safe::vector<moveCell> _cells;

    // private function
    void updateCell(const unsigned i);
};

bool myfunc(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b);