// Copyright 2020 Chistov Vladimir

#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/chistov_v_gauss_block/gauss_block.h"
// fix7
TEST(Parallel_Count_Sentences_MPI, Image50x50) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int n = 50;
    double resS, resP;
    double time1, time2, time3, time4;
    std::vector<double> mas(n * n), res(n * n), res1(n * n);
    mas = GenRandVec(n * n);
    time1 = MPI_Wtime();
    res1 = Gauss_Parallel(mas, n, n);
    time2 = MPI_Wtime();
    resS = time2 - time1;
    if (ProcRank == 0) {
        time3 = MPI_Wtime();
        res = Gauss_Sequential(mas, n, n);
        time4 = MPI_Wtime();
        resP = time4 - time3;
        std::cout << "Time Parallel = " << resP << std::endl << "Time Sequential = " << resS << std::endl << std::endl;
    }
    ASSERT_NO_THROW();
}

TEST(Parallel_Count_Sentences_MPI, Image110x130) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int a = 110;
    int b = 130;
    double resS, resP;
    double time1, time2, time3, time4;
    std::vector<double> mas(a * b), res(a * b), res1(a * b);
    mas = GenRandVec(a * b);
    time1 = MPI_Wtime();
    res1 = Gauss_Parallel(mas, a, b);
    time2 = MPI_Wtime();
    resS = time2 - time1;
    if (ProcRank == 0) {
        time3 = MPI_Wtime();
        res = Gauss_Sequential(mas, a, b);
        time4 = MPI_Wtime();
        resP = time4 - time3;
        std::cout << "Time Parallel = " << resP << std::endl << "Time Sequential = " << resS << std::endl << std::endl;
    }
    ASSERT_NO_THROW();
}

TEST(Parallel_Count_Sentences_MPI, Image123x321) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int a = 123;
    int b = 321;
    double resS, resP;
    double time1, time2, time3, time4;
    std::vector<double> mas(a * b), res(a * b), res1(a * b);
    mas = GenRandVec(a * b);
    time1 = MPI_Wtime();
    res1 = Gauss_Parallel(mas, a, b);
    time2 = MPI_Wtime();
    resS = time2 - time1;
    if (ProcRank == 0) {
        time3 = MPI_Wtime();
        res = Gauss_Sequential(mas, a, b);
        time4 = MPI_Wtime();
        resP = time4 - time3;
        std::cout << "Time Parallel = " << resP << std::endl << "Time Sequential = " << resS << std::endl << std::endl;
    }
    ASSERT_NO_THROW();
}

TEST(Parallel_Count_Sentences_MPI, Image1920x1080) {
    int ProcRank, ProcNum;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    int a = 1920;
    int b = 1080;
    double resS, resP;
    double time1, time2, time3, time4;
    std::vector<double> mas(a * b), res(a * b), res1(a * b);
    mas = GenRandVec(a * b);
    time1 = MPI_Wtime();
    res1 = Gauss_Parallel(mas, a, b);
    time2 = MPI_Wtime();
    resS = time2 - time1;
    if (ProcRank == 0) {
        time3 = MPI_Wtime();
        res = Gauss_Sequential(mas, a, b);
        time4 = MPI_Wtime();
        resP = time4 - time3;
        std::cout << "Time Parallel = " << resP << std::endl << "Time Sequential = " << resS << std::endl << std::endl;
    }
    ASSERT_NO_THROW();
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
