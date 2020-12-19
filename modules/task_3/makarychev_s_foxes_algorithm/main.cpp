// Copyright 2020 Makarychev Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "./foxes_algorithm.h"

TEST(Parallel_Operations_MPI, Test_9) {
    int rank, ProcNum;
    double beginT, beginT1, endT, endT1;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matA;
    std::vector<double> matB;
    std::vector<double> resultPar;
    int size = 9;
    int gridSize = static_cast<int>(sqrt(ProcNum));
    if (gridSize * gridSize == ProcNum) {
        int remains = size % gridSize;
        if (remains)
            size += gridSize - (size % gridSize);
        if (rank == 0) {
            matA = getRandomMatrix(size);
            matB = getRandomMatrix(size);
        }
        beginT1 = MPI_Wtime();
        resultPar = foxsAlgorithm(matA, matB, size);
        if (rank == 0) {
            endT1 = MPI_Wtime();
            beginT = MPI_Wtime();
            std::vector<double> resultSeq = seqMult(matA, matB, size);
            endT = MPI_Wtime();
            std::cout << "MPI time:  " << endT1 - beginT1 << std::endl;
            std::cout << "SEQ time: " << endT - beginT << std::endl;
            std::cout << "efficiency: " << (endT - beginT) / (endT1 - beginT1) << std::endl;
            ASSERT_TRUE(compareMat(resultPar, resultSeq));
        }
    } else {
        if (rank == 0) {
            ASSERT_FALSE(0);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_67) {
    int rank, ProcNum;
    double beginT, beginT1, endT, endT1;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matA;
    std::vector<double> matB;
    std::vector<double> resultPar;
    int size = 67;
    int gridSize = static_cast<int>(sqrt(ProcNum));
    if (gridSize * gridSize == ProcNum) {
        int remains = size % gridSize;
        if (remains)
            size += gridSize - (size % gridSize);
        if (rank == 0) {
            matA = getRandomMatrix(size);
            matB = getRandomMatrix(size);
        }
        beginT1 = MPI_Wtime();
        resultPar = foxsAlgorithm(matA, matB, size);
        if (rank == 0) {
            endT1 = MPI_Wtime();
            beginT = MPI_Wtime();
            std::vector<double> resultSeq = seqMult(matA, matB, size);
            endT = MPI_Wtime();
            std::cout << "MPI time:  " << endT1 - beginT1 << std::endl;
            std::cout << "SEQ time: " << endT - beginT << std::endl;
            std::cout << "efficiency: " << (endT - beginT) / (endT1 - beginT1) << std::endl;
            ASSERT_TRUE(compareMat(resultPar, resultSeq));
        }
    } else {
        if (rank == 0) {
            ASSERT_FALSE(0);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_90) {
    int rank, ProcNum;
    double beginT, beginT1, endT, endT1;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matA;
    std::vector<double> matB;
    std::vector<double> resultPar;
    int size = 90;
    int gridSize = static_cast<int>(sqrt(ProcNum));
    if (gridSize * gridSize == ProcNum) {
        int remains = size % gridSize;
        if (remains)
            size += gridSize - (size % gridSize);
        if (rank == 0) {
            matA = getRandomMatrix(size);
            matB = getRandomMatrix(size);
        }
        beginT1 = MPI_Wtime();
        resultPar = foxsAlgorithm(matA, matB, size);
        if (rank == 0) {
            endT1 = MPI_Wtime();
            beginT = MPI_Wtime();
            std::vector<double> resultSeq = seqMult(matA, matB, size);
            endT = MPI_Wtime();
            std::cout << "MPI time:  " << endT1 - beginT1 << std::endl;
            std::cout << "SEQ time: " << endT - beginT << std::endl;
            std::cout << "efficiency: " << (endT - beginT) / (endT1 - beginT1) << std::endl;
            ASSERT_TRUE(compareMat(resultPar, resultSeq));
        }
    } else {
        if (rank == 0) {
            ASSERT_FALSE(0);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_150) {
    int rank, ProcNum;
    double beginT, beginT1, endT, endT1;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matA;
    std::vector<double> matB;
    std::vector<double> resultPar;
    int size = 150;
    int gridSize = static_cast<int>(sqrt(ProcNum));
    if (gridSize * gridSize == ProcNum) {
        int remains = size % gridSize;
        if (remains)
            size += gridSize - (size % gridSize);
        if (rank == 0) {
            matA = getRandomMatrix(size);
            matB = getRandomMatrix(size);
        }
        beginT1 = MPI_Wtime();
        resultPar = foxsAlgorithm(matA, matB, size);
        if (rank == 0) {
            endT1 = MPI_Wtime();
            beginT = MPI_Wtime();
            std::vector<double> resultSeq = seqMult(matA, matB, size);
            endT = MPI_Wtime();
            std::cout << "MPI time:  " << endT1 - beginT1 << std::endl;
            std::cout << "SEQ time: " << endT - beginT << std::endl;
            std::cout << "efficiency: " << (endT - beginT) / (endT1 - beginT1) << std::endl;
            ASSERT_TRUE(compareMat(resultPar, resultSeq));
        }
    } else {
        if (rank == 0) {
            ASSERT_FALSE(0);
        }
    }
}

TEST(Parallel_Operations_MPI, Test_229) {
    int rank, ProcNum;
    double beginT, beginT1, endT, endT1;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<double> matA;
    std::vector<double> matB;
    std::vector<double> resultPar;
    int size = 229;
    int gridSize = static_cast<int>(sqrt(ProcNum));
    if (gridSize * gridSize == ProcNum) {
        int remains = size % gridSize;
        if (remains)
            size += gridSize - (size % gridSize);
        if (rank == 0) {
            matA = getRandomMatrix(size);
            matB = getRandomMatrix(size);
        }
        beginT1 = MPI_Wtime();
        resultPar = foxsAlgorithm(matA, matB, size);
        if (rank == 0) {
            endT1 = MPI_Wtime();
            beginT = MPI_Wtime();
            std::vector<double> resultSeq = seqMult(matA, matB, size);
            endT = MPI_Wtime();
            std::cout << "MPI time:  " << endT1 - beginT1 << std::endl;
            std::cout << "SEQ time: " << endT - beginT << std::endl;
            std::cout << "efficiency: " << (endT - beginT) / (endT1 - beginT1) << std::endl;
            ASSERT_TRUE(compareMat(resultPar, resultSeq));
        }
    } else {
        if (rank == 0) {
            ASSERT_FALSE(0);
        }
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
