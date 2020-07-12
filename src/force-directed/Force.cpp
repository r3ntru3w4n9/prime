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
    _movable.reserve(_chip.getMaxMove());

    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        _row.push_back(_chip.getCell(i).getRow());
        _column.push_back(_chip.getCell(i).getColumn());
        _rowNew.push_back(0);
        _columnNew.push_back(0);
    }
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        _rowSum.push_back(0);
        _columnSum.push_back(0);
    }
}

void Force::balance(unsigned ite) {
    balance_init();
    balance_ite(ite - 1);
}

inline void Force::balance_init() {
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
    argList list;
    for (unsigned i = 0; i < _chip.getNumCells(); ++i) {
        if (_chip.getCell(i).movable(_chip.limited())) {
            unsigned row = _row[i], column = _column[i],
                     rowNew = _rowNew[i], columnNew = _columnNew[i];
            unsigned rowDiff = row > rowNew ? row - rowNew : rowNew - row;
            unsigned columnDiff = column > columnNew ? column - columnNew : columnNew;
            list.push_back(std::make_pair(i, rowDiff + columnDiff));
        }
    }
    std::sort(list.begin(), list.end(), myfunc);
    for (auto i = list.begin(); i != list.end(); ++i) {
        if (i->second == 0) {
            break;
        }
        unsigned idx = i->first;
        _movable.push_back(idx);
        _column[idx] = _columnNew[idx];
        _row[idx] = _rowNew[idx];
    }
}

inline void balance_ite(unsigned ite){

}

inline void Force::NetSum(unsigned idx) {
    unsigned rowSum = 0, columnSum = 0;
    GridNet& net = _chip.getNet(idx);
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        unsigned cell = _chip.getPin(net.getPinIdx(i)).get_cell_idx();
        rowSum += _row[cell];
        columnSum += _column[cell];
    }
    _rowSum[idx] = rowSum;
    _columnSum[idx] = columnSum;
}

inline void Force::UpdateCell(unsigned idx) {
    Cell& cell = _chip.getCell(idx);
    unsigned rowNew = 0, columnNew = 0;
    for (unsigned i = 0; i < cell.getNumPins(); ++i) {
        const Pin& pin = _chip.getPin(cell.getPinIdx(i));
        rowNew += _rowSum[pin.get_net_idx()];
        columnNew += _columnSum[pin.get_net_idx()];
    }
    _rowNew[idx] = unsigned(double(rowNew)/cell.getNumPins() - _row[idx] + 0.5);
    _columnNew[idx] = unsigned(double(columnNew)/cell.getNumPins() - _column[idx] + 0.5);
}

inline unsigned Force::HPWL() const {
    unsigned ret = 0;
    for (unsigned i = 0; i < _chip.getNumNets(); ++i) {
        ret += HPWL_NET(i);
    }
    return ret;
}

inline unsigned Force::HPWL_NET(unsigned idx) const {
    GridNet& net = _chip.getNet(idx);
    unsigned maxRow = 1, minRow = _chip.getNumRows(),
             maxColumn = 1, minColumn = _chip.getNumColumns();
    for (unsigned i = 0; i < net.getNumPin(); ++i) {
        unsigned cell = _chip.getPin(net.getPinIdx(i)).get_cell_idx();
        unsigned row = _row[cell],
                 column = _column[cell];
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

bool myfunc(std::pair<unsigned, unsigned> a, std::pair<unsigned, unsigned> b) {
    return a.second > b.second;
}