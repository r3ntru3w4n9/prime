// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Gradient.h"
#include "QuadForest.h"
#include "safe.h"

class ConjugateGradient {
   public:
    explicit ConjugateGradient(void) noexcept;
    explicit ConjugateGradient(size_t size) noexcept;
    explicit ConjugateGradient(safe::vector<Gradient>&& grad) noexcept;
    explicit ConjugateGradient(safe::vector<Gradient>&& grad,
                               safe::vector<Gradient>&& prev_grads) noexcept;
    explicit ConjugateGradient(QuadForest& qf) noexcept;

    size_t size(void) const;
    void zero_grad_(void);

    // TODO: currently supports one kind of gradient calculations
    double eval(void);
    void backward(void);
    double update_directions(safe::vector<Gradient>& gradients);

   private:
    void clear(void);

    // FIXME: not ideal to have made CG a reference type
    safe::vector<Gradient> grads;
    safe::vector<Gradient> prev_grads;
    QuadForest* qf;
};
