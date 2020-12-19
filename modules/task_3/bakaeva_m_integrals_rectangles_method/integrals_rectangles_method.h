// Copyright 2020 Bakaeva Maria
#ifndef MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_
#define MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_
#include <vector>
#include <algorithm>
#include <utility>

using std::vector;
using std::pair;

double getSequentialIntegrals(const int n, vector<pair<double, double> > a_b, double (*F)(vector<double>));
double getParallelIntegrals(const int n, vector<pair<double, double> > a_b, double (*F)(vector<double>));
#endif  // MODULES_TASK_3_BAKAEVA_M_INTEGRALS_RECTANGLES_METHOD_INTEGRALS_RECTANGLES_METHOD_H_
