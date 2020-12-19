// Copyright 2020 Kiseleva Anastasia
#ifndef MODULES_TASK_3_KISELEVA_VERT_YADRO_GAUSS_VERT_YADRO_GAUSS_H_
#define MODULES_TASK_3_KISELEVA_VERT_YADRO_GAUSS_VERT_YADRO_GAUSS_H_

#include <vector>
#include <random>
#include <ctime>

int check(int v, int max, int min);
std::vector<double> random(int str, int stlb);
std::vector<double> transp(const std::vector<double>& image, int str, int stlb);
std::vector<double> yadro(int sigma);
std::vector<double> posled(const std::vector<double>& image, int xx, int xmax, int str, int stlb, int size_, int sigma);
std::vector<double> parallel(const std::vector<double>& image, int str, int stlb, int sigma);

#endif  // MODULES_TASK_3_KISELEVA_VERT_YADRO_GAUSS_VERT_YADRO_GAUSS_H_
