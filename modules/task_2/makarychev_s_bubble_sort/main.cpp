// Copyright 2020 Makarychev Sergey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./bubble_sort.h"

TEST(Parallel_Operations_MPI, Test_120) {
    int rank;
    double beginT, endT, timeMPI;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec1;
    std::vector<int> globalVec2;
    const int size = 120;
    if (rank == 0) {
        globalVec1 = getRandomVector(size);
        globalVec2 = globalVec1;
        beginT = MPI_Wtime();
        std::sort(globalVec2.begin(), globalVec2.begin() + size);
        endT = MPI_Wtime();
    }

    sortVecParallel(globalVec1.data(), size, &timeMPI);

    if (rank == 0) {
        std::cout << "std::sort time: " << endT - beginT << std::endl;
        std::cout << "efficiency: " << (endT - beginT) / timeMPI << std::endl;
        ASSERT_EQ(globalVec1, globalVec2);
    }
}
TEST(Parallel_Operations_MPI, Test_37) {
    int rank;
    double beginT, endT, timeMPI;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec1;
    std::vector<int> globalVec2;
    const int size = 37;
    if (rank == 0) {
        globalVec1 = getRandomVector(size);
        globalVec2 = globalVec1;
        beginT = MPI_Wtime();
        std::sort(globalVec2.begin(), globalVec2.begin() + size);
        endT = MPI_Wtime();
    }

    sortVecParallel(globalVec1.data(), size, &timeMPI);

    if (rank == 0) {
        std::cout << "std::sort time: " << endT - beginT << std::endl;
        std::cout << "efficiency: " << (endT - beginT) / timeMPI << std::endl;
        ASSERT_EQ(globalVec1, globalVec2);
    }
}
TEST(Parallel_Operations_MPI, Test_560) {
    int rank;
    double beginT, endT, timeMPI;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec1;
    std::vector<int> globalVec2;
    const int size = 560;
    if (rank == 0) {
        globalVec1 = getRandomVector(size);
        globalVec2 = globalVec1;
        beginT = MPI_Wtime();
        std::sort(globalVec2.begin(), globalVec2.begin() + size);
        endT = MPI_Wtime();
    }

    sortVecParallel(globalVec1.data(), size, &timeMPI);

    if (rank == 0) {
        std::cout << "std::sort time: " << endT - beginT << std::endl;
        std::cout << "efficiency: " << (endT - beginT) / timeMPI << std::endl;
        ASSERT_EQ(globalVec1, globalVec2);
    }
}
TEST(Parallel_Operations_MPI, Test_1397) {
    int rank;
    double beginT, endT, timeMPI;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec1;
    std::vector<int> globalVec2;
    const int size = 1397;
    if (rank == 0) {
        globalVec1 = getRandomVector(size);
        globalVec2 = globalVec1;
        beginT = MPI_Wtime();
        std::sort(globalVec2.begin(), globalVec2.begin() + size);
        endT = MPI_Wtime();
    }

    sortVecParallel(globalVec1.data(), size, &timeMPI);

    if (rank == 0) {
        std::cout << "std::sort time: " << endT - beginT << std::endl;
        std::cout << "efficiency: " << (endT - beginT) / timeMPI << std::endl;
        ASSERT_EQ(globalVec1, globalVec2);
    }
}
TEST(Parallel_Operations_MPI, Test_1049) {
    int rank;
    double beginT, endT, timeMPI;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::vector<int> globalVec1;
    std::vector<int> globalVec2;
    const int size = 1049;
    if (rank == 0) {
        globalVec1 = getRandomVector(size);
        globalVec2 = globalVec1;
        beginT = MPI_Wtime();
        std::sort(globalVec2.begin(), globalVec2.begin() + size);
        endT = MPI_Wtime();
    }

    sortVecParallel(globalVec1.data(), size, &timeMPI);

    if (rank == 0) {
        std::cout << "std::sort time: " << endT - beginT << std::endl;
        std::cout << "efficiency: " << (endT - beginT) / timeMPI << std::endl;
        ASSERT_EQ(globalVec1, globalVec2);
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
