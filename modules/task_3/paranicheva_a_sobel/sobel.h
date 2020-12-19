// Copyright 2020 Paranicheva Alyona
#ifndef MODULES_TASK_3_PARANICHEVA_A_SOBEL_SOBEL_H_
#define MODULES_TASK_3_PARANICHEVA_A_SOBEL_SOBEL_H_

#include <vector>

std::vector<int> getRandomMatrix(int rows, int cols);
int check(int tmp, int min, int max);
int SobelXY(std::vector<int> mat, int cols, int i, int j);
std::vector<int> getSequentialSobel(std::vector<int> mat, int rows, int cols);
std::vector<int> getParalSobel(std::vector<int> mat, int rows, int cols);

#endif  // MODULES_TASK_3_PARANICHEVA_A_SOBEL_SOBEL_H_
