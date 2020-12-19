// Copyright 2020 Kuznetsov Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include "./mult_sparse_mat.h"
#define SIZE 50

TEST(MULTIPLY_SPARSE_MATRIX, THROWS_WHEN_SIZE_IS_NEGATIVE) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(randMat(-10, -10));
  }
}

TEST(MULTIPLY_SPARSE_MATRIX, THROWS_WHEN_SIZE_IS_ZERO) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    ASSERT_ANY_THROW(randMat(0, 0));
  }
}

TEST(MULTIPLY_SPARSE_MATRIX, SEQ_METHOD_EQUAL_EXP_RES) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if (rank == 0) {
    sparseMatrix A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    sparseMatrix B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);
    std::vector<double> exp_res = { 4, 0, 0, 0, 12, 0, 0, 0 };
    std::vector<double> res = A * B;
    ASSERT_EQ(res, exp_res);
  }
}

TEST(MULTIPLY_SPARSE_MATRIX, PARALLEL_METHOD_EQUAL_EXP_RES) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sparseMatrix A, B;
  if (rank == 0) {
    A = CCS(std::vector<double>{0, 0, 1, 0, 2, 0, 0, 0, 3, 0, 0, 0}, 3, 4);
    B = CCS(std::vector<double>{3, 0, 0, 0, 4, 0}, 2, 3);
  }
  std::vector<double> res = matMultiply(A, B);
  if (rank == 0) {
    std::vector<double> exp_res = { 4, 0, 0, 0, 12, 0, 0, 0 };
    ASSERT_EQ(res, exp_res);
  }
}

TEST(MULTIPLY_SPARSE_MATRIX, SEQ_EQUAL_PARALLEL_METHOD) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  sparseMatrix A, B;
  if (rank == 0) {
    A = CCS(randMat(SIZE, SIZE), SIZE, SIZE);
    B = CCS(randMat(SIZE, SIZE), SIZE, SIZE);
  }
  double start2 = MPI_Wtime();
  std::vector<double> par_res = matMultiply(A, B);
  double finish2 = MPI_Wtime();
  if (rank == 0) {
    double start1 = MPI_Wtime();
    std::vector<double> seq_res = A * B;
    double finish1 = MPI_Wtime();
    ASSERT_EQ(seq_res, par_res);
    std::cout << "SEQ TIME = " << finish1 - start1 << std::endl << "PARALLEL TIME = " << finish2 - start2 << std::endl;
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
