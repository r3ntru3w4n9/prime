/***********************************************************************

  FileName    [analytical.cpp]

  Author      [Yang Chien Yi]

  This file describes the analytical chip method.

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                           INCLUDES                               ///
////////////////////////////////////////////////////////////////////////

#include "analytical.h"

#include <assert.h>

#include <iostream>

////////////////////////////////////////////////////////////////////////
///                          PARAMETERS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                          FUNCTIONS                               ///
////////////////////////////////////////////////////////////////////////

Cost::Cost(double gamma, double Mb, Chip& chip, unsigned dimension)
    : _chip(chip),
      _dimension(chip.getNumCells() * 2),
      _gamma(gamma),
      _lambda(0),
      _lambda_step(1. / iter) {
    _f_max_val.reserve(_dimension);
    for (unsigned i = 0; i < _dimension; i++)
        _f_max_val.push_back(0);
    _f_max_wei.reserve(_dimension);
    for (unsigned i = 0; i < _dimension; i++)
        _f_max_wei.push_back(0);
    _f_min_val.reserve(_dimension);
    for (unsigned i = 0; i < _dimension; i++)
        _f_min_val.push_back(0);
    _f_min_wei.reserve(_dimension);
    for (unsigned i = 0; i < _dimension; i++)
        _f_min_wei.push_back(0);
#ifdef DENSITY_SIGMOID
    _range_x = _chip.boundryRight() - _chip.boundryLeft();
    _range_y = _chip.boundryTop() - _chip.boundryBottom();
    _bin_x = Dim_bin;
    _bin_y = Dim_bin;
    _bin_width = _range_x / _bin_x;
    _bin_height = _range_y / _bin_y;
    _bin_area = _bin_width * _bin_height;
    _Mb = Mb * _bin_area;
    _alpha_x = _bin_width / alpha_;
    _alpha_y = _bin_height / alpha_;
    _g_buf.reserve(_dimension);
    for (int i = 0; i < _dimension; i++) {
        _g_buf.push_back(0);
    }
    _bin_idx.reserve(_dimension * 2);
    for (int i = 0, n = _dimension * 2; i < n; ++i) {
        _bin_idx.push_back(0);
    }
    _bin.reserve(_bin_x * _bin_y);
    for (int i = 0, n = _bin_x * _bin_y; i < n; ++i)
        _bin.push_back(0);
#endif
#ifdef DENSITY_COULOMB
    _bin.reserve(Dim_bin * Dim_bin);
    for (int i = 0; i < (Dim_bin * Dim_bin); i++)
        _bin.push_back(0);
#endif
#ifdef DENSITY
#endif
}

void Cost::evaluateFG(const safe::vector<double>& x, double& f, safe::vector<double>& g) {
    double wirelength = 0, density = 0;
    for (int i = 0; i < _dimension; ++i)
        g[i] = 0;
    _lambda += _lambda_step;
// vector<double> x; x.reserve(_dimension); for(int i=0;i<_dimension;i++)
// x.push_back(_x[i]);
/*for(int i=0,n = _chip.numModules(); i < n;++i)//update every weighted value
    {
        unsigned x_idx = 2*i, y_idx = 2*i+1;
        double x_val = x[x_idx], y_val = x[y_idx];
        if(x[x_idx] < _chip.boundryLeft()) x[x_idx] = _chip.boundryLeft();
        else if(x[x_idx] > _chip.boundryRight()) x[x_idx] =
   _chip.boundryRight(); if(x[y_idx] < _chip.boundryBottom()) x[y_idx] =
   _chip.boundryBottom(); else if(x[y_idx] > _chip.boundryTop()) x[y_idx] =
   _chip.boundryTop();
    }*/

////////////////////////////////////////////////////////////////////////
///                         WIRELENGTH                               ///
////////////////////////////////////////////////////////////////////////
#ifdef WIRELENTH_WA
    for (unsigned i = 0; i < _dimension; i++) {
        double val = x[i];
        _f_max_wei[i] = exp(val / _gamma);
        _f_max_val[i] = _f_max_wei[i] * val;
        _f_min_wei[i] = exp(-val / _gamma);
        _f_min_val[i] = _f_min_wei[i] * val;
    }
    for (unsigned i = 0; i < _chip.getNumNets();
         i++)  // for every net
    {
        double x_max_val = 0, x_max_wei = 0, x_min_val = 0, x_min_wei = 0,
               y_max_val = 0, y_max_wei = 0, y_min_val = 0, y_min_wei = 0;
        GridNet& net = _chip.getNet(i);
        for (unsigned j = 0; j < net.getNumPin(); j++)  // for every pin, f
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            x_max_val += _f_max_val[x_idx];
            y_max_val += _f_max_val[y_idx];
            x_max_wei += _f_max_wei[x_idx];
            y_max_wei += _f_max_wei[y_idx];
            x_min_val += _f_min_val[x_idx];
            y_min_val += _f_min_val[y_idx];
            x_min_wei += _f_min_wei[x_idx];
            y_min_wei += _f_min_wei[y_idx];
        }
        double x_max = x_max_val / x_max_wei, x_min = x_min_val / x_min_wei,
               y_max = y_max_val / y_max_wei, y_min = y_min_val / y_min_wei;
        for (unsigned j = 0; j < net.getNumPin(); j++)  // for every pin, g
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            // calculate x
            // cout << g[x_idx] << " ";
            g[x_idx] += (_f_max_wei[x_idx] * (1 + (x[x_idx] - x_max) / _gamma) /
                         x_max_wei)  // max term
                        -
                        (_f_min_wei[x_idx] * (1 - (x[x_idx] - x_min) / _gamma) /
                         x_min_wei);  // min term
            // calculate y
            // cout << g[y_idx] << endl;
            g[y_idx] += (_f_max_wei[y_idx] * (1 + (x[y_idx] - y_max) / _gamma) /
                         y_max_wei)  // max term
                        -
                        (_f_min_wei[y_idx] * (1 - (x[y_idx] - y_min) / _gamma) /
                         y_min_wei);  // min term
        }
        wirelength += x_max - x_min + y_max - y_min;
    }
#endif
////////////////////////////////////////////////////////////////////////
///                           DENSITY                                ///
////////////////////////////////////////////////////////////////////////
#ifdef DENSITY_SIGMOID
    // int neg = 0, pos = 0;
    for (int i = 0, n = _bin.size(); i < n; ++i)
        _bin[i] = 0;
    for (int i = 0, n = _bin_idx.size(); i < n; ++i) {
        _bin_idx[i] = 0;
    }  // x1,x2,y1,y2
    for (int i = 0, n = _g_buf.size(); i < n; ++i) {
        _g_buf[i] = 0;
    }
    for (int i = 0, n = _chip.numModules(); i < n;
         ++i)  // for every module, f&g
    {
        Module module = _chip.module(i);
        double x_val = x[2 * i], y_val = x[2 * i + 1];
        double x_right = x_val + module.width() / 2,
               x_left = x_val - module.width() / 2,
               y_top = y_val + module.height() / 2,
               y_bottom = y_val - module.height() / 2;

        // boundary condition
        if (x_left < _chip.boundryLeft())
            continue;  //{ x_left = _chip.boundryLeft(); x_right = x_left +
                       //module.width(); }
        else if (x_right > _chip.boundryRight())
            continue;  //{  x_right = _chip.boundryRight(); x_left = x_right -
                       //module.width(); }
        if (y_bottom < _chip.boundryBottom())
            continue;  //{ y_bottom = _chip.boundryBottom(); y_top = y_bottom +
                       //module.height(); }
        else if (y_top > _chip.boundryTop())
            continue;  //{ y_top = _chip.boundryTop(); y_bottom = y_top -
                       //module.height(); }

        // find bin
        // unsigned x_idx = unsigned((x_left-_chip.boundryLeft())/_bin_width),
        // y_idx = unsigned((y_bottom-_chip.boundryBottom())/_bin_height);
        unsigned x_idx = unsigned((x_val - _chip.boundryLeft()) / _bin_width),
                 y_idx =
                     unsigned((y_val - _chip.boundryBottom()) / _bin_height);
        _bin_idx[4 * i] = x_idx;
        _bin_idx[4 * i + 2] = y_idx;
        unsigned x_idx2 = x_idx + 1, y_idx2 = y_idx + 1;
        // while(x_idx2*_bin_width+_chip.boundryLeft() < x_right) x_idx2++;
        // while(y_idx2*_bin_height+_chip.boundryBottom() < y_top) y_idx2++;
        _bin_idx[4 * i + 1] = x_idx2;
        _bin_idx[4 * i + 3] = y_idx2;

        // for every bin across the module
        vector<double>* x_buf = new vector<double>();
        vector<double>* y_buf = new vector<double>();
        for (int j = x_idx; j < x_idx2; j++) {
            double x_cent = (j + 0.5) * _bin_width + _chip.boundryLeft();
            double s_x1 = sigmoid_x(x_cent - x_left),
                   s_x2 = sigmoid_x(x_right - x_cent);
            for (int k = y_idx; k < y_idx2; k++) {
                double y_cent = (k + 0.5) * _bin_height + _chip.boundryBottom();
                double s_y1 = sigmoid_y(y_cent - y_bottom),
                       s_y2 = sigmoid_y(y_top - y_cent);
                double dens = _bin_area * s_x1 * s_x2 * s_y1 * s_y2;
                bin(j, k) += dens;
                x_buf->push_back(dens * (s_x1 - s_x2) / _alpha_x);
                y_buf->push_back(dens * (s_y1 - s_y2) / _alpha_y);
            }
        }
        _g_buf[2 * i] = x_buf;
        _g_buf[2 * i + 1] = y_buf;
        // for(int j = 0; j < x_buf->size();j++) cout << x_buf->at(j) << " ";
        // for(int j = 0; j < y_buf->size();j++) cout << y_buf->at(j) << " ";
        // cout << endl;
    }
    for (int i = 0, n = _bin.size(); i < n; ++i) {
        double dif = _bin[i] - _Mb;
        density += dif * dif;
        _bin[i] = dif;
    }
    // for(int i = 0,n = _bin.size(); i < n;++i) cout << _bin[i] << endl;
    for (int i = 0, n = _chip.numModules(); i < n; ++i)  // for every module, g
    {
        unsigned bin_num = 0;
        double g_update_x = 0, g_update_y = 0;
        for (int j = _bin_idx[4 * i], o = _bin_idx[4 * i + 1]; j < o;
             ++j)  // for every x
        {
            for (int k = _bin_idx[4 * i + 2], p = _bin_idx[4 * i + 3]; k < p;
                 ++k)  // for every y
            {
                double Db = bin(j, k);
                g_update_x += Db * _g_buf[2 * i]->at(bin_num);
                g_update_y += Db * _g_buf[2 * i + 1]->at(bin_num);
                bin_num++;
            }
        }
// cout << g[2*i] << " " << g_update_x*2/_bin_area << " " << g[2*i+1] << " " <<
// g_update_y*2/_bin_area << endl; cout << "-------------------------" << endl;
// assert(g_update_x==0&&g_update_y==0);
#ifndef WIRELENTH_WA
        assert(g[2 * i] == 0 && g[2 * i + 1] == 0);
#endif
        g[2 * i] = /*(1.-_lambda/2)*/ g[2 * i] +
                   lambda_ /*_lambda*/ * 2 * g_update_x /*_bin_area*/;
        g[2 * i + 1] = /*(1.-_lambda/2)*/ g[2 * i + 1] +
                       lambda_ /*_lambda*/ * 2 * g_update_y /*_bin_area*/;
        // if(g[2*i]<0) neg++; else pos++;
        // if(g[2*i+1]<0) neg++; else pos++;
        // cout << g[2*i] << " " << g[2*i+1] << endl;
        // if(g[2*i] > _bin_width/2) g[2*i] = _bin_width/2; else if(g[2*i] <
        // -_bin_width/2) g[2*i] = -_bin_width/2; if(g[2*i+1] > _bin_height/2)
        // g[2*i+1] = _bin_height/2; else if(g[2*i+1] < -_bin_height/2) g[2*i] =
        // -_bin_height/2;
    }
    // cout << neg << " " << pos << endl;
    for (int i = 0, n = _g_buf.size(); i < n; ++i)
        delete _g_buf[i];
#endif
#ifdef DENSITY_COULOMB
    for (int i = 0, n = _bin.size(); i < n; i++)
        _bin[i] = 0;
    for (int i = 0, n = _chip.numModules(); i < n;
         ++i)  // for every module, f&g
    {
        Module module = _chip.module(i);
        double x_val = x[2 * i], y_val = x[2 * i + 1];

        bool out_of_range = false;
        // boundary condition
        if (x_val < _chip.boundryLeft()) {
            density += _bin_width * (_chip.boundryLeft() - x_val);
            g[2 * i] -= _bin_width;
            out_of_range = true;
        }
        if (x_val > _chip.boundryRight()) {
            density += _bin_width * (x_val - _chip.boundryRight());
            g[2 * i] += _bin_width;
            out_of_range = true;
        }
        if (y_val < _chip.boundryBottom()) {
            density += _bin_height * (_chip.boundryBottom() - y_val);
            g[2 * i + 1] -= _bin_height;
            out_of_range = true;
        }
        if (y_val > _chip.boundryTop()) {
            density += _bin_height * (_chip.boundryTop() - y_val);
            g[2 * i + 1] += _bin_height;
            out_of_range = true;
        }
        if (out_of_range)
            continue;

        // find bin
        int x_idx = int((x_val - _chip.boundryLeft()) / _bin_width);
        int y_idx = int((y_val - _chip.boundryBottom()) / _bin_height);

        // set bin
        assert(x_idx < Dim_bin && y_idx < Dim_bin);
        if (bin(x_idx, y_idx) == 0)
            bin(x_idx, y_idx) = new vector<unsigned>();
        bin(x_idx, y_idx)->push_back(i);
    }
    for (int i = 0, n = _bin.size(); i < n; ++i) {
        if (_bin[i] == 0)
            continue;
        for (int j = 0, o = _bin[i]->size(); j < o; ++j) {
            unsigned m1 = _bin[i]->at(j);
            double q1 = _chip.module(m1).area();
            unsigned Ix1 = 2 * m1, Iy1 = 2 * m1 + 1;
            double x1 = x[Ix1], y1 = x[Iy1];
            for (int k = j, p = _bin[i]->size(); k < p; ++k) {
                unsigned m2 = _bin[i]->at(k);
                double q2 = _chip.module(m2).area();
                unsigned Ix2 = 2 * m2, Iy2 = 2 * m2 + 1;
                double x2 = x[Ix2], y2 = x[Iy2];
                // calculation
                double x_dif = x1 - x2, y_dif = y1 - y2;
                if (x_dif == 0)
                    x_dif = 1e-20;
                if (y_dif == 0)
                    y_dif = 1e-20;
                double r_2 = x_dif * x_dif + y_dif * y_dif;
                double r = sqrt(r_2);
                double param = q2 * q1 / r;
                density += param;
                double Fx = param * x_dif / r_2, Fy = param * y_dif / r_2;
                g[Ix1] -= _lambda * Fx;
                g[Iy1] -= _lambda * Fy;
                g[Ix2] += _lambda * Fx;
                g[Iy2] += _lambda * Fy;
            }
        }
    }
    for (int i = 0, n = _bin.size(); i < n; ++i)
        delete _bin[i];
#endif
    // assert(wirelength==0);
    f = /*(1. - _lambda/2)*/ wirelength +
        lambda_ /*_lambda*/ * density /*_bin_area*/;
    // cout << wirelength << " " << density << endl;
    // cout << "-------------------------" << endl;
    // for(int i = 0,n = _chip.numModules(); i < n; ++i)  cout << g[2*i] << " "
    // << g[2*i+1] << endl; cout /*<< (lambda_-_lambda) << " "*/ << wirelength <<
    // " " << _lambda << " " << density << " " << f << '\n';
}

void Cost::evaluateF(const safe::vector<double>& x,
                     double& f) { /*cout << "Hi" << endl;*/
    f = /*(1. - _lambda/2)*/ Wirelength(x) +
        lambda_ * /*_lambda*/ Density(x) /*_bin_area*/;
}

unsigned Cost::dimension() {
    return _dimension;
}

double Cost::Wirelength(const safe::vector<double>& x) {
    double ret = 0;
/*vector<double> x; x.reserve(_dimension); for(int i=0;i<_dimension;i++)
x.push_back(_x[i]); for(int i=0,n = _chip.numModules(); i < n;++i)//update every
weighted value
    {
        unsigned x_idx = 2*i, y_idx = 2*i+1;
        double x_val = x[x_idx], y_val = x[y_idx];
        if(x[x_idx] < _chip.boundryLeft()) x[x_idx] = _chip.boundryLeft();
        else if(x[x_idx] >= _chip.boundryRight()) x[x_idx] =
_chip.boundryRight(); if(x[y_idx] < _chip.boundryBottom()) x[y_idx] =
_chip.boundryBottom(); else if(x[y_idx] >= _chip.boundryTop()) x[y_idx] =
_chip.boundryTop();
    }*/
#ifdef WIRELENTH_WA
    for (unsigned i = 0; i < _dimension; i++) {
        double val = x[i];
        _f_max_wei[i] = exp(val / _gamma);
        _f_max_val[i] = _f_max_wei[i] * val;
        _f_min_wei[i] = exp(-val / _gamma);
        _f_min_val[i] = _f_min_wei[i] * val;
    }
    unsigned numNets = _chip.getNumNets();
    for (unsigned i = 0; i < numNets; i++)  // for every net
    {
        double x_max_val = 0, x_max_wei = 0, x_min_val = 0, x_min_wei = 0,
               y_max_val = 0, y_max_wei = 0, y_min_val = 0, y_min_wei = 0;
        GridNet& net = _chip.getNet(i);
        for (int j = 0, n = net.getNumPin(); j < n; j++)  // for every pin
        {
            const Pin& pin = _chip.getPin(net.getPinIdx(j));
            unsigned x_idx = 2 * pin.get_cell_idx();
            unsigned y_idx = 2 * pin.get_cell_idx() + 1;
            x_max_val += _f_max_val[x_idx];
            y_max_val += _f_max_val[y_idx];
            x_max_wei += _f_max_wei[x_idx];
            y_max_wei += _f_max_wei[y_idx];
            x_min_val += _f_min_val[x_idx];
            y_min_val += _f_min_val[y_idx];
            x_min_wei += _f_min_wei[x_idx];
            y_min_wei += _f_min_wei[y_idx];
        }
        ret += x_max_val / x_max_wei - x_min_val / x_min_wei +
               y_max_val / y_max_wei - y_min_val / y_min_wei;
    }
#endif
    return ret;
}

double Cost::Density(const safe::vector<double>& x) {
    double ret = 0;
#ifdef DENSITY_SIGMOID
    for (int i = 0, n = _bin.size(); i < n; ++i)
        _bin[i] = 0;
    for (int i = 0, n = _bin_idx.size(); i < n; ++i) {
        _bin_idx[i] = 0;
    }  // x1,x2,y1,y2
    for (int i = 0, n = _chip.numModules(); i < n;
         ++i)  // for every module, f&g
    {
        Module module = _chip.module(i);
        double x_val = x[2 * i], y_val = x[2 * i + 1];
        double x_right = x_val + module.width() / 2,
               x_left = x_val - module.width() / 2,
               y_top = y_val + module.height() / 2,
               y_bottom = y_val - module.height() / 2;

        // boundary condition
        if (x_left < _chip.boundryLeft())
            continue;  //{ x_left = _chip.boundryLeft(); x_right = x_left +
                       //module.width(); }
        else if (x_right > _chip.boundryRight())
            continue;  //{  x_right = _chip.boundryRight(); x_left = x_right -
                       //module.width(); }
        if (y_bottom < _chip.boundryBottom())
            continue;  //{ y_bottom = _chip.boundryBottom(); y_top = y_bottom +
                       //module.height(); }
        else if (y_top > _chip.boundryTop())
            continue;  //{ y_top = _chip.boundryTop(); y_bottom = y_top -
                       //module.height(); }

        // find bin
        // unsigned x_idx = unsigned((x_left-_chip.boundryLeft())/_bin_width),
        // y_idx = unsigned((y_bottom-_chip.boundryBottom())/_bin_height);
        unsigned x_idx = unsigned((x_val - _chip.boundryLeft()) / _bin_width),
                 y_idx =
                     unsigned((y_val - _chip.boundryBottom()) / _bin_height);
        _bin_idx[4 * i] = x_idx;
        _bin_idx[4 * i + 2] = y_idx;
        unsigned x_idx2 = x_idx + 1, y_idx2 = y_idx + 1;
        // while(x_idx2*_bin_width+_chip.boundryLeft() < x_right) x_idx2++;
        // while(y_idx2*_bin_height+_chip.boundryBottom() < y_top) y_idx2++;
        _bin_idx[4 * i + 1] = x_idx2;
        _bin_idx[4 * i + 3] = y_idx2;

        // for every bin across the module
        for (int j = x_idx; j < x_idx2; j++) {
            double x_cent = (j + 0.5) * _bin_width + _chip.boundryLeft();
            double s_x1 = sigmoid_x(x_cent - x_left),
                   s_x2 = sigmoid_x(x_right - x_cent);
            for (int k = y_idx; k < y_idx2; k++) {
                double y_cent = (k + 0.5) * _bin_height + _chip.boundryBottom();
                double s_y1 = sigmoid_y(y_cent - y_bottom),
                       s_y2 = sigmoid_y(y_top - y_cent);
                double dens = _bin_area * s_x1 * s_x2 * s_y1 * s_y2;
                bin(j, k) += dens;
            }
        }
    }
    for (int i = 0, n = _bin.size(); i < n; ++i) {
        double dif = _bin[i] - _Mb;
        ret += dif * dif;
    }
#endif
#ifdef DENSITY_COULOMB
    for (int i = 0, n = _bin.size(); i < n; i++)
        _bin[i] = 0;
    for (int i = 0, n = _chip.numModules(); i < n;
         ++i)  // for every module, f&g
    {
        Module module = _chip.module(i);
        double x_val = x[2 * i], y_val = x[2 * i + 1];

        bool out_of_range = false;
        // boundary condition
        if (x_val < _chip.boundryLeft()) {
            ret += _bin_width * (_chip.boundryLeft() - x_val);
            out_of_range = true;
        }
        if (x_val > _chip.boundryRight()) {
            ret += _bin_width * (x_val - _chip.boundryRight());
            out_of_range = true;
        }
        if (y_val < _chip.boundryBottom()) {
            ret += _bin_height * (_chip.boundryBottom() - y_val);
            out_of_range = true;
        }
        if (y_val > _chip.boundryTop()) {
            ret += _bin_height * (_chip.boundryTop() - y_val);
            out_of_range = true;
        }
        if (out_of_range)
            continue;

        // find bin
        int x_idx = int((x_val - _chip.boundryLeft()) / _bin_width);
        int y_idx = int((y_val - _chip.boundryBottom()) / _bin_height);

        // set bin
        assert(x_idx < Dim_bin && y_idx < Dim_bin);
        if (bin(x_idx, y_idx) == 0)
            bin(x_idx, y_idx) = new vector<unsigned>();
        bin(x_idx, y_idx)->push_back(i);
    }
    for (int i = 0, n = _bin.size(); i < n; ++i) {
        if (_bin[i] == 0)
            continue;
        for (int j = 0, o = _bin[i]->size(); j < o; ++j) {
            unsigned m1 = _bin[i]->at(j);
            double q1 = _chip.module(m1).area();
            unsigned Ix1 = 2 * m1, Iy1 = 2 * m1 + 1;
            double x1 = x[Ix1], y1 = x[Iy1];
            for (int k = j, p = _bin[i]->size(); k < p; ++k) {
                unsigned m2 = _bin[i]->at(k);
                double q2 = _chip.module(m2).area();
                unsigned Ix2 = 2 * m2, Iy2 = 2 * m2 + 1;
                double x2 = x[Ix2], y2 = x[Iy2];
                // calculation
                double x_dif = x1 - x2, y_dif = y1 - y2;
                if (x_dif == 0)
                    x_dif = 1e-20;
                if (y_dif == 0)
                    y_dif = 1e-20;
                double r_2 = x_dif * x_dif + y_dif * y_dif;
                double r = sqrt(r_2);
                ret += q2 * q1 / r;
            }
        }
    }
    for (int i = 0, n = _bin.size(); i < n; ++i)
        delete _bin[i];
#endif
    // cout << ret << endl;
    return ret;
}

#ifdef DENSITY_SIGMOID
double Cost::sigmoid_x(const double t) {
    return 1 / (1 + exp(-t / _alpha_x));
}
double Cost::sigmoid_y(const double t) {
    return 1 / (1 + exp(-t / _alpha_y));
}
double Cost::sigmoid_x1(const double x_left, const unsigned x_idx) {
    return sigmoid_x((x_idx + 0.5) * _bin_width + _chip.boundryLeft() - x_left);
}
double Cost::sigmoid_x2(const double x_right, const unsigned x_idx) {
    return sigmoid_x(x_right - (x_idx + 0.5) * _bin_width -
                     _chip.boundryLeft());
}
double Cost::sigmoid_y1(const double y_bottom, const unsigned y_idx) {
    return sigmoid_y((y_idx + 0.5) * _bin_height + _chip.boundryBottom() -
                     y_bottom);
}
double Cost::sigmoid_y2(const double y_top, const unsigned y_idx) {
    return sigmoid_y(y_top - (y_idx + 0.5) * _bin_height -
                     _chip.boundryBottom());
}
#endif