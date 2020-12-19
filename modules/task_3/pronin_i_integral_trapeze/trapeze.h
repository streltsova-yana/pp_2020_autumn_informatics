// Copyright 2020 Pronin Igor
#ifndef MODULES_TASK_3_PRONIN_I_INTEGRAL_TRAPEZE_TRAPEZE_H_
#define MODULES_TASK_3_PRONIN_I_INTEGRAL_TRAPEZE_TRAPEZE_H_
#include <mpi.h>
#include <vector>
double SequentialOperations(double(*function)(std::vector<double>), std::vector<double>, std::vector<double>, int);
double ParllelOperations(double(*function)(std::vector<double>), std::vector<double>, std::vector<double>, int);
#endif  // MODULES_TASK_3_PRONIN_I_INTEGRAL_TRAPEZE_TRAPEZE_H_
