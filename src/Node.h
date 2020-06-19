// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>
#include <iostream>
#include <memory>
#include <sstream>

template <typename T>
class Node {
   public:
    // constructor
    Node(T&& data)
        : data(std::move(data)),
          parent(nullptr),
          left(nullptr),
          right(nullptr) {}

    Node(T&& data, Node& parent) {}
    Node(T&& data, Node& parent, Node& left, Node& right)
        : data(std::move(data)), parent(&parent), left(&left), right(&right) {}
    ~Node() {
        delete left;
        delete right;
    }

    // operator=
    Node& operator=(Node&& n) {
        data = std::move(n.data);
        parent = n.parent;
        left = n.left;
        right = n.right;

        n.parent = nullptr;
        n.left = nullptr;
        n.right = nullptr;

        return *this;
    }

    // attribute
    bool has_parent() const { return p != nullptr; }
    bool has_left() const { return l != nullptr; }
    bool has_right() const { return r != nullptr; }

    // setter
    void parent(Node& n) { p = &n; }
    void left(Node& n) { l = &n; }
    void right(Node& n) { r = &n; }

    // getter
    Node& parent() {
        assert(has_parent());
        return *p;
    }
    const Node& parent() const {
        assert(has_parent());
        return *p;
    }
    Node& left() {
        assert(has_left());
        return *l;
    }
    const Node& left() const {
        assert(has_left());
        return *l;
    }
    Node& right() {
        assert(has_right());
        return *r;
    }
    const Node& right() const {
        assert(has_right());
        return *r;
    }

   private:
    // fields
    T data;

    Node *p, *l, *r;
};
