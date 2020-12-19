// Copyright 2020 Bulychev Vladislav
#ifndef MODULES_TASK_3_BULYCHEV_V_CALCULATION_OF_INTEGRALS_CALCULATION_H_
#define MODULES_TASK_3_BULYCHEV_V_CALCULATION_OF_INTEGRALS_CALCULATION_H_

#include <vector>

double SequentialCalculation(std::vector<double> a, std::vector<double> b,
    int n, double(*f)(std::vector<double>));

double ParallelCalculation(std::vector<double> a, std::vector<double> b,
    int n, double (*func)(std::vector<double>));

#endif  // MODULES_TASK_3_BULYCHEV_V_CALCULATION_OF_INTEGRALS_CALCULATION_H_
