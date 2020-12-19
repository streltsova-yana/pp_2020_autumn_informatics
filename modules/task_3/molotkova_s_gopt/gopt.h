// Copyright 2020 Molotkova Svetlana
#ifndef MODULES_TASK_3_MOLOTKOVA_S_GOPT_GOPT_H_
#define MODULES_TASK_3_MOLOTKOVA_S_GOPT_GOPT_H_
#include <functional>
#include <vector>

class StronginMethod {
  double left_border;
  double right_border;
  std::function<double(double*)> Given_Function;
  double precision;

  double Point(int t, double Lconst2, const std::vector<double>& array);
  double Interval_characteristic(int index, double Lconst1, const std::vector<double>& array);
  double Lipsh_Const1(int index, const std::vector<double>& array);
  double Lipsh_Const2(double Lconst1, double r);
  double Value(double x);
 public :
  StronginMethod(double _left_border, double _right_border,
  std::function<double(double*)> _Given_Function, double precision);
  double Find_Sequential(int count_It = 1000);
  double Find_Parallel(int count_It = 1000);
};

#endif  // MODULES_TASK_3_MOLOTKOVA_S_GOPT_GOPT_H_
