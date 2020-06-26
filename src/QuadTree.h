// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>

#include "QuadNode"
#include "safe.h"

class QuadTree {
   public:
    // constructor
    QuadTree() noexcept;

    const std::string& get_name() const { return NetName; }

    

   private:
    const std::string        NetName;
    int                     root_idx;
    safe::vector<QuadNode>     nodes;
};