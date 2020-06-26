// * Copyright (C) Heng-Jui Chang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <iostream>
#include <memory>
#include <assert.h>

#include "QuadNode.h"
#include "safe.h"

class QuadTree {
   public:
    // constructor
    QuadTree() noexcept;
    explicit QuadTree(std::string& s, int& n_id) noexcept;

    const std::string& get_name() const;
    const int get_net_id() const;

    void add_pin();
    void add_segment();
    void construct_tree();

   private:
    const std::string       _NetName;
    const int                 _NetId;
    int                     root_idx;
    safe::vector<QuadNode>     nodes;

    // private functions
    void expand_memory(); // TODO: not sure if this is needed (insertion/deletion?)
};

class NetSegment{
   public:
    // constructor
    NetSegment() noexcept
        : x_start(-1), y_start(-1), x_end(-1), y_end(-1) {}
    NetSegment(int xs, int ys, int xe, int ye) noexcept
        : x_start(xs), y_start(ys), x_end(xe), y_end(ye) {
            assert(x_start <= x_end);
            assert(y_start <= y_end);
        }

    const bool operator<(const NetSegment& ns) const {
        if      (x_start < ns.get_xs()) return true;
        else if (x_start > ns.get_xs()) return false;
        else if (x_end   < ns.get_xe()) return true;
        else if (x_end   > ns.get_xe()) return false;
        else return true;
    }

    const int get_xs() const { return x_start; }
    const int get_ys() const { return y_start; }
    const int get_xe() const { return   x_end; }
    const int get_ye() const { return   y_end; }
    // direction: true -> vertical, false -> horizontal
    const bool get_direction() const { return true ? (x_start < x_end) : false; }

    const bool check_instersect(const NetSegment& ns) const {
        // TODO:
    }

    void merge_segment(NetSegment& ns) {
        if(get_direction() != ns.get_direction() || !check_instersect(ns)) return;
        if(get_direction()){ // vertical
            if (x_start > ns.get_xs()) x_start = ns.get_xs();
            if (x_end   < ns.get_xe()) x_end   = ns.get_xe();
        } else {             // horizontal
            if (y_start > ns.get_ys()) x_start = ns.get_ys();
            if (y_end   < ns.get_ye()) x_end   = ns.get_ye();
        }
    }

   private:
    int x_start, x_end;
    int y_start, y_end;
};