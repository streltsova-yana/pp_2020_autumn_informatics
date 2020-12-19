// Copyright 2020 Molotkova Svetlana
#include <mpi.h>
#include <utility>
#include <list>
#include <algorithm>
#include <vector>
#include <iostream>
#include <cmath>
#include "../../../modules/task_3/molotkova_s_gopt/gopt.h"

StronginMethod::StronginMethod(double _left_border, double _right_border,
  std::function<double(double*)> _Given_Function, double _precision) {
  left_border = _left_border;
  right_border = _right_border;
  Given_Function = _Given_Function;
  precision = _precision;
}

double StronginMethod::Value(double x) {
  double* arg = &x;
  return Given_Function(arg);
}

double Sign(double x) {
  if (x >= 0)
    return x;
  else
    return -x;
}

double StronginMethod::Lipsh_Const1(int index, const std::vector<double>& array) {
  return Sign(Value(array[index]) - Value(array[index - 1])) / (array[index] - array[index - 1]);
}

double StronginMethod::Lipsh_Const2(double Lconst1, double r) {
  if (Lconst1 == 0)
    return 1;
  else
    return r * Lconst1;
}

double StronginMethod::Interval_characteristic(int index, double Lconst1, const std::vector<double>& array) {
  double x_diff = array[index] - array[index - 1];
  if (index == 1) {
    return 2 * x_diff - 4 * Value(array[index]) / Lconst1;
  } else {
    double y_diff_in_pow = pow((Value(array[index]) - Value(array[index - 1])), 2);
    double interval_characteristic = x_diff + y_diff_in_pow / (Lconst1 * Lconst1 * x_diff) -
    2 * (Value(array[index]) + Value(array[index - 1])) / Lconst1;
    return  interval_characteristic;
  }
}

double StronginMethod::Point(int index, double Lconst2, const std::vector<double>& array) {
  double arg = (Value(array[index]) - Value(array[index - 1])) / (2 * Lconst2);
  double point = (array[index] + array[index - 1]) / 2 - arg;
  return point;
}

double StronginMethod::Find_Sequential(int count_It) {
  std::vector<double> x(count_It + 1, -1e+300);
  int nIteration = 1;
  int t = 1;
  double r = 2;
  double R, tmp;
  x[0] = left_border;
  x[1] = right_border;
  double Lconst1 = Lipsh_Const1(1, x);
  double Lconst2 = Lipsh_Const2(Lconst1, r);
  x[2] = Point(1, Lconst2, x);
  ++nIteration;
  while (nIteration < count_It) {
    sort(x.begin(), x.begin() + nIteration + 1);
    Lconst1 = Lipsh_Const1(1, x);
    for (int i = 2; i <= nIteration; ++i) {
      Lconst1 = std::max(Lconst1, Lipsh_Const1(i, x));
    }
    Lconst2 = Lipsh_Const2(Lconst1, r);
    R = Interval_characteristic(1, Lconst1, x);
    t = 1;
    for (int i = 2; i < nIteration; ++i) {
      tmp = Interval_characteristic(i, Lconst1, x);
      if (R < tmp) {
        R = tmp;
        t = i;
      }
    }
    tmp = 2 * (x[nIteration] - x[nIteration - 1]) - 4 * Value(x[nIteration - 1]) / Lconst1;
    if (R < tmp) {
      R = tmp;
      t = nIteration;
    }
    x[nIteration + 1] = Point(t, Lconst2, x);
    ++nIteration;
    if (x[t] - x[t - 1] < precision) {
      break;
    }
  }
  return x[t];
}
  double StronginMethod::Find_Parallel(int count_It) {
  int procNum;
  int porog = 64;
  MPI_Comm_size(MPI_COMM_WORLD, &procNum);
  if (procNum == 1) {
    return Find_Sequential(count_It);
  }
  int procRank;
  MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
  double localRes;
  std::vector<double> result(procNum);
  std::vector<double> x(porog + 1, -1e+300);
  int nIteration = 1;
  int t = 1;
  double r = 2;
  double R, tmp;

  x[0] = left_border;
  x[1] = right_border;
  double M = Lipsh_Const1(1, x);
  double m = Lipsh_Const2(M, r);
  x[2] = Point(1, m, x);
  ++nIteration;

  while (nIteration < porog) {
    sort(x.begin(), x.begin() + nIteration + 1);

    M = Lipsh_Const1(1, x);
    for (int i = 2; i <= nIteration; ++i) {
      M = std::max(M, Lipsh_Const1(i, x));
    }
    m = Lipsh_Const2(M, r);
    R = Interval_characteristic(1, M, x);
    t = 1;

    for (int i = 2; i < nIteration; ++i) {
      tmp = Interval_characteristic(i, M, x);
      if (R < tmp) {
        R = tmp;
        t = i;
      }
    }
    tmp = 2 * (x[nIteration] - x[nIteration - 1]) - 4 * Value(x[nIteration - 1]) / M;
    if (R < tmp) {
      R = tmp;
      t = nIteration;
    }

    x[nIteration + 1] = Point(t, m, x);
    ++nIteration;
    if (x[t] - x[t - 1] < precision) {
      break;
    }
  }
  sort(x.begin(), x.begin() + nIteration + 1);
  double h = porog / procNum;
  double locA, locB;
  if (procRank != procNum - 1) {
    locA = x[procRank * h];
    locB = x[(procRank + 1) * h];
  } else {
    locA = x[procRank * h];
    locB = x[porog - 1];
  }
  StronginMethod opt(locA, locB, Given_Function, precision);
  localRes = opt.Find_Sequential(count_It);
  MPI_Gather(&localRes, 1, MPI_DOUBLE, &result[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  if (procRank == 0) {
    for (int i = 1; i < procNum; ++i) {
      if (Value(result[i]) < Value(result[0])) {
        std::swap(result[i], result[0]);
      }
    }
  }
  return result[0];
}
