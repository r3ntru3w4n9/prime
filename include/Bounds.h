// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

// A bound is a HPWL. As simple as that.
class Bounds {
   public:
    // an empty bound
    explicit Bounds(void) noexcept = delete;
    // create a bound
    explicit Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept;
    explicit Bounds(const Bounds& bounds) noexcept = delete;
    // replace another bound
    explicit Bounds(Bounds&& bounds) noexcept;
    // consumes two bounds
    explicit Bounds(Bounds&& a, Bounds&& b) noexcept;

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
