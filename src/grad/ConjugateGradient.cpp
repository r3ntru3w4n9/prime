// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#include "ConjugateGradient.h"

#include <assert.h>

#include <algorithm>
#include <memory>

// FIXME: give me a better mechanism

ConjugateGradient::ConjugateGradient(void) noexcept
    : grads(safe::vector<Gradient>()),
      prev_grads(safe::vector<Gradient>()),
      qf(nullptr) {}

ConjugateGradient::ConjugateGradient(size_t size) noexcept
    : grads(safe::vector<Gradient>(size)),
      prev_grads(safe::vector<Gradient>(size)),
      qf(nullptr) {}

ConjugateGradient::ConjugateGradient(safe::vector<Gradient>&& g) noexcept
    : grads(std::move(g)), prev_grads(safe::vector<Gradient>()), qf(nullptr) {}

ConjugateGradient::ConjugateGradient(safe::vector<Gradient>&& g,
                                     safe::vector<Gradient>&& pg) noexcept
    : grads(std::move(g)), prev_grads(std::move(pg)) {}

ConjugateGradient::ConjugateGradient(QuadForest& qf) noexcept
    : grads(safe::vector<Gradient>(qf.size())),
      prev_grads(safe::vector<Gradient>(qf.size())),
      qf(&qf) {
    assert(grads.size() == qf.size());
    assert(prev_grads.size() == qf.size());
}

size_t ConjugateGradient::size(void) const {
    assert(prev_grads.size() == grads.size());
    return grads.size();
}

void ConjugateGradient::clear(void) {
    prev_grads.clear();
    grads.clear();
}

void ConjugateGradient::zero_grad_(void) {
    std::fill(grads.begin(), grads.end(), Gradient(0., 0.));
    std::fill(prev_grads.begin(), prev_grads.end(), Gradient(0., 0.));
}

double ConjugateGradient::eval(void) {
    unsigned sum_hpwl = 0;
    for (unsigned idx = 0; idx < qf->size(); ++idx) {
        const QuadTree qt = qf->get_tree(idx);
        unsigned hpwl = qt.get_hpwl();
        sum_hpwl += hpwl;
    }
    return (double)sum_hpwl;
}

void ConjugateGradient::backward(void) {}

double ConjugateGradient::update_directions(safe::vector<Gradient>&) {
    return 0.;
}
