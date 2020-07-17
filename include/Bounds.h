// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include <memory>

#include "safe.h"

// TODO: finish tree implementation

// A bound is an HPWL. As simple as that.
class Bounds {
   public:
    // an empty bound
    constexpr Bounds(void) noexcept;
    // create a bound
    Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept;
    Bounds(const Bounds& bounds) noexcept;
    // replace another bound
    Bounds(Bounds&& bounds) noexcept;

    // construct bounds based on child
    Bounds(const Bounds& a, const Bounds& b) noexcept;

    // operators
    Bounds& operator=(const Bounds& b) noexcept;
    bool operator==(const Bounds& b) const noexcept;
    bool operator!=(const Bounds& b) const noexcept;

    bool is_initialized(void) const;

    unsigned get_top(void) const;
    unsigned get_bottom(void) const;
    unsigned get_left(void) const;
    unsigned get_right(void) const;

    void set_top(unsigned val);
    void set_bottom(unsigned val);
    void set_left(unsigned val);
    void set_right(unsigned val);

   private:
    int top, bottom, left, right;
};

class BoundsNode {
   public:
    explicit BoundsNode(void) noexcept;
    explicit BoundsNode(Bounds&& b) noexcept;
    explicit BoundsNode(Bounds&& b,
                        std::shared_ptr<BoundsNode> left,
                        std::shared_ptr<BoundsNode> right) noexcept;
    // ! primary constructor
    explicit BoundsNode(std::shared_ptr<BoundsNode> left,
                        std::shared_ptr<BoundsNode> right) noexcept;
    explicit BoundsNode(Bounds&& b,
                        std::shared_ptr<BoundsNode> left,
                        std::shared_ptr<BoundsNode> right,
                        std::shared_ptr<BoundsNode> parent) noexcept;
    explicit BoundsNode(BoundsNode&& other) noexcept;

    void left(std::shared_ptr<BoundsNode> lft);
    void right(std::shared_ptr<BoundsNode> rgt);
    void parent(std::shared_ptr<BoundsNode> par);

    std::weak_ptr<BoundsNode> left(void);
    const std::weak_ptr<BoundsNode> left(void) const;
    std::weak_ptr<BoundsNode> right(void);
    const std::weak_ptr<BoundsNode> right(void) const;

    std::weak_ptr<BoundsNode> parent(void);
    const std::weak_ptr<BoundsNode> parent(void) const;

    Bounds data(void) const;
    void data(Bounds b);

   private:
    Bounds d;
    std::shared_ptr<BoundsNode> l, r;
    std::weak_ptr<BoundsNode> p;
};

class BoundsTree {
   public:
    explicit BoundsTree(void) noexcept;
    explicit BoundsTree(Bounds&& b) noexcept;
    explicit BoundsTree(BoundsNode&& b) noexcept;
    explicit BoundsTree(BoundsTree&& bt) noexcept;
    explicit BoundsTree(BoundsTree&& a, BoundsTree&& b) noexcept;

    size_t size(void) const;

    BoundsTree left(void) const;
    BoundsTree right(void) const;

    void flatten(safe::vector<Bounds>& bounds) const;

   private:
    void size_check(void) const;

    std::shared_ptr<BoundsNode> root;
    size_t sz;
};
