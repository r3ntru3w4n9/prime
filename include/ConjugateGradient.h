// * Copyright (C) Ren-Chu Wang - All Rights Reserved
// * Unauthorized copying of this file, via any medium is strictly prohibited
// * Proprietary and confidential

#pragma once

#include "Chip.h"
#include "QuadForest.h"
#include "safe.h"

constexpr double ILLEGAL = -1.;

enum class GradType { Plain };

class Scheduler {
   public:
    explicit Scheduler(void) noexcept = delete;
    explicit Scheduler(double cur, double rat) noexcept;
    explicit Scheduler(const Scheduler& sch) noexcept = delete;
    explicit Scheduler(Scheduler&& sch) noexcept;

    Scheduler& operator=(const Scheduler& sch) noexcept = delete;
    Scheduler& operator=(Scheduler&& sch) noexcept;

    double next(void);

   private:
    double current;
    double rate;
};

class ConjGrad {
   public:
    explicit ConjGrad(void) noexcept = delete;
    explicit ConjGrad(Chip& chip,
                      QuadForest& qf,
                      GradType gt,
                      Scheduler&& sch) noexcept;

    ConjGrad& operator=(const ConjGrad& cg) noexcept;
    ConjGrad& operator=(ConjGrad&& cg) noexcept = delete;

    size_t dim(void) const;

    safe::vector<double>& positions(void);
    const safe::vector<double>& positions(void) const;

    void apply(void);

    template <bool backward>
    double cg(void) {
        if (!backward) {
            return value();
        }

        mv();

        double v = value_and_grad();

        // assert(v == value());

        update_directions();

        update_positions();

        return v;
    }

   private:
    void mv(void);
    double beta(void) const;

    void update_directions(void);
    void update_positions(void);

    double value(void) const;
    double value_and_grad(void);

    Chip& chip;
    QuadForest& qf;
    safe::vector<double> grads;
    safe::vector<double> prev_grads;
    safe::vector<double> pos;
    GradType gt;
    double best_step;
    Scheduler sch;
};
