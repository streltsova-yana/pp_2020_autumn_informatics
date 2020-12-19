// Copyright 2020 Pronin Igor
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <cmath>
#include "./trapeze.h"

double function1(std::vector<double> node) {
    double x = node[0];
    double y = node[1];
    return (x + y);
}
double function2(std::vector<double> node) {
    double x = node[0];
    double y = node[1];
    return (x * x + y);
}
double function3(std::vector<double> node) {
    double x = node[0];
    double y = node[1];
    double z = node[2];
    return(x + y + z);
}
double function4(std::vector<double> node) {
    double x = node[0];
    double y = node[1];
    double z = node[2];
    return(x * x + y + z * z);
}
double function5(std::vector<double> node) {
    double x = node[0];
    double y = node[1];
    return(cos(x) + sin(y));
}

TEST(Parallel_Operations_MPI, function1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mer = 2;
    int n = 100;
    std::vector<double> a(mer);
    std::vector<double> b(mer);
    a = { 1, 2 };
    b = { 5, 6 };
    double presult = ParllelOperations(function1, a, b, n);
    if (rank == 0) {
        double sresult = SequentialOperations(function1, a, b, n);
        double error = 0.0001;
        ASSERT_NEAR(presult, sresult, error);
    }
}

TEST(Parallel_Operations_MPI, function2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mer = 2;
    int n = 100;
    std::vector<double> a(mer);
    std::vector<double> b(mer);
    a = { -1, 2 };
    b = { 5, 6 };
    double presult = ParllelOperations(function2, a, b, n);
    if (rank == 0) {
        double sresult = SequentialOperations(function2, a, b, n);
        double error = 0.0001;
        ASSERT_NEAR(presult, sresult, error);
    }
}

TEST(Parallel_Operations_MPI, function3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mer = 2;
    int n = 100;
    std::vector<double> a(mer);
    std::vector<double> b(mer);
    a = { 5, 2, 3 };
    b = { -1, 6, 10 };
    double presult = ParllelOperations(function3, a, b, n);
    if (rank == 0) {
        double sresult = SequentialOperations(function3, a, b, n);
        double error = 0.0001;
        ASSERT_NEAR(presult, sresult, error);
    }
}

TEST(Parallel_Operations_MPI, function4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mer = 2;
    int n = 100;
    std::vector<double> a(mer);
    std::vector<double> b(mer);
    a = { 5, 2, 3 };
    b = { -1, 12, 20 };
    double presult = ParllelOperations(function4, a, b, n);
    if (rank == 0) {
        double sresult = SequentialOperations(function4, a, b, n);
        double error = 0.0001;
        ASSERT_NEAR(presult, sresult, error);
    }
}

TEST(Parallel_Operations_MPI, function5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int mer = 2;
    int n = 100;
    std::vector<double> a(mer);
    std::vector<double> b(mer);
    a = { -1, 11 };
    b = { 5, 32 };
    double presult = ParllelOperations(function5, a, b, n);
    if (rank == 0) {
        double sresult = SequentialOperations(function5, a, b, n);
        double error = 0.0001;
        ASSERT_NEAR(presult, sresult, error);
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
