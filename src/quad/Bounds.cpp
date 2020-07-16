// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "Bounds.h"

#include <assert.h>

template <typename T>
inline T maximum(T a, T b) {
    return a < b ? b : a;
}

template <typename T>
inline T minimum(T a, T b) {
    return a > b ? b : a;
}

constexpr Bounds::Bounds(void) noexcept
    : top(0), bottom(0), left(0), right(0) {}

Bounds::Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept
    : top(t), bottom(b), left(l), right(r) {}

Bounds::Bounds(const Bounds& b) noexcept
    : top(b.top), bottom(b.bottom), left(b.left), right(b.right) {}

Bounds::Bounds(Bounds&& b) noexcept
    : top(b.top), bottom(b.bottom), left(b.left), right(b.right) {}

Bounds::Bounds(const Bounds& a, const Bounds& b) noexcept {
    top = maximum(a.top, b.top);
    bottom = minimum(a.bottom, b.bottom);
    left = minimum(a.left, b.left);
    right = maximum(a.right, b.right);
}

inline Bounds& Bounds::operator=(const Bounds& b) noexcept {
    top = b.top;
    bottom = b.bottom;
    left = b.left;
    right = b.right;
    return *this;
}

inline bool Bounds::operator==(const Bounds& b) const noexcept {
    return top == b.top && bottom == b.bottom && left == b.left &&
           right == b.right;
}

inline bool Bounds::operator!=(const Bounds& b) const noexcept {
    return !(*this == b);
}

inline bool Bounds::is_initialized(void) const {
    return *this != Bounds();
}

inline unsigned Bounds::get_top(void) const {
    return top;
}

inline unsigned Bounds::get_bottom(void) const {
    return bottom;
}

inline unsigned Bounds::get_left(void) const {
    return left;
}

inline unsigned Bounds::get_right(void) const {
    return right;
}

inline void Bounds::set_top(unsigned val) {
    top = val;
}

inline void Bounds::set_bottom(unsigned val) {
    bottom = val;
}

inline void Bounds::set_left(unsigned val) {
    left = val;
}

inline void Bounds::set_right(unsigned val) {
    right = val;
}

BoundsNode::BoundsNode(void) noexcept
    : BoundsNode(Bounds(), nullptr, nullptr, nullptr) {}

BoundsNode::BoundsNode(Bounds&& b) noexcept
    : BoundsNode(std::move(b), nullptr, nullptr, nullptr) {}

BoundsNode::BoundsNode(Bounds&& b,
                       std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right) noexcept
    : BoundsNode(std::move(b), left, right, nullptr) {}

BoundsNode::BoundsNode(std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right) noexcept
    : BoundsNode(Bounds(left->data(), right->data()), left, right, nullptr) {}

BoundsNode::BoundsNode(Bounds&& b,
                       std::shared_ptr<BoundsNode> left,
                       std::shared_ptr<BoundsNode> right,
                       std::shared_ptr<BoundsNode> parent) noexcept
    : d(std::move(b)), l(left), r(right), p(parent) {}

std::weak_ptr<BoundsNode> BoundsNode::left(void) {
    return l;
}

const std::weak_ptr<BoundsNode> BoundsNode::left(void) const {
    return l;
}

void BoundsNode::left(std::shared_ptr<BoundsNode> lft) {
    l = lft;
}

std::weak_ptr<BoundsNode> BoundsNode::right(void) {
    return l;
}

const std::weak_ptr<BoundsNode> BoundsNode::right(void) const {
    return l;
}

void BoundsNode::right(std::shared_ptr<BoundsNode> lft) {
    l = lft;
}

std::weak_ptr<BoundsNode> BoundsNode::parent(void) {
    return l;
}

const std::weak_ptr<BoundsNode> BoundsNode::parent(void) const {
    return l;
}

void BoundsNode::parent(std::shared_ptr<BoundsNode> lft) {
    l = lft;
}

Bounds BoundsNode::data(void) const {
    return d;
}

void BoundsNode::data(Bounds bnd) {
    d = bnd;
}

BoundsTree::BoundsTree(void) noexcept
    : root(std::shared_ptr<BoundsNode>(nullptr)), sz(0) {}

BoundsTree::BoundsTree(Bounds&& b) noexcept
    : root(std::make_shared<BoundsNode>(std::move(b))), sz(1) {}

BoundsTree::BoundsTree(BoundsNode&& b) noexcept
    : root(std::make_shared<BoundsNode>(std::move(b))), sz(1) {}

BoundsTree::BoundsTree(BoundsTree&& bt) noexcept
    : root(std::move(bt.root)), sz(bt.sz) {}

BoundsTree::BoundsTree(BoundsTree&& a, BoundsTree&& b) noexcept
    : root(std::make_shared<BoundsNode>(a.root, b.root)), sz(a.sz + b.sz + 1) {}

inline size_t BoundsTree::size(void) const {
    return sz;
}

void BoundsTree::flatten(safe::vector<Bounds>& bounds) const {
    if (root == nullptr) {
        assert(sz == 0);
        return;
    }

    bounds.push_back(root->data());
}

inline void BoundsTree::size_check(void) const {
    safe::vector<Bounds> b;
    flatten(b);
    assert(size() == b.size());
}
