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
    Segment(Point&& p1, Point&& p2)
        : source(std::move(p1)), target(std::move(p2)) {}

    Point source, target;
};

class TreeNode {
   public:
    TreeNode(unsigned idx) : pin_index(idx) {}

   private:
    unsigned pin_index;
    TreeNode *left, *right;
    safe::vector<Point> bends;
};

class TreeNet {
   public:
    TreeNet(std::string&& name,
            unsigned id,
            unsigned num_pins,
            unsigned min_layer)
        : net_name(std::move(name)), net_id(id), min_layer(min_layer) {
        connected.reserve(num_pins);
    }

    void add_pin(Pin&& pin) {
        assert(connected.size() < connected.capacity());
        connected.push_back(std::move(pin));
    }

    void add_segments(const safe::vector<Segment>& segments) {
        // TODO
        // haven't worked out the details.
        // ? Use union-find to merge the segments and initialize the tree?
    }

   private:
    std::string net_name;
    unsigned net_id;
    unsigned min_layer;

    safe::vector<Pin> connected;

    TreeNode* tree_root;
    unsigned tree_size;
};