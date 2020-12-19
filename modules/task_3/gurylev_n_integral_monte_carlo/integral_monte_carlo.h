// Copyright 2020 Gurylev Nikita
#ifndef MODULES_TASK_3_GURYLEV_N_INTEGRAL_MONTE_CARLO_INTEGRAL_MONTE_CARLO_H_
#define MODULES_TASK_3_GURYLEV_N_INTEGRAL_MONTE_CARLO_INTEGRAL_MONTE_CARLO_H_

#include <mpi.h>
#include <vector>
#include <functional>

double getSequentialIntegralMCarlo(double(*function)(std::vector<double>), const std::vector<double>& llim,
    const std::vector<double>& ulim, int n);

double getParallelIntegralMCarlo(double(*function)(std::vector<double>), const std::vector<double>& llim,
    const std::vector<double>& ulim, int n);

#endif  // MODULES_TASK_3_GURYLEV_N_INTEGRAL_MONTE_CARLO_INTEGRAL_MONTE_CARLO_H_
