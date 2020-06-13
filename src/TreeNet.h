// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <assert.h>

#include <memory>
#include <string>

#include "Cell.h"
#include "safe.h"

// TODO
// ! not sure yet how to assign route to connect pins.
// ? connect the routes such that the points only go from pin to pin
// ? use the GridNet as a temporal holder

class TreeNet;

struct Point {
    Point(unsigned x, unsigned y, unsigned z) : x(x), y(y), z(z) {}

    // fields
    unsigned x, y, z;
};

struct Segment {
    Segment(unsigned s_x,
            unsigned s_y,
            unsigned s_z,
            unsigned t_x,
            unsigned t_y,
            unsigned t_z)
        : source(std::move(Point(s_x, s_y, s_z))),
          target(std::move(Point(t_x, t_y, t_z))) {}
    Segment(Point&& p1, Point&& p2)
        : source(std::move(p1)), target(std::move(p2)) {}

    Point source, target;
};

class TreeNode {
   public:
    TreeNode(unsigned idx) : pin_index(idx) {}
    ~TreeNode() {
        delete left;
        delete right;
    }

   private:
    unsigned pin_index;
    TreeNode *left, *right;
    safe::vector<Point> bends;
};

class TreeNet {
   public:
    // constructor
    TreeNet(std::string&& name,
            unsigned id,
            unsigned num_pins,
            unsigned min_layer)
        : net_name(std::move(name)), net_id(id), min_layer(min_layer) {
        connected.reserve(num_pins);
    }
    TreeNet(TreeNet&& tn)
        : net_name(std::move(tn.net_name)),
          net_id(tn.net_id),
          min_layer(tn.min_layer),
          connected(std::move(tn.connected)),
          tree_root(tn.tree_root),
          tree_size(tn.tree_size) {
        if (size()) {
            assert(tree_root != nullptr);
        }
        tn.tree_root = nullptr;
    }

    // destructor
    ~TreeNet() { delete tree_root; }

    // operator=
    TreeNet& operator=(TreeNet&& tn) {
        net_name = std::move(tn.net_name);
        net_id = tn.net_id;
        min_layer = tn.min_layer;

        connected = std::move(tn.connected);

        tree_root = tn.tree_root;
        tree_size = tn.tree_size;

        tn.tree_root = nullptr;

        return *this;
    }

    void add_pin(Pin&& pin) {
        assert(connected.size() < connected.capacity());
        connected.push_back(std::move(pin));
    }

    // void add_segments(const safe::vector<Segment>& segments) {
    //     // TODO
    //     // haven't worked out the details.
    //     // ? Use union-find to merge the segments and initialize the tree?
    // }

    size_t size() const { return connected.size(); }

   private:
    std::string net_name;
    unsigned net_id;
    unsigned min_layer;

    safe::vector<Pin> connected;

    TreeNode* tree_root;
    unsigned tree_size;
};