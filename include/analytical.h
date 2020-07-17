/***********************************************************************

  FileName    [Chip.h]

  Author      [Yang Chien Yi, Ren-Chu Wang]

  This file difines the data manager for prime.

***********************************************************************/

#pragma once

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include <assert.h>

#include <cmath>

#include "Chip.h"

//#include "QuadTree.h"
// #include "TreeNet.h"

////////////////////////////////////////////////////////////////////////
///                          DESCRIPTION                             ///
////////////////////////////////////////////////////////////////////////

/*
 */

////////////////////////////////////////////////////////////////////////
///                           CLASSES                                ///
////////////////////////////////////////////////////////////////////////
#define iter 100
//#define StepSize    1000
//#define gamma       300
#define lambda_ 3.
#define Dim_bin 30
#define WIRELENTH_WA
//#define DENSITY
//#define DENSITY_SIGMOID
#define SS 1.
//#define DENSITY_COULOMB
//#define RANDOM_INIT
//#define ORIGINAL

#ifdef DENSITY_SIGMOID
#define alpha_ 1.
#endif

class Cost {
   public:
    Cost(double, double, Chip&, unsigned);
    ~Cost() {}

    void evaluateFG(const safe::vector<double>& x, double& f, safe::vector<double>& g);
    void evaluateF(const safe::vector<double>& x, double& f);
    unsigned dimension();

   private:
    Chip& _chip;
    unsigned _dimension;
    double _gamma;
    double _Mb;
    double _lambda;
    double _lambda_step;
    double _bin_width;
    double _bin_height;
    double _bin_area;
    safe::vector<double> _f_max_val, _f_min_val, _f_max_wei,
        _f_min_wei;  // buffer for calculating f; odd for y, even for x
#ifdef DENSITY_SIGMOID
    double _range_x, _range_y;
    double _alpha_x, _alpha_y;
    unsigned _bin_x, _bin_y;
    safe::vector<double> _bin;
    safe::vector<unsigned> _bin_idx;
    safe::vector<safe::vector<double>> _g_buf;
#endif
#ifdef DENSITY_COULOMB
    vector<vector<unsigned>*> _bin;
#endif
    // private function
    double Wirelength(const safe::vector<double>& x);
    double Density(const safe::vector<double>& x);
#ifdef DENSITY_SIGMOID
    // double& Bin(unsigned i, unsigned x, unsigned y) { unsigned idx =
    // x*Dim_bin+y; assert(idx<Dim_bin*Dim_bin); _bin_idx[i].push_back(idx);
    // return _bin[idx]; }
    double sigmoid_x(const double t);
    double sigmoid_y(const double t);
    double sigmoid_x1(const double x_left, const unsigned x_idx);
    double sigmoid_x2(const double x_right, const unsigned x_idx);
    double sigmoid_y1(const double y_bottom, const unsigned y_idx);
    double sigmoid_y2(const double y_top, const unsigned y_idx);
#endif
#ifdef DENSITY_COULOMB
// vector* Bin(unsigned x, unsigned y) { unsigned idx = x*Dim_bin+y;
// assert(idx<Dim_bin*Dim_bin); return _bin[idx]; }
#endif
};