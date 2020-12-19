// Copyright 2020 Lobov Aleksandr
#ifndef MODULES_TASK_3_LOBOV_A_MONTE_CARLO_MONTE_CARLO_H_
#define MODULES_TASK_3_LOBOV_A_MONTE_CARLO_MONTE_CARLO_H_

#include <mpi.h>
#include <vector>

double getMonteCarloSeq(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

double getMonteCarloParall(double(*f)(std::vector<double>), const std::vector<double>& a,
  const std::vector<double>& b, int n);

#endif  // MODULES_TASK_3_LOBOV_A_MONTE_CARLO_MONTE_CARLO_H_
