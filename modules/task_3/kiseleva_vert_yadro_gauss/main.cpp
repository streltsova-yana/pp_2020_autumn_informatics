// Copyright 2020 Kiseleva Anastasia
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./vert_yadro_gauss.h"

#define EPS 1e-5

TEST(Parallel_random_matrix, 3004x2598) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 3004;
    int stlb = 2598;
    std::vector<double> matrix(str*stlb);
    matrix = random(str, stlb);
    matrix = transp(matrix, str, stlb);
    int tmp = str;
    str = stlb;
    stlb = tmp;
    double s1 = MPI_Wtime();
    std::vector<double> parallel_res = parallel(matrix, str, stlb, 5);
    double f1 = MPI_Wtime();
    if (rank == 0) {
        double s2 = MPI_Wtime();
        std::vector<double> posled_res = posled(matrix, 0, str, str, stlb, str*stlb, 5);
        double f2 = MPI_Wtime();
        for (int i = 0; i < stlb*str; i++) {
            ASSERT_NEAR(parallel_res[i], posled_res[i], EPS);
        }
        std::cout << "Parallel: " << f1 - s1 << std::endl;
        std::cout << "Sequence: " << f2 - s2 << std::endl;
    }
}

TEST(Parallel_random_matrix, 884x728) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 884;
    int stlb = 728;
    std::vector<double> matrix(str*stlb);
    matrix = random(str, stlb);
    matrix = transp(matrix, str, stlb);
    int tmp = str;
    str = stlb;
    stlb = tmp;
    double s1 = MPI_Wtime();
    std::vector<double> parallel_res = parallel(matrix, str, stlb, 5);
    double f1 = MPI_Wtime();
    if (rank == 0) {
        double s2 = MPI_Wtime();
        std::vector<double> posled_res = posled(matrix, 0, str, str, stlb, str*stlb, 5);
        double f2 = MPI_Wtime();
        for (int i = 0; i < stlb*str; i++) {
            ASSERT_NEAR(parallel_res[i], posled_res[i], EPS);
        }
        std::cout << "Parallel: " << f1 - s1 << std::endl;
        std::cout << "Sequence: " << f2 - s2 << std::endl;
    }
}

TEST(Parallel_random_matrix, 299x298) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 299;
    int stlb = 298;
    std::vector<double> matrix(str*stlb);
    matrix = random(str, stlb);
    matrix = transp(matrix, str, stlb);
    int tmp = str;
    str = stlb;
    stlb = tmp;
    double s1 = MPI_Wtime();
    std::vector<double> parallel_res = parallel(matrix, str, stlb, 5);
    double f1 = MPI_Wtime();
    if (rank == 0) {
        double s2 = MPI_Wtime();
        std::vector<double> posled_res = posled(matrix, 0, str, str, stlb, str*stlb, 5);
        double f2 = MPI_Wtime();
        for (int i = 0; i < stlb*str; i++) {
            ASSERT_NEAR(parallel_res[i], posled_res[i], EPS);
        }
        std::cout << "Parallel: " << f1 - s1 << std::endl;
        std::cout << "Sequence: " << f2 - s2 << std::endl;
    }
}

TEST(Parallel_random_matrix, 30x15) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 30;
    int stlb = 15;
    std::vector<double> matrix(str*stlb);
    matrix = random(str, stlb);
    matrix = transp(matrix, str, stlb);
    int tmp = str;
    str = stlb;
    stlb = tmp;
    double s1 = MPI_Wtime();
    std::vector<double> parallel_res = parallel(matrix, str, stlb, 5);
    double f1 = MPI_Wtime();
    if (rank == 0) {
        double s2 = MPI_Wtime();
        std::vector<double> posled_res = posled(matrix, 0, str, str, stlb, str*stlb, 5);
        double f2 = MPI_Wtime();
        for (int i = 0; i < stlb*str; i++) {
            ASSERT_NEAR(parallel_res[i], posled_res[i], EPS);
        }
        std::cout << "Parallel: " << f1 - s1 << std::endl;
        std::cout << "Sequence: " << f2 - s2 << std::endl;
    }
}

TEST(Parallel_random_matrix, 344x185) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int str = 344;
    int stlb = 185;
    std::vector<double> matrix(str*stlb);
    matrix = random(str, stlb);
    matrix = transp(matrix, str, stlb);
    int tmp = str;
    str = stlb;
    stlb = tmp;
    double s1 = MPI_Wtime();
    std::vector<double> parallel_res = parallel(matrix, str, stlb, 5);
    double f1 = MPI_Wtime();
    if (rank == 0) {
        double s2 = MPI_Wtime();
        std::vector<double> posled_res = posled(matrix, 0, str, str, stlb, str*stlb, 5);
        double f2 = MPI_Wtime();
        for (int i = 0; i < stlb*str; i++) {
            ASSERT_NEAR(parallel_res[i], posled_res[i], EPS);
        }
        std::cout << "Parallel: " << f1 - s1 << std::endl;
        std::cout << "Sequence: " << f2 - s2 << std::endl;
    }
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners = ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
