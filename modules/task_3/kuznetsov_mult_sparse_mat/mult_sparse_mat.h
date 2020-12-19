// Copyright 2020 Kuznetsov Nikita
#ifndef MODULES_TASK_3_KUZNETSOV_MULT_SPARSE_MAT_MULT_SPARSE_MAT_H_
#define MODULES_TASK_3_KUZNETSOV_MULT_SPARSE_MAT_MULT_SPARSE_MAT_H_

#include <vector>

struct sparseMatrix {
  std::vector<double> val;
  std::vector<int> JA;
  std::vector<int> IA;
  int cols, rows, not_null;
  friend const std::vector<double> operator*(const sparseMatrix& A, const sparseMatrix& B);
};

sparseMatrix CCS(const std::vector<double> new_mat, const int new_cols, const int new_rows);
std::vector<double> randMat(const int rows, const int cols);
std::vector<double> matMultiply(sparseMatrix A, sparseMatrix B);

#endif  // MODULES_TASK_3_KUZNETSOV_MULT_SPARSE_MAT_MULT_SPARSE_MAT_H_
