// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

// TODO handle union find and merging tree
// Merging Tree Node

#pragma once

#include "Node.h"

// ? inheritance vs new type pattern
template <typename T>
class UnionFindNode {
   public:
    // constructor
    UnionFindNode(Node&& n) : node : (std::move(n){}) {}

    // operator=
    UnionFindNode(UnionFindNode&& ufn) { node = std::move(ufn.node); }

    // attribute
    bool has_parent() const { return node.has_parent(); }
    bool has_left() const { return node.has_left(); }
    bool has_right() const { return node.has_right(); }

    // getter
    UnionFindNode& parent() { return UnionFindNode }

   private:
    Node<T> node;
};

template <typename T>
Node<T>& find(UnionFindNode<T>& node) {
    if (node.has_parent()) {
        Node& parent = node.parent();
        node.parent(find(parent));
    } else {
        return node;
    }
}
