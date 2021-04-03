/***********************************************************************

  FileName    [SimpleRouter.h]

  Author      [Kan-Hsuan, Lai]

  This file difines the SimpleRouter (connect the pins using merging tree).

***********************************************************************/

#pragma once

#include "Bounds.h"
#include "Chip.h"

class SimpleRouter {
   public:
    SimpleRouter(Chip& chip);
};