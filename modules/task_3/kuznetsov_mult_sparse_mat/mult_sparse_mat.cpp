// Copyright 2020 Kuznetsov Nikita
#include <mpi.h>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_3/kuznetsov_mult_sparse_mat/mult_sparse_mat.h"

std::vector<double> randMat(const int cols, const int rows) {
  if (rows <= 0 || cols <= 0) {
    throw - 1;
  }
  std::srand(std::time(nullptr));
  std::vector<double> res(cols * rows);
  for (int i = 0; i < rows * cols; i++) {
    double val_rand = static_cast<double>(std::rand() % 50 + 1);
    if (val_rand < 4) {
      res[i] = val_rand;
    } else {
      res[i] = 0;
    }
  }
  return res;
}

sparseMatrix CCS(const std::vector<double> new_mat, const int new_cols, const int new_rows) {
  if (new_cols <= 0 || new_rows <= 0) {
    throw - 1;
  }
  sparseMatrix res;
  res.cols = new_cols;
  res.rows = new_rows;
  res.not_null = 0;
  res.JA.push_back(0);
  for (int column = 0; column < new_cols; column++) {
    int not_null_count = 0;
    for (int i = column; i <= (new_rows - 1) * new_cols + column;
      i += new_cols) {
      if (new_mat[i] != 0) {
        not_null_count++;
        res.val.push_back(new_mat[i]);
        res.IA.push_back((i - column) / new_cols);
      }
    }
    res.JA.push_back(res.JA.back() + not_null_count);
    res.not_null += not_null_count;
  }
  return res;
}

const std::vector<double> operator*(const sparseMatrix& A, const sparseMatrix& B) {
  if (A.cols != B.rows) {
    throw - 1;
  }
  std::vector<double> result(A.rows * B.cols, 0);
  for (int col = 0; col < A.cols; col++) {
    for (int b_col = 0; b_col < B.cols; b_col++) {
      for (int i = A.JA[col]; i <= A.JA[col + 1] - 1; i++) {
        if (B.JA[b_col + 1] - B.JA[b_col] == 0) {
          continue;
        }
        for (int j = B.JA[b_col]; j <= B.JA[b_col + 1] - 1; j++) {
          if (B.IA[j] == col) {
            result[A.IA[i] * B.cols + b_col] += A.val[i] * B.val[j];
          }
        }
      }
    }
  }
  return result;
}

std::vector<double> matMultiply(sparseMatrix A, sparseMatrix B) {
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  if (size == 1) {
    return A * B;
  }
  MPI_Bcast(&A.cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.not_null, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.not_null, 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (A.not_null == 0 || B.not_null == 0) {
    if (rank == 0) {
      return A * B;
    } else {
      return std::vector<double>();
    }
  }
  if (A.cols != B.rows) {
    throw - 1;
  }
  if (A.cols < size) {
    if (rank == 0) {
      return A * B;
    } else {
      return std::vector<double>();
    }
  }
  if (rank != 0) {
    A.val.resize(A.not_null);
    A.IA.resize(A.not_null);
    A.JA.resize(A.cols + 1);
  }
  MPI_Bcast(&A.val[0], A.not_null, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.IA[0], A.not_null, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&A.JA[0], A.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank != 0) {
    B.val.resize(B.not_null);
    B.IA.resize(B.not_null);
    B.JA.resize(B.cols + 1);
  }
  MPI_Bcast(&B.val[0], B.not_null, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.IA[0], B.not_null, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&B.JA[0], B.cols + 1, MPI_INT, 0, MPI_COMM_WORLD);
  int delta = A.cols / size;
  int l_bound = rank * delta;
  int r_bound = (rank + 1) * delta;
  if (rank == size - 1) {
    r_bound = A.cols;
  }
  std::vector<double> local_result(A.rows * B.cols);
  for (int col = l_bound; col < r_bound; col++) {
    for (int b_col = 0; b_col < B.cols; b_col++) {
      for (int i = A.JA[col]; i <= A.JA[col + 1] - 1; i++) {
        if (B.JA[b_col + 1] - B.JA[b_col] == 0) {
          continue;
        }
        for (int j = B.JA[b_col]; j <= B.JA[b_col + 1] - 1; j++) {
          if (B.IA[j] == col) {
            local_result[A.IA[i] * B.cols + b_col] += A.val[i] * B.val[j];
          }
        }
      }
    }
  }
  std::vector<double> overal_result;
  if (rank == 0) {
    overal_result.resize(A.rows * B.cols);
  }
  if (rank == 0) {
    MPI_Reduce(&local_result[0], &overal_result[0], A.rows * B.cols, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  } else {
    MPI_Reduce(&local_result[0], MPI_IN_PLACE, A.rows * B.cols, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
  }
  return overal_result;
}
