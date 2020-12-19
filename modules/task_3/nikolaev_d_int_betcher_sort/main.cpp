// Copyright 2020 Nikolaev Denis
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include "../../../modules/task_3/nikolaev_d_int_betcher_sort/SortIntBetcher.h"

TEST(Radix_Sort_Merge_Batcher, TEST1) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    const int n = 10;
    vec = genRandVector(n);
    std::vector<int> vec1 = BetcherMerge(vec, n);
    if (ProcRank == 0) {
        std::vector<int> vec2 = SequentialRadixSort(vec);
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Radix_Sort_Merge_Batcher, TEST2) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    int n = 25;
    vec = genRandVector(n);
    std::vector<int> vec1 = BetcherMerge(vec, n);
    if (ProcRank == 0) {
        std::vector<int> vec2 = SequentialRadixSort(vec);
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Radix_Sort_Merge_Batcher, TEST3) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    int n = 50;
    vec = genRandVector(n);
    std::vector<int> vec1 = BetcherMerge(vec, n);
    if (ProcRank == 0) {
        std::vector<int> vec2 = SequentialRadixSort(vec);
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Radix_Sort_Merge_Batcher, TEST4) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    int n = 100;
    vec = genRandVector(n);
    std::vector<int> vec1 = BetcherMerge(vec, n);
    if (ProcRank == 0) {
        std::vector<int> vec2 = SequentialRadixSort(vec);
        ASSERT_EQ(vec1, vec2);
    }
}
TEST(Radix_Sort_Merge_Batcher, TEST5) {
    int ProcRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    std::vector<int> vec;
    int n = 150;
    vec = genRandVector(n);
    std::vector<int> vec1 = BetcherMerge(vec, n);
    if (ProcRank == 0) {
        std::vector<int> vec2 = SequentialRadixSort(vec);
        ASSERT_EQ(vec1, vec2);
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
