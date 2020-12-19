// Copyright 2020 Bakaeva Maria
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <math.h>
#include <utility>
#include <vector>
#include <ctime>
#include <iostream>
#include "./integrals_rectangles_method.h"


    double f1(vector<double> vec) {
        int x = vec[0];
        int y = vec[1];
        return x + y * y;
    }

    double f2(vector<double> vec) {
        int x = vec[0];
        int y = vec[1];
        return x + y;
    }

    double f3(vector<double> vec) {
        int x = vec[0];
        int y = vec[1];
        return sin(3 * x) + cos(y);
    }

    double f4(vector<double> vec) {
        int x = vec[0];
        int y = vec[1];
        int z = vec[2];
        return 8 * y * y * z * exp(2 * x * y * z);
    }

    double f5(vector<double> vec) {
        int x = vec[0];
        int y = vec[1];
        int z = vec[2];
        return x * x * z * sin(x * y * z);
    }

TEST(IntegralsRectanglesMethod, DISABLED_timeTest) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double t1, t2;

    vector<pair<double, double>> a_b(2);
    a_b = {{1, 5}, {0, 3}};

    t1 = MPI_Wtime();
    double resParallel = getParallelIntegrals(100, a_b, f1);
    t2 = MPI_Wtime();

    if (rank == 0)
    std::cout << "Parallel time: " <<t2 - t1 << std::endl;

    if (rank == 0) {
        t1 = MPI_Wtime();
        double resLinear = getSequentialIntegrals(100, a_b, f1);
        t2 = MPI_Wtime();
        std::cout << "Linear time: " << t2 - t1 << std::endl;
        ASSERT_NEAR(resLinear, resParallel, 0.001);
    }
}

TEST(IntegralsRectanglesMethod, testCalculate_f1) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<pair<double, double>> a_b(2);
    a_b = {{1, 5}, {0, 3}};

    double resParallel = getParallelIntegrals(100, a_b, f1);

    if (rank == 0) {
        double resLinear = getSequentialIntegrals(100, a_b, f1);
            ASSERT_NEAR(resLinear, resParallel, 0.001);
    }
}

TEST(IntegralsRectanglesMethod, testCalculate_f2) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<pair<double, double>> a_b(2);
    a_b = {{1, 5}, {0, 3}};

    double resParallel = getParallelIntegrals(100, a_b, f2);

    if (rank == 0) {
        double resLinear = getSequentialIntegrals(100, a_b, f2);
            ASSERT_NEAR(resLinear, resParallel, 0.001);
    }
}

TEST(IntegralsRectanglesMethod, testCalculate_f3) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<pair<double, double>> a_b(2);
    a_b = {{1, 5}, {0, 3}};

    double resParallel = getParallelIntegrals(100, a_b, f3);

    if (rank == 0) {
        double resLinear = getSequentialIntegrals(100, a_b, f3);
            ASSERT_NEAR(resLinear, resParallel, 0.001);
    }
}

TEST(IntegralsRectanglesMethod, testCalculate_f4) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<pair<double, double>> a_b(3);
    a_b = {{-1, 0}, {0, 2}, {0, 1}};

    double resParallel = getParallelIntegrals(100, a_b, f4);

    if (rank == 0) {
        double resLinear = getSequentialIntegrals(100, a_b, f4);
            ASSERT_NEAR(resLinear, resParallel, 0.001);
    }
}

TEST(IntegralsRectanglesMethod, testCalculate_f5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    vector<pair<double, double>> a_b(3);
    a_b = {{0, 2}, {0, 1}, {0, 3.14}};

    double resParallel = getParallelIntegrals(100, a_b, f5);

    if (rank == 0) {
        double resLinear = getSequentialIntegrals(100, a_b, f5);
            ASSERT_NEAR(resLinear, resParallel, 0.001);
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
