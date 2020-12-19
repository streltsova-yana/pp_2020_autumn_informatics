// Copyright 2020 Panova Olga
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <cmath>
#include <iostream>
#include "../../../modules/task_3/panova_o_optimization_splt_area/optimization_split_area.h"
double func1(double* _x) {
    double x = *_x;
    return cos(2 * x) * sin(5 * x);  // global minmum 1.568
}
double func2(double* _x) {
    double x = *_x;
    return x * sin(5 * x);  // global minimum 4.72
}
double func3(double* _x) {
    double x = *_x;
    return exp(-0.5 * x) * sin(6 * x - 1.5);  // global minimum -2.12
}
double func4(double* _x) {
    double x = *_x;
    return -pow(x, 3) / 3 + 3 * pow(x, 2) - 5 * x - 1;  // global minimum 1
}
TEST(GlobalOptimization, SequentialSearchIsCorrect) {
    double res = SequentialOptimization(-2, 2, func1, 1e-5);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        ASSERT_NEAR(res, 1.568, 1e-2);
    }
}
TEST(GlobalOptimization, SeqAndParEquality) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start_seq = MPI_Wtime();
    double res_seq = SequentialOptimization(-2, 2, func1, 1e-5);
    double end_seq = MPI_Wtime();
    double start_par = MPI_Wtime();
    double res_par = ParallelOptimization(-2, 2, func1, 1e-5);
    double end_par = MPI_Wtime();
    if (rank == 0) {
        if ((end_par - start_par) > (end_seq - start_seq)) {
            std::cout << "Sequential more effective" << std::endl;
            std::cout << "Time difference: " << (end_par - start_par) - (end_seq - start_seq) << std::endl;
        } else {
            std::cout << "Parallel more effective" << std::endl;
            std::cout << "Time difference: " << (end_seq - start_seq) - (end_par - start_par) << std::endl;
        }
        ASSERT_NEAR(res_seq, res_par, 1e-2);
    }
}
TEST(GlobalOptimization, MultiExtraFunction_Trig) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start_seq = MPI_Wtime();
    double res_seq = SequentialOptimization(-1, 5, func2, 1e-5);
    double end_seq = MPI_Wtime();
    double start_par = MPI_Wtime();
    double res_par = ParallelOptimization(-1, 5, func2, 1e-5);
    double end_par = MPI_Wtime();
    if (rank == 0) {
        if ((end_par - start_par) > (end_seq - start_seq)) {
            std::cout << "Sequential more effective" << std::endl;
            std::cout << "Time difference: " << (end_par - start_par) - (end_seq - start_seq) << std::endl;
        } else {
            std::cout << "Parallel more effective" << std::endl;
            std::cout << "Time difference: " << (end_seq - start_seq) - (end_par - start_par) << std::endl;
        }
        ASSERT_NEAR(res_par, res_seq, 1e-2);
    }
}
TEST(GlobalOptimization, TwoExtraFunction_Exp) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start_seq = MPI_Wtime();
    double res_seq = SequentialOptimization(-3, 1, func3, 1e-5);
    double end_seq = MPI_Wtime();
    double start_par = MPI_Wtime();
    double res_par = ParallelOptimization(-3, 1, func3, 1e-5);
    double end_par = MPI_Wtime();
    if (rank == 0) {
        if ((end_par - start_par) > (end_seq - start_seq)) {
            std::cout << "Sequential more effective" << std::endl;
            std::cout << "Time difference: " << (end_par - start_par) - (end_seq - start_seq) << std::endl;
        } else {
            std::cout << "Parallel more effective" << std::endl;
            std::cout << "Time difference: " << (end_seq - start_seq) - (end_par - start_par) << std::endl;
        }
        ASSERT_NEAR(res_par, res_seq, 1e-2);
    }
}
TEST(GlobalOptimization, MultiExtraFunction_Polinom) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double start_seq = MPI_Wtime();
    double res_seq = SequentialOptimization(0, 7, func4, 1e-5);
    double end_seq = MPI_Wtime();
    double start_par = MPI_Wtime();
    double res_par = ParallelOptimization(0, 7, func4, 1e-5);
    double end_par = MPI_Wtime();
    if (rank == 0) {
        if ((end_par - start_par) > (end_seq - start_seq)) {
            std::cout << "Sequential more effective" << std::endl;
            std::cout << "Time difference: " << (end_par - start_par) - (end_seq - start_seq) << std::endl;
        } else {
            std::cout << "Parallel more effective" << std::endl;
            std::cout << "Time difference: " << (end_seq - start_seq) - (end_par - start_par) << std::endl;
        }
        ASSERT_NEAR(res_par, res_seq, 1e-1);
    }
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& lst = ::testing::UnitTest::GetInstance()->listeners();
    lst.Release(lst.default_result_printer());
    lst.Release(lst.default_xml_generator());
    lst.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
