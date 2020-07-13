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

Bounds::Bounds(unsigned t, unsigned b, unsigned l, unsigned r) noexcept
    : top(t), bottom(b), left(l), right(r) {}

Bounds::Bounds(Bounds&& b) noexcept
    : top(b.top), bottom(b.bottom), left(b.left), right(b.right) {}

Bounds::Bounds(Bounds&& a, Bounds&& b) noexcept {
    top = maximum(a.top, b.top);
    bottom = minimum(a.bottom, b.bottom);
    left = minimum(a.left, b.left);
    right = minimum(a.right, b.right);

    // TODO: handle more interactions between bounds
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
