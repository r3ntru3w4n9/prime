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

constexpr Bounds::Bounds(void) noexcept : t(0), b(0), l(0), r(0) {}

Bounds::Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept
    : t(t), b(b), l(l), r(r) {}

Bounds::Bounds(const Bounds& b) noexcept : t(b.t), b(b.b), l(b.l), r(b.r) {}

Bounds::Bounds(Bounds&& b) noexcept : t(b.t), b(b.b), l(b.l), r(b.r) {}

Bounds::Bounds(const Bounds& a, const Bounds& b) noexcept {
    t = maximum(a.t, b.t);
    b = minimum(a.b, b.b);
    l = minimum(a.l, b.l);
    r = maximum(a.r, b.r);
}

inline Bounds& Bounds::operator=(const Bounds& o) noexcept {
    t = o.t;
    b = o.b;
    l = o.l;
    r = o.r;
    return *this;
}

inline bool Bounds::operator==(const Bounds& o) const noexcept {
    return t == o.t && b == o.b && l == o.l && r == o.r;
}

inline bool Bounds::operator!=(const Bounds& b) const noexcept {
    return !(*this == b);
}

inline bool Bounds::is_initialized(void) const {
    return *this != Bounds();
}

inline unsigned Bounds::top(void) const {
    return t;
}

inline unsigned Bounds::bottom(void) const {
    return b;
}

inline unsigned Bounds::left(void) const {
    return l;
}

inline unsigned Bounds::right(void) const {
    return r;
}

inline void Bounds::top(unsigned val) {
    t = (int)val;
}

inline void Bounds::bottom(unsigned val) {
    b = (int)val;
}

inline void Bounds::left(unsigned val) {
    l = (int)val;
}

inline void Bounds::right(unsigned val) {
    r = (int)val;
}

inline bool Bounds::in_box(unsigned x, unsigned y) const {
    return x <= t && x >= b && y <= r && y >= l;
}

inline std::pair<Corner, Corner> Bounds::overlap_with(const Bounds& oth) const {
    return std::make_pair(contains_point(oth), oth.contains_point(*this));
}

inline Corner Bounds::contains_point(const Bounds& oth) const {
    if (in_box(oth.l, oth.b)) {
        return Corner::BL;
    }
    if (in_box(oth.l, oth.t)) {
        return Corner::TL;
    }
    if (in_box(oth.r, oth.b)) {
        return Corner::BR;
    }
    if (in_box(oth.r, oth.t)) {
        return Corner::TR;
    }

    return Corner::None;
}

inline std::pair<unsigned, unsigned> Bounds::corner(Corner cor) const {
    switch (cor) {
        case Corner::TL:
            return std::make_pair(left(), top());
        case Corner::BL:
            return std::make_pair(left(), bottom());
        case Corner::TR:
            return std::make_pair(right(), top());
        case Corner::BR:
            return std::make_pair(right(), bottom());
    }
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

BoundsNode::BoundsNode(BoundsNode&& other) noexcept
    : d(std::move(other.d)),
      l(std::move(other.l)),
      r(std::move(other.r)),
      p(std::move(other.p)) {}

std::shared_ptr<BoundsNode> BoundsNode::left(void) {
    return l;
}

const std::shared_ptr<BoundsNode> BoundsNode::left(void) const {
    return l;
}

void BoundsNode::left(std::shared_ptr<BoundsNode> lft) {
    l = lft;
}

std::shared_ptr<BoundsNode> BoundsNode::right(void) {
    return l;
}

const std::shared_ptr<BoundsNode> BoundsNode::right(void) const {
    return l;
}

void BoundsNode::right(std::shared_ptr<BoundsNode> lft) {
    l = lft;
}

std::shared_ptr<BoundsNode> BoundsNode::parent(void) {
    return l;
}

const std::shared_ptr<BoundsNode> BoundsNode::parent(void) const {
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

inline void BoundsTree::validate(void) const {
    size_check();
    bounds_check();
}

inline void BoundsTree::size_check(void) const {
    safe::vector<Bounds> b;
    flatten(b);
    assert(size() == b.size());
}

static void recursive_bounds_check(std::shared_ptr<BoundsNode> node) {
    if (node == nullptr) {
        return;
    }

    std::shared_ptr<BoundsNode> l = node->left(), r = node->right();

    if (l == nullptr && r == nullptr) {
    } else if (l == nullptr) {
        assert(node->data() == r->data());
    } else if (r == nullptr) {
        assert(node->data() == l->data());
    } else {
        assert(node->data() == Bounds(l->data(), r->data()));
    }

    recursive_bounds_check(l);
    recursive_bounds_check(r);
}

inline void BoundsTree::bounds_check(void) const {
    recursive_bounds_check(root);
}

safe::list<EndPoints> BoundsNode::top_down(void) const {
    auto left = l, right = r;

    auto llist = l->top_down(), rlist = r->top_down();

    llist.splice(llist.end(), rlist);
    assert(rlist.empty());

    EndPoints ep;

    auto lbox = l->data(), rbox = r->data();

    auto contains = lbox.overlap_with(rbox);

    Corner lcontr = contains.first, rcontl = contains.second;

    EndPoints ep;

    // TODO: finish this part
    if (lcontr == Corner::None && rcontl == Corner::None) {
        bool tb = lbox.top()<rbox.bottom(), bt = lbox.bottom()> rbox.top(),
             lr = lbox.left() > rbox.right(), rl = lbox.right() < rbox.left();

        if (tb || bt || lr || rl) {
            if (tb) {
                if (lr) {
                } else {
                    assert(rl);
                }
            } else {
                assert(bt);
                if (lr) {
                } else {
                    assert(rl);
                }
            }
        }
    } else if (lcontr == Corner::None) {
    } else if (rcontl == Corner::None) {
    } else {
        assert(lcontr != Corner::None && rcontl != Corner::None);
    }

    llist.push_front(ep);

    return llist;
}

safe::list<EndPoints> BoundsTree::top_down(void) const {
    return root->top_down();
}
