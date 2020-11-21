// Copyright 2020 Zoreev Mikhail
#ifndef MODULES_TASK_1_ZOREEV_M_RECTANGLE_METHOD_RECTANGLE_METHOD_H_
#define MODULES_TASK_1_ZOREEV_M_RECTANGLE_METHOD_RECTANGLE_METHOD_H_

#include <mpi.h>

#include <cmath>
#include <functional>
#include <stdexcept>

double integralSeqential(std::function<double(double)> integrable_function, double a, double b, size_t count);

double integralParallel(std::function<double(double)> integrable_function, double a, double b, size_t count);

#endif  // MODULES_TASK_1_ZOREEV_M_RECTANGLE_METHOD_RECTANGLE_METHOD_H_
