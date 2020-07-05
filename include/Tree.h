// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <utility>

#include "Node.h"
#include "safe.h"
#include "union_find.h"

// * A tree only represents a structure
// ! no data involved
class Tree {
   public:
    // constructor

    Tree() noexcept;
    Tree(size_t size) noexcept;
    Tree(safe::vector<TreeNode>&& nodes) noexcept;
    Tree(Tree&& tn) noexcept;

    // operator=

    Tree& operator=(Tree&& tn) noexcept;

    // getter

    const TreeNode& get(size_t idx) const;
    TreeNode& get(size_t idx);

    bool has_self(size_t idx) const;
    bool has_parent(size_t idx) const;
    bool has_left(size_t idx) const;
    bool has_right(size_t idx) const;

    unsigned parent(size_t idx) const;
    unsigned left(size_t idx) const;
    unsigned right(size_t idx) const;

    const TreeNode& operator[](size_t idx) const;
    TreeNode& operator[](size_t idx);

    const TreeNode& at(size_t idx) const;
    TreeNode& at(size_t idx);

    bool is_leaf(size_t idx) const;

    void push_back(TreeNode&& node);

    size_t size() const;

    const safe::vector<TreeNode>& field(void) const;
    safe::vector<TreeNode>& field(void);

   protected:
    // fields
    safe::vector<TreeNode> nodes;

    // friends
    friend std::ostream& operator<<(std::ostream& out, const Tree& tree);
};
