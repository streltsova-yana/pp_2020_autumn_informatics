// Copyright 2020 Zhuravlev Roman
#include <mpi.h>
#include <iostream>
#include <numeric>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/zhuravl_r_simpson/Simpson.h"

double Sequential_SimpsonForDouble(double (*f)(double x, double y), double a1, double b1,
  double a2, double b2, int n1, int n2) {
  if (a1 > b1 || a2 > b2 || n1 < 1 || n2 < 1) throw "Invalid argument";

  double step1 = (b1 - a1) / (n1);
  double step2 = (b2 - a2) / (n2);
  double s1 = 0, s4 = 0, s16 = 0;
  double res;

  for (int i = 0; i < n1; i += 2) {
    for (int j = 0; j < n2; j += 2) {
      s1 += f(a1 + i * step1, a2 + j * step2) + f(a1 + (i + 2) * step1, a2 + (j + 2) * step2) +
        f(a1 + (i + 2) * step1, a2 + j * step2) + f(a1 + i * step1, a2 + (j + 2) * step2);

      s4 += f(a1 + (i + 1) * step1, a2 + j * step2) + f(a1 + (i + 2) * step1, a2 + (j + 1) * step2) +
        f(a1 + (i + 1) * step1, a2 + (j + 2) * step2) + f(a1 + i * step1, a2 + (j + 1) * step2);

      s16 += f(a1 + (i + 1) * step1, a2 + (j + 1) * step2);
    }
  }
  res = s1 + 4 * s4 + 16 * s16;
  return res * (step1 * step2 / 9);
}

double Parallel_SimpsonForDouble(double (*f)(double x, double y), double a1, double b1,
  double a2, double b2, int n1, int n2) {
  int rank, num;
  double local_res = 0;
  double step1 = (b1 - a1) / (n1);
  double step2 = (b2 - a2) / (n2);
  double s1 = 0, s4 = 0, s16 = 0;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num);

  int delta = n1 / num;
  int start = rank * delta;
  int finish = start + delta;

  if (rank == num - 1) {
    finish += n1 % num;
  }
  if (start % 2 != 0) {
    start++;
  }

  for (int i = start; i < finish; i += 2) {
    for (int j = 0; j < n2; j += 2) {
      s1 += f(a1 + i * step1, a2 + j * step2) + f(a1 + (i + 2) * step1, a2 + (j + 2) * step2) +
        f(a1 + (i + 2) * step1, a2 + j * step2) + f(a1 + i * step1, a2 + (j + 2) * step2);

      s4 += f(a1 + (i + 1) * step1, a2 + j * step2) + f(a1 + (i + 2) * step1, a2 + (j + 1) * step2) +
        f(a1 + (i + 1) * step1, a2 + (j + 2) * step2) + f(a1 + i * step1, a2 + (j + 1) * step2);

      s16 += f(a1 + (i + 1) * step1, a2 + (j + 1) * step2);
    }
  }

  local_res = s1 + 4 * s4 + 16 * s16;
  std::vector<double> buf(num);

  double res = 0;
  MPI_Gather(&local_res, 1, MPI_DOUBLE, &buf[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (int i = 0; i < num; i++) {
      res += buf[i];
    }
  }

  return res * (step1 * step2 / 9);
}

double func(double x, double y) {
  return 2 * x * y / 5;
}
