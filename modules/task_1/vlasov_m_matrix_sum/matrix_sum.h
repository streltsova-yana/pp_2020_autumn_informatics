// Copyright 2020 Vlasov Maksim
#ifndef MODULES_TASK_1_VLASOV_M_MATRIX_SUM_MATRIX_SUM_H_
#define MODULES_TASK_1_VLASOV_M_MATRIX_SUM_MATRIX_SUM_H_

#include <vector>

std::vector<int> createRandomVector(int elements_count);
int calculateSumSequental(const std::vector<int> &vector);
int calculateSumParallel(const std::vector<int> &vector, int elements_count);

#endif  // MODULES_TASK_1_VLASOV_M_MATRIX_SUM_MATRIX_SUM_H_
