/***********************************************************************

  FileName    [SimpleRouter.cpp]

  Author      [Kan-Hsuan, Lai]

  This file describes the functions in "SimpleRouter.h".

***********************************************************************/

#include "SimpleRouter.h"
#include "Cell.h"

SimpleRouter::SimpleRouter(Chip& chip) {
    
    for(unsigned i = 0; i < chip.getNumNets(); i++) {
        GridNet& net = chip.getNet(i);
        safe::list<BoundsTree> forest;
        
        for(int j = 0; j < net.getNumPin(); j++) {
            Cell& cell = chip.getCell(chip.getPin(net.getPinIdx(j)).get_cell_idx());
            unsigned x = cell.getRow(), y = cell.getColumn();

            forest.push_back(BoundsTree(Bounds(x, x, y, y)));
        }

        while(forest.size() > 1)
        {
            BoundsTree& a = forest.front();
            forest.pop_front();
            BoundsTree& b = forest.front();
            forest.pop_front();
            forest.push_back(BoundsTree(std::move(a), std::move(b)));
        }

        BoundsTree& root = forest.front();
        safe::list<EndPoints> connections = root.mark_edge();
    }
}