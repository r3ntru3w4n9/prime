// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <memory>

#include "safe.h"

// * UnionFind uses its parent pointer
// * Tree uses its left child and right child
class TreeNode {
   public:
    TreeNode() noexcept;
    TreeNode(unsigned self) noexcept;
    TreeNode(unsigned self,
             unsigned parent,
             unsigned left,
             unsigned right) noexcept;

    bool has_self() const;
    bool has_parent() const;
    bool has_left() const;
    bool has_right() const;

    unsigned self() const;
    unsigned parent() const;
    unsigned left() const;
    unsigned right() const;

    void self(unsigned s);
    void parent(unsigned p);
    void left(unsigned l);
    void right(unsigned r);

   private:
    int slf, par, lft, rgt;
};
