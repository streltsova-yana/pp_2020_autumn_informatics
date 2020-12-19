// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include "./sobel.h"

TEST(Test_Sobel, Matrix_4x5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 4;
    int cols = 5;
    std::vector<int> mat;
    if (rank == 0)
        mat = getRandomMatrix(rows, cols);
    std::vector<int> sob1 = getParalSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sob2 = getSequentialSobel(mat, rows, cols);
        ASSERT_EQ(sob1, sob2);
    }
}

TEST(Test_Sobel, Matrix_21x12) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 21;
    int cols = 12;
    std::vector<int> mat;
    if (rank == 0)
        mat = getRandomMatrix(rows, cols);
    std::vector<int> sob1 = getParalSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sob2 = getSequentialSobel(mat, rows, cols);
        ASSERT_EQ(sob1, sob2);
    }
}

TEST(Test_Sobel, Matrix_100x100) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 100;
    int cols = 100;
    std::vector<int> mat;
    if (rank == 0)
        mat = getRandomMatrix(rows, cols);
    std::vector<int> sob1 = getParalSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sob2 = getSequentialSobel(mat, rows, cols);
        ASSERT_EQ(sob1, sob2);
    }
}

TEST(Test_Sobel, Matrix_2x2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 2;
    int cols = 2;
    std::vector<int> mat;
    if (rank == 0)
        mat = getRandomMatrix(rows, cols);
    std::vector<int> sob1 = getParalSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sob2 = getSequentialSobel(mat, rows, cols);
        ASSERT_EQ(sob1, sob2);
    }
}

TEST(Test_Sobel, Matrix_3x3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int rows = 3;
    int cols = 3;
    std::vector<int> mat;
    if (rank == 0)
        mat = getRandomMatrix(rows, cols);
    std::vector<int> sob1 = getParalSobel(mat, rows, cols);
    if (rank == 0) {
        std::vector<int> sob2 = getSequentialSobel(mat, rows, cols);
        ASSERT_EQ(sob1, sob2);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
