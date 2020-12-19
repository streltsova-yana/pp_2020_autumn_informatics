// Copyright 2020 Streltsova Yana
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <ctime>
#include "./Strassens_algorithm.h"

TEST(Parallel_Operations_MPI, Test_7x7) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 7;
    std::mt19937 gen(time(0));
    Matrix a, b;
    if (rank == 0) {
        a = Matrix(n, n, gen);
        b = Matrix(n, n, gen);
    }
    double start = MPI_Wtime();
    Matrix* c_parallel = Strassen_alg(a, b);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel time: " << end - start << std::endl;
        start = MPI_Wtime();
        Matrix* c_seq = sequential_mul(a, b);
        end = MPI_Wtime();
        std::cout << "Sequential time: " << end - start << std::endl;
        c_parallel = get_orig_size_matrix(*c_parallel, n);
        for (int i = 0; i < n * n; i ++)
            ASSERT_DOUBLE_EQ(round(c_seq->m[i] * 10) / 10, round(c_parallel->m[i] * 10) / 10);
    }
}
TEST(Parallel_Operations_MPI, Test_16x16) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 16;
    std::mt19937 gen(time(0));
    Matrix a, b;
    if (rank == 0) {
        a = Matrix(n, n, gen);
        b = Matrix(n, n, gen);
    }
    double start = MPI_Wtime();
    Matrix* c_parallel = Strassen_alg(a, b);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel time: " << end - start << std::endl;
        start = MPI_Wtime();
        Matrix* c_seq = sequential_mul(a, b);
        end = MPI_Wtime();
        std::cout << "Sequential time: " << end - start << std::endl;
        c_parallel = get_orig_size_matrix(*c_parallel, n);
        for (int i = 0; i < n * n; i++)
            ASSERT_DOUBLE_EQ(round(c_seq->m[i] * 10) / 10, round(c_parallel->m[i] * 10) / 10);
    }
}
TEST(Parallel_Operations_MPI, Test_64x64) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 64;
    std::mt19937 gen(time(0));
    Matrix a, b;
    if (rank == 0) {
        a = Matrix(n, n, gen);
        b = Matrix(n, n, gen);
    }
    double start = MPI_Wtime();
    Matrix* c_parallel = Strassen_alg(a, b);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel time: " << end - start << std::endl;
        start = MPI_Wtime();
        Matrix* c_seq = sequential_mul(a, b);
        end = MPI_Wtime();
        std::cout << "Sequential time: " << end - start << std::endl;
        c_parallel = get_orig_size_matrix(*c_parallel, n);
        for (int i = 0; i < n * n; i++)
            ASSERT_DOUBLE_EQ(round(c_seq->m[i] * 10) / 10, round(c_parallel->m[i] * 10) / 10);
    }
}
TEST(Parallel_Operations_MPI, Test_128x128) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 128;
    std::mt19937 gen(time(0));
    Matrix a, b;
    if (rank == 0) {
        a = Matrix(n, n, gen);
        b = Matrix(n, n, gen);
    }
    double start = MPI_Wtime();
    Matrix* c_parallel = Strassen_alg(a, b);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel time: " << end - start << std::endl;
        start = MPI_Wtime();
        Matrix* c_seq = sequential_mul(a, b);
        end = MPI_Wtime();
        std::cout << "Sequential time: " << end - start << std::endl;
        c_parallel = get_orig_size_matrix(*c_parallel, n);
        for (int i = 0; i < n * n; i++)
            ASSERT_DOUBLE_EQ(round(c_seq->m[i] * 10) / 10, round(c_parallel->m[i] * 10) / 10);
    }
}
TEST(Parallel_Operations_MPI, Test_257x257) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 257;
    std::mt19937 gen(time(0));
    Matrix a, b;
    if (rank == 0) {
        a = Matrix(n, n, gen);
        b = Matrix(n, n, gen);
    }
    double start = MPI_Wtime();
    Matrix* c_parallel = Strassen_alg(a, b);
    double end = MPI_Wtime();
    if (rank == 0) {
        std::cout << "Parallel time: " << end - start << std::endl;
        start = MPI_Wtime();
        Matrix* c_seq = sequential_mul(a, b);
        end = MPI_Wtime();
        std::cout << "Sequential time: " << end - start << std::endl;
        c_parallel = get_orig_size_matrix(*c_parallel, n);
        for (int i = 0; i < n * n; i++)
            ASSERT_DOUBLE_EQ(round(c_seq->m[i] * 10) / 10, round(c_parallel->m[i] * 10) / 10);
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
