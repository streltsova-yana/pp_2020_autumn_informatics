// Copyright 2020 Novozhilova Ekaterina
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <ctime>
#include <iostream>
#include "./cannon_s_algorithm.h"

TEST(Cannon_s_algorithm_MPI, Test_size_400) {
    double t1par = MPI_Wtime();
    int Comm_rank, Comm_size;
    int dim = 400;
    std::vector<double> MatrixA;
    std::vector<double> MatrixB;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    if (Comm_size < 4) {
        if (Comm_rank == 0) {
            std::cout << "ERORR!! NOT ENOUGH PROCESSES" << std::endl;
            ASSERT_EQ(0, 0);
        }
    } else {
        if (Comm_rank == 0) {
            MatrixA = GenMatrix(dim);
            MatrixB = GenMatrix(dim);
        }
        std::vector<double> res = CannonAlgorithm(MatrixA, MatrixB, dim);
        double t2par = MPI_Wtime();
        if (Comm_rank == 0) {
            std::cout << std::endl << "time of parallel operations is " << t2par - t1par << std::endl;
            std::vector<double> expected;
            double t1seq = MPI_Wtime();
            expected = SeqMultiply(MatrixA, MatrixB, dim);
            double t2seq = MPI_Wtime();
            std::cout << std::endl << "time of sequantial operations is " << t2seq - t1seq << std::endl;
            for (int i = 0; i < dim*dim; i++) {
                ASSERT_NEAR(expected[i], res[i], 0.001);
            }
        }
    }
}

TEST(Cannon_s_algorithm_MPI, Test_size_200) {
    double t1par = MPI_Wtime();
    int Comm_rank, Comm_size;
    int dim = 200;
    std::vector<double> MatrixA;
    std::vector<double> MatrixB;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    if (Comm_size < 4) {
        if (Comm_rank == 0) {
            std::cout << "ERORR!! NOT ENOUGH PROCESSES" << std::endl;
            ASSERT_EQ(0, 0);
        }
    } else {
        if (Comm_rank == 0) {
            MatrixA = GenMatrix(dim);
            MatrixB = GenMatrix(dim);
        }
        std::vector<double> res = CannonAlgorithm(MatrixA, MatrixB, dim);
        double t2par = MPI_Wtime();
        if (Comm_rank == 0) {
            std::cout << std::endl << "time of parallel operations is " << t2par - t1par << std::endl;
            std::vector<double> expected;
            double t1seq = MPI_Wtime();
            expected = SeqMultiply(MatrixA, MatrixB, dim);
            double t2seq = MPI_Wtime();
            std::cout << std::endl << "time of sequantial operations is " << t2seq - t1seq << std::endl;
            for (int i = 0; i < dim*dim; i++) {
                ASSERT_NEAR(expected[i], res[i], 0.001);
            }
        }
    }
}
TEST(Cannon_s_algorithm_MPI, Test_size_300) {
    double t1par = MPI_Wtime();
    int Comm_rank, Comm_size;
    int dim = 300;
    std::vector<double> MatrixA;
    std::vector<double> MatrixB;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    if (Comm_size < 4) {
        if (Comm_rank == 0) {
            std::cout << "ERORR!! NOT ENOUGH PROCESSES" << std::endl;
            ASSERT_EQ(0, 0);
        }
    } else {
        if (Comm_rank == 0) {
            MatrixA = GenMatrix(dim);
            MatrixB = GenMatrix(dim);
        }
        std::vector<double> res = CannonAlgorithm(MatrixA, MatrixB, dim);
        double t2par = MPI_Wtime();
        if (Comm_rank == 0) {
            std::cout << std::endl << "time of parallel operations is " << t2par - t1par << std::endl;
            std::vector<double> expected;
            double t1seq = MPI_Wtime();
            expected = SeqMultiply(MatrixA, MatrixB, dim);
            double t2seq = MPI_Wtime();
            std::cout << std::endl << "time of sequantial operations is " << t2seq - t1seq << std::endl;
            for (int i = 0; i < dim*dim; i++) {
                ASSERT_NEAR(expected[i], res[i], 0.001);
            }
        }
    }
}

TEST(Cannon_s_algorithm_MPI, Test_size_1000) {
    double t1par = MPI_Wtime();
    int Comm_rank, Comm_size;
    int dim = 1000;
    std::vector<double> MatrixA;
    std::vector<double> MatrixB;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    if (Comm_size < 4) {
        if (Comm_rank == 0) {
            std::cout << "ERORR!! NOT ENOUGH PROCESSES" << std::endl;
            ASSERT_EQ(0, 0);
        }
    } else {
        if (Comm_rank == 0) {
            MatrixA = GenMatrix(dim);
            MatrixB = GenMatrix(dim);
        }
        std::vector<double> res = CannonAlgorithm(MatrixA, MatrixB, dim);
        double t2par = MPI_Wtime();
        if (Comm_rank == 0) {
            std::cout << std::endl << "time of parallel operations is " << t2par - t1par << std::endl;
            std::vector<double> expected;
            double t1seq = MPI_Wtime();
            expected = SeqMultiply(MatrixA, MatrixB, dim);
            double t2seq = MPI_Wtime();
            std::cout << std::endl << "time of sequantial operations is " << t2seq - t1seq << std::endl;
            for (int i = 0; i < dim*dim; i++) {
                ASSERT_NEAR(expected[i], res[i], 0.001);
            }
        }
    }
}
TEST(Cannon_s_algorithm_MPI, Test_size_564) {
    double t1par = MPI_Wtime();
    int Comm_rank, Comm_size;
    int dim = 564;
    std::vector<double> MatrixA;
    std::vector<double> MatrixB;
    MPI_Comm_rank(MPI_COMM_WORLD, &Comm_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &Comm_size);
    if (Comm_size < 4) {
        if (Comm_rank == 0) {
            std::cout << "ERORR!! NOT ENOUGH PROCESSES" << std::endl;
            ASSERT_EQ(0, 0);
        }
    } else {
        if (Comm_rank == 0) {
            MatrixA = GenMatrix(dim);
            MatrixB = GenMatrix(dim);
        }
        std::vector<double> res = CannonAlgorithm(MatrixA, MatrixB, dim);
        double t2par = MPI_Wtime();
        if (Comm_rank == 0) {
            std::cout << std::endl << "time of parallel operations is " << t2par - t1par << std::endl;
            std::vector<double> expected;
            double t1seq = MPI_Wtime();
            expected = SeqMultiply(MatrixA, MatrixB, dim);
            double t2seq = MPI_Wtime();
            std::cout << std::endl << "time of sequantial operations is " << t2seq - t1seq << std::endl;
            for (int i = 0; i < dim*dim; i++) {
                ASSERT_NEAR(expected[i], res[i], 0.001);
            }
        }
    }
}

int main(int argc, char**argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners = ::testing::UnitTest::GetInstance()->listeners();
    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());
    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
