/***********************************************************************

  FileName    [Cell.h]

  Author      [Yang, Chien Yi]

  This file defines the cells and their elements, pins and nets.

***********************************************************************/

#ifndef CELL_H
#define CELL_H

////////////////////////////////////////////////////////////////////////
///                          INCLUDES                                ///
////////////////////////////////////////////////////////////////////////

#include "MasterCell.h"

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////

class Net;

class Pin
{
public:
    //Constructors
    Pin(PinType& PT, Net& net): _PT(PT), _net(net) {}
    Pin(Pin& a): _PT(a._PT), _net(a._net) {}
private:
    PinType&                            _PT;
    Net&                                _net;
};

class Cell
{
public:
    //Constructors(no copy constructor)
    Cell(const std::string& CellName, MasterCellType& MCT, bool movable):
    _CellName(CellName), _MCT(MCT), _movable(movable)
    {

    }

    //modifier
    void setX(unsigned x)                               { _x = x; }
    void setY(unsigned y)                               { _y = y; }

    //acceser
    std::string getCellName() const                     { return _CellName; }
    MasterCellType& getMCType() const                   { return _MCT; }
    unsigned getX() const                               { return _x; }
    unsigned getY() const                               { return _y; }

    //friend
    friend std::ostream& operator<<(std::ostream& os, const Cell& cell);

private:
    const std::string                   _CellName;
    MasterCellType&                     _MCT;
    const bool                          _movable;
    unsigned                            _x;
    unsigned                            _y;
    std::vector<Pin>                    _pins;
};

std::ostream& operator<<(std::ostream& os, const Cell& cell);

#endif