/***********************************************************************

  FileName    [Force.cpp]

  Author      [Yang Chien Yi]

  This file describes the force-directed method.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "Force.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*

 */

////////////////////////////////////////////////////////////////////////
///                           FUCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Force::Force(Chip& chip) : _chip(chip) {
    _row.reserve(_chip.getNumCells());
    _column.reserve(_chip.getNumCells());
    _rowNew.reserve(_chip.getNumCells());
    _columnNew.reserve(_chip.getNumCells());
    _rowSum.reserve(_chip.getNumNets());
    _columnSum.reserve(_chip.getNumNets());
    _movable.reserve(_chip.getNumCells());

    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        _row.push_back(_chip.getCell(i).getRow());
        _column.push_back(_chip.getCell(i).getColumn());
        _rowNew.push_back(0);
        _columnNew.push_back(0);
        _movable.push_back(true);
    }
}

void Force::balance() {
    balance_first();
}

void Force::balance_first() {
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        NetSum(i);
    }
    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        if (!_chip.getCell(i).movable(_chip.limited())) {
            _rowNew[i] = _row[i];
            _columnNew[i] = _column[i];
        } else {
            UpdateCell(i);
        }
    }
    
}

void Force::NetSum(unsigned idx) {
    unsigned rowSum = 0, columnSum = 0;
    GridNet& net = _chip.getNet(idx);
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        rowSum += _row[net.getCellIdx(i)];
        columnSum += _column[net.getCellIdx(i)];
    }
    _rowSum.push_back(rowSum);
    _columnSum.push_back(columnSum);
}

void Force::UpdateCell(unsigned idx) {
    Cell& cell = _chip.getCell(idx);
    unsigned rowNew = 0, columnNew = 0;
    for (unsigned i = 0; i < cell.getNumPins(); ++i) {
        Pin& pin = cell.getPin(i);
        rowNew += _rowSum[pin.get_net_idx()];
        columnNew += _columnSum[pin.get_net_idx()];
    }
    _rowNew[idx] = unsigned(double(rowNew)/cell.getNumPins() - _row[idx] + 0.5);
    _columnNew[idx] = unsigned(double(columnNew)/cell.getNumPins() - _column[idx] + 0.5);
}

unsigned Force::HPWL() const {
    unsigned ret = 0;
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        ret += HPWL_NET(i);
    }
    return ret;
}

unsigned Force::HPWL_NET(unsigned idx) const {
    GridNet& net = _chip.getNet(idx);
    unsigned maxRow = 1, minRow = _chip.getNumRows(),
             maxColumn = 1, minColumn = _chip.getNumColumns();
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        unsigned row = _row[net.getCellIdx(i)],
                 column = _column[net.getCellIdx(i)];
        if (row > maxRow) {
            maxRow = row;
        }
        else if (row < minRow) {
            minRow = row;
        }
        if (column > maxColumn) {
            maxColumn = column;
        }
        else if (column < minColumn) {
            minColumn = column;
        }
    }
    return maxRow - minRow + maxColumn - minColumn;
}