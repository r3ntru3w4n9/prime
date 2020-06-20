// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Node.h"

#include <assert.h>

TreeNode::TreeNode() noexcept : slf(-1), par(-1), lft(-1), rgt(-1) {}

TreeNode::TreeNode(unsigned self) noexcept
    : slf((int)self), par(-1), lft(-1), rgt(-1) {}

TreeNode::TreeNode(unsigned self,
                   unsigned parent,
                   unsigned left,
                   unsigned right) noexcept
    : slf((int)self), par((int)parent), lft((int)left), rgt((int)right) {}

bool TreeNode::has_self() const {
    return slf >= 0;
}
bool TreeNode::has_parent() const {
    return par >= 0;
}
bool TreeNode::has_left() const {
    return lft >= 0;
}
bool TreeNode::has_right() const {
    return rgt >= 0;
}

unsigned TreeNode::self() const {
    assert(has_self());
    return (unsigned)slf;
}
unsigned TreeNode::parent() const {
    assert(has_parent());
    return (unsigned)par;
}
unsigned TreeNode::left() const {
    assert(has_left());
    return (unsigned)lft;
}
unsigned TreeNode::right() const {
    assert(has_right());
    return (unsigned)rgt;
}

void TreeNode::self(unsigned s) {
    slf = (int)s;
}
void TreeNode::parent(unsigned p) {
    par = (int)p;
}
void TreeNode::left(unsigned l) {
    lft = (int)l;
}
void TreeNode::right(unsigned r) {
    rgt = (int)r;
}
