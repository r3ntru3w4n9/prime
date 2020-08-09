/***********************************************************************

  FileName    [Grid.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file defines the Layers, the Grids, and their Coordinates(to
  save cells).

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "place.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                           FUNCTIONS                              ///
////////////////////////////////////////////////////////////////////////

BoundingNet::BoundingNet(Chip& chp, GridNet& net)
    : _leftmost(chp.getNumColumns() - 1),
      _leftnext(chp.getNumColumns() - 1),
      _rightmost(0),
      _rightnext(0),
      _topmost(0),
      _topnext(0),
      _bottommost(chp.getNumRows() - 1),
      _bottomnext(chp.getNumRows() - 1) {
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        const Pin& pin = chp.getPin(net.getPinIdx(i));
        unsigned row = chp.getPinRow(pin), column = chp.getPinColumn(pin);
        updatePos(row, column, pin.getIdx());
    }
}

BoundingNet::BoundingNet(BoundingNet&& other)
    : _leftmost(other._leftmost),
      _leftnext(other._leftnext),
      _rightmost(other._rightmost),
      _rightnext(other._rightnext),
      _topmost(other._topmost),
      _topnext(other._topnext),
      _bottommost(other._bottommost),
      _bottomnext(other._bottomnext),
      _leftmostpin(other._leftmostpin),
      _rightmostpin(other._rightmostpin),
      _topmostpin(other._topmostpin),
      _bottommostpin(other._bottommostpin) {}

unsigned BoundingNet::getLeftmost() const {
    return _leftmostpin;
}

unsigned BoundingNet::getRightmost() const {
    return _rightmostpin;
}

unsigned BoundingNet::getTopmost() const {
    return _topmostpin;
}

unsigned BoundingNet::getBottommost() const {
    return _bottommostpin;
}

unsigned BoundingNet::getLeftRange() const {
    assert(_leftnext >= _leftmost);
    return _leftnext - _leftmost;
}
unsigned BoundingNet::getRightRange() const {
    assert(_rightnext <= _rightmost);
    return _rightmost - _rightnext;
}
unsigned BoundingNet::getUpRange() const {
    assert(_topnext <= _topmost);
    return _topmost - _topnext;
}
unsigned BoundingNet::getDownRange() const {
    assert(_bottomnext >= _bottommost);
    return _bottomnext - _bottommost;
}

void BoundingNet::updatePos(const unsigned row,
                            const unsigned column,
                            const unsigned pin) {
    if (column < _leftmost) {
        _leftnext = _leftmost;
        _leftmost = column;
        _leftmostpin = pin;
    } else if (column < _leftnext) {
        _leftnext = column;
    }

    if (column > _rightmost) {
        _rightnext = _rightmost;
        _rightmost = column;
        _rightmostpin = pin;
    } else if (column > _rightnext) {
        _rightnext = column;
    }

    if (row < _bottommost) {
        _bottomnext = _bottommost;
        _bottommost = row;
        _bottommostpin = pin;
    } else if (row < _bottomnext) {
        _bottomnext = row;
    }

    if (row > _topmost) {
        _topnext = _topmost;
        _topmost = row;
        _topmostpin = pin;
    } else if (row > _topnext) {
        _topnext = row;
    }
}

moveCell::moveCell() {}
moveCell::moveCell(moveCell&& other)
    : _H(other._H),
      _V(other._V),
      _rightRange(other._rightRange),
      _leftRange(other._leftRange),
      _upRange(other._upRange),
      _downRange(other._downRange) {}

void moveCell::setleftRange(const unsigned range) {
    _H--;
    if (range < _leftRange) {
        _leftRange = range;
    }
}

void moveCell::setrightRange(const unsigned range) {
    _H++;
    if (range < _rightRange) {
        _rightRange = range;
    }
}

void moveCell::setupRange(const unsigned range) {
    _V++;
    if (range < _upRange) {
        _upRange = range;
    }
}

void moveCell::setdownRange(const unsigned range) {
    _V--;
    if (range < _downRange) {
        _downRange = range;
    }
}

Place::Place(Chip& chp) : _chp(chp) {
    _nets.reserve(_chp.getNumNets());
    for (unsigned i = 0; i < _chp.getNumCells(); ++i) {
        _cells.push_back(std::move(moveCell()));
    }
    for (unsigned i = 0; i < _chp.getNumNets(); ++i) {
        _nets.push_back(std::move(BoundingNet(_chp, _chp.getNet(i))));
        updateCell(i);
    }
    for (unsigned i = 0; i < _chp.getNumCells(); ++i) {
        // TODO move
    }
}

inline void Place::updateCell(const unsigned i) {
    BoundingNet& net = _nets[i];
    unsigned cell;
    cell = _chp.getPin(net.getLeftmost()).get_cell_idx();
    _cells[cell].setleftRange(net.getLeftRange());
    cell = _chp.getPin(net.getRightmost()).get_cell_idx();
    _cells[cell].setrightRange(net.getRightRange());
    cell = _chp.getPin(net.getTopmost()).get_cell_idx();
    _cells[cell].setupRange(net.getUpRange());
    cell = _chp.getPin(net.getBottommost()).get_cell_idx();
    _cells[cell].setdownRange(net.getDownRange());
}