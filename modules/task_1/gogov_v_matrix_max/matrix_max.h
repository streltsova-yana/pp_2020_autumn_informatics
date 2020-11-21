// Copyright 2020 Gogov Vladislav
#ifndef MODULES_TASK_1_GOGOV_V_MATRIX_MAX_MATRIX_MAX_H_
#define MODULES_TASK_1_GOGOV_V_MATRIX_MAX_MATRIX_MAX_H_
#include <vector>

using Matrix = std::vector<int>;

Matrix createRandomMatrix(int rows, int cols);
int findMaxSequential(const Matrix& vec);
int findMaxParallel(const Matrix& matirx, int rows, int cols);

#endif  // MODULES_TASK_1_GOGOV_V_MATRIX_MAX_MATRIX_MAX_H_
