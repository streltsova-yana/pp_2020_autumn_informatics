// Copyright 2020 Bulychev Vladislav
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "../../../modules/task_3/bulychev_v_calculation_of_integrals/calculation.h"

double f1(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    return (x * 2 * x + 5 * y);
}

double f2(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    double z = t[2];
    return (x + y + 6 + z * z);
}

double f3(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    return (x + y + 1);
}

double f4(std::vector<double> t) {
    double x = t[0];
    double y = t[1];
    double z = t[2];
    double d = t[3];
    return (x + y + 6 + z * z * 2 - + d + 3 );
}

TEST(Calculation_Integraion, Return_correct_answer_sequential_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    if (rank == 0) {
        a[0] = 5;
        b[0] = 14;

        a[1] = 3;
        b[1] = 21;

        ASSERT_NO_THROW(SequentialCalculation(a, b, 5, f3));
    }
}

TEST(Calculation_Integraion, Return_correct_answer_parallel_method) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    if (rank == 0) {
        a[0] = 5;
        b[0] = 14;

        a[1] = 3;
        b[1] = 21;
    }

    ASSERT_NO_THROW(ParallelCalculation(a, b, 5, f3));
}

TEST(Calculation_Integraion, Test_1_for_two_dimensional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    if (rank == 0) {
        a[0] = 0;
        b[0] = 100;

        a[1] = 0;
        b[1] = 10;
    }

    double result_par = ParallelCalculation(a, b, 10, f3);

    if (rank == 0) {
        double result_seq = SequentialCalculation(a, b, 10, f3);

        ASSERT_NEAR(result_par, result_seq, 0.01);
    }
}

TEST(Calculation_Integraion, Test_2_for_two_dimensional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 2;
    std::vector<double> a(s);
    std::vector<double> b(s);

    if (rank == 0) {
        a[0] = 0;
        b[0] = 1;

        a[1] = 0;
        b[1] = 10;
    }

    double result_par = ParallelCalculation(a, b, 10, f1);

    if (rank == 0) {
        double result_seq = SequentialCalculation(a, b, 10, f1);

        ASSERT_NEAR(result_par, result_seq, 0.01);
    }
}

TEST(Calculation_Integraion, Test_for_three_dimensional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 3;
    std::vector<double> a(s);
    std::vector<double> b(s);
    if (rank == 0) {
        a[0] = 0;
        b[0] = 1;

        a[1] = 0;
        b[1] = 10;

        a[2] = 0;
        b[2] = 100;
    }

    double result_par = ParallelCalculation(a, b, 10, f2);

    if (rank == 0) {
        double result_seq = SequentialCalculation(a, b, 10, f2);
        ASSERT_NEAR(result_par, result_seq, 0.001);
    }
}

TEST(Calculation_Integraion, Test_for_four_dimensional) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = 4;
    std::vector<double> a(s);
    std::vector<double> b(s);
    if (rank == 0) {
        a[0] = 0;
        b[0] = 1;

        a[1] = 0;
        b[1] = 10;

        a[2] = 0;
        b[2] = 100;

        a[3] = 4;
        b[3] = 15;
    }

    double result_par = ParallelCalculation(a, b, 5, f4);

    if (rank == 0) {
        double result_seq = SequentialCalculation(a, b, 5, f4);
        ASSERT_NEAR(result_par, result_seq, 0.001);
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
