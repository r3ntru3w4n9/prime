// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO handle union find and merging tree
// Merging Tree Node

#pragma once

#include <assert.h>

#include <memory>

#include "Node.h"
#include "safe.h"

template <typename T>
class UnionFindTree {
   public:
    // constructor
    UnionFindTree(safe::vector<T>&& data_list) noexcept
        : data(std::move(data_list)) {
        nodes = safe::vector<TreeNode>();
        nodes.reserve(data.size());
        for (unsigned i = 0; i < data.size(); ++i) {
            nodes.push_back(TreeNode((unsigned)i));
        }
    }
    UnionFindTree(UnionFindTree&& tn) noexcept
        : nodes(std::move(tn.nodes)), data(std::move(tn.data)) {}

    // operator=
    UnionFindTree& operator=(UnionFindTree&& tn) {
        nodes = std::move(tn.nodes);
        data = std::move(tn.data);

        return *this;
    }

    const T& operator[](size_t idx) const { return data[idx]; }
    T& operator[](size_t idx) { return data[idx]; }

    const T& at(size_t idx) const { return (*this)[idx]; }
    T& at(size_t idx) { return (*this)[idx]; }

   private:
    // fields
    safe::vector<TreeNode> nodes;
    safe::vector<T> data;
};
