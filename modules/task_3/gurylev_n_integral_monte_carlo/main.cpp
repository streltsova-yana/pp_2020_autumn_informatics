// Copyright 2020 Gurylev Nikita
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <iostream>
#include <ctime>
#include <cmath>
#include "./integral_monte_carlo.h"

double pol_f(std::vector<double> args) {
    return args[0] + args[0] * args[0] + args[1] * args[1];
}

double comp_f(std::vector<double> args) {
    return cos(args[0]) * args[1] + args[1] - args[2];
}

double lin_f(std::vector<double> args) {
    return args[0] + args[1] + args[2];
}

TEST(Parallel_Operations_MPI, test1_pol_operation) {
    int rank;
    int n = 1000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim = { 1.25, 2.0 }, ulim = { 1.8, 3.0 };;
    double result = getParallelIntegralMCarlo(pol_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result_1 = getSequentialIntegralMCarlo(pol_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result_1 << std::endl;
        ASSERT_NEAR(result, result_1, 0.5);
    }
}

TEST(Parallel_Operations_MPI, test2_pol_operation) {
    int rank;
    int n = 10000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim = { 1.25, 2.0 }, ulim = { 1.8, 3.0 };;
    double result = getParallelIntegralMCarlo(pol_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result_1 = getSequentialIntegralMCarlo(pol_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result_1 << std::endl;
        ASSERT_NEAR(result, result_1, 0.5);
    }
}

TEST(Parallel_Operations_MPI, test3_comp_operation) {
    int rank;
    int n = 1000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim = { 0.0, 1.0, 2.35 }, ulim = { 1.5, 2.25, 2.675 };;
    double result = getParallelIntegralMCarlo(comp_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result_1 = getSequentialIntegralMCarlo(comp_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result_1 << std::endl;
        ASSERT_NEAR(result, result_1, 0.5);
    }
}

TEST(Parallel_Operations_MPI, test4_comp_operation) {
    int rank;
    int n = 10000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim = { 0.0, 1.0, 2.35 }, ulim = { 1.5, 2.25, 2.675 };;
    double result = getParallelIntegralMCarlo(comp_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result_1 = getSequentialIntegralMCarlo(comp_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result_1 << std::endl;
        ASSERT_NEAR(result, result_1, 0.5);
    }
}

TEST(Parallel_Operations_MPI, test5_lin_operation) {
    int rank;
    int n = 1000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim(3), ulim(3);
    for (int i = 0; i < 3; i++) {
        llim[i] = 0.5, ulim[i] = 1.5;
    }
    double result = getParallelIntegralMCarlo(lin_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result1 = getSequentialIntegralMCarlo(lin_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result1 << std::endl;
        ASSERT_NEAR(result, result1, 0.5);
    }
}

TEST(Parallel_Operations_MPI, test6_lin_operation) {
    int rank;
    int n = 10000;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start = MPI_Wtime();
    std::vector<double>llim(3), ulim(3);
    for (int i = 0; i < 3; i++) {
        llim[i] = 0.5, ulim[i] = 1.5;
    }
    double result = getParallelIntegralMCarlo(lin_f, llim, ulim, n);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "TimeParallelMethod: " << end - start << std::endl;
        start = MPI_Wtime();
        double result_1 = getSequentialIntegralMCarlo(lin_f, llim, ulim, n);
        double end = MPI_Wtime();
        std::cout << "ResultParallelMethod: " << result << std::endl;
        std::cout << "TimeSeqMethod: " << end - start << std::endl;
        std::cout << "ResultSeqMethod: " << result_1 << std::endl;
        ASSERT_NEAR(result, result_1, 0.5);
    }
}

int main(int argc, char** argv) {
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
