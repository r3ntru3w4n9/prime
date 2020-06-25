// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO handle union find and merging tree
// Merging Tree Node

#pragma once

#include "Node.h"
#include "Tree.h"
#include "safe.h"

class MergingTree {
   public:
    // constructor

    MergingTree(void) noexcept;
    MergingTree(size_t size) noexcept;
    explicit MergingTree(Tree&& t) noexcept;
    explicit MergingTree(MergingTree&& mt) noexcept;

    // operator=

    MergingTree& operator=(MergingTree&& mt) noexcept;

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

    template <bool orphan = false>
    void union_find(const safe::vector<std::pair<unsigned, unsigned>>& pairs) {
        run_union_find<orphan>(nodes, pairs);
    }

   private:
    Tree tree;
};
