// Copyright 2020 Gogov Vladislav
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./matrix_max.h"

TEST(Parallel_Matrix_Max_MPI, Size_0x0) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 0, cols = 0;
    Matrix matrix;
    if (procRank == 0)
        matrix = createRandomMatrix(rows, cols);
    ASSERT_ANY_THROW(findMaxParallel(matrix, rows, cols));
}

TEST(Parallel_Matrix_Max_MPI, Size_300x300) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 300, cols = 300;
    Matrix matrix;
    if (procRank == 0)
         matrix = createRandomMatrix(rows, cols);
    int max = findMaxParallel(matrix, rows, cols);
    if (procRank == 0) {
        int controlMax = findMaxSequential(matrix);
        ASSERT_EQ(controlMax, max);
    }
}

TEST(Parallel_Matrix_Max_MPI, Size_92x1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 92, cols = 1;
    Matrix matrix;
    if (procRank == 0)
        matrix = createRandomMatrix(rows, cols);
    int max = findMaxParallel(matrix, rows, cols);
    if (procRank == 0) {
        int controlMax = findMaxSequential(matrix);
        ASSERT_EQ(controlMax, max);
    }
}

TEST(Parallel_Matrix_Max_MPI, Size_1x35) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 1, cols = 35;
    Matrix matrix;
    if (procRank == 0)
        matrix = createRandomMatrix(rows, cols);
    int max = findMaxParallel(matrix, rows, cols);
    if (procRank == 0) {
        int controlMax = findMaxSequential(matrix);
        ASSERT_EQ(controlMax, max);
    }
}

TEST(Parallel_Matrix_Max_MPI, Size_77x58) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 77, cols = 58;
    Matrix matrix;
    if (procRank == 0)
        matrix = createRandomMatrix(rows, cols);
    int max = findMaxParallel(matrix, rows, cols);
    if (procRank == 0) {
        int controlMax = findMaxSequential(matrix);
        ASSERT_EQ(controlMax, max);
    }
}

TEST(Parallel_Matrix_Max_MPI, Size_58x77) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int rows = 58, cols = 77;
    Matrix matrix;
    if (procRank == 0)
        matrix = createRandomMatrix(rows, cols);
    int max = findMaxParallel(matrix, rows, cols);
    if (procRank == 0) {
        int controlMax = findMaxSequential(matrix);
        ASSERT_EQ(controlMax, max);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
