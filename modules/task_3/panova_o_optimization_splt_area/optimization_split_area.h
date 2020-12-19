// Copyright 2020 Panova Olga
#ifndef MODULES_TASK_3_PANOVA_O_OPTIMIZATION_SPLT_AREA_OPTIMIZATION_SPLIT_AREA_H_
#define MODULES_TASK_3_PANOVA_O_OPTIMIZATION_SPLT_AREA_OPTIMIZATION_SPLIT_AREA_H_
#include <functional>
#include <vector>
double GetMForLipschitz(int num, const std::vector<double>& vec, std::function<double(double*)> my_function);
double GetmForLipschitz(double M, double reliability);
double GetProbability(double m, int num, const std::vector<double>& vec, std::function<double(double*)> my_function);
double GetValue(double x, std::function<double(double*)> my_function);
double SequentialOptimization(double start, double end, std::function<double(double*)> my_function, double eps);
double ParallelOptimization(double start, double end, std::function<double(double*)> my_function, double eps);
#endif  // MODULES_TASK_3_PANOVA_O_OPTIMIZATION_SPLT_AREA_OPTIMIZATION_SPLIT_AREA_H_
