// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include "./radix_with_merge.h"

TEST(RadixSortWithMergeMPI, Empty_Vector) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 0;

    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);

    if (procRank == 0) {
        std::vector<int> sequentalResult = radixSortSigned(randomVector);

        EXPECT_EQ(parallelResult, randomVector);
        EXPECT_EQ(sequentalResult, randomVector);
    }
}

TEST(RadixSortWithMergeMPI, Ref_Size_1) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 1;

    if (procRank == 0) {
        randomVector = { -758 };
    }

    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);

    if (procRank == 0) {
        std::vector<int> sequentalResult = radixSortSigned(randomVector);

        EXPECT_EQ(parallelResult, randomVector);
        EXPECT_EQ(sequentalResult, randomVector);
    }
}

TEST(RadixSortWithMergeMPI, Ref_Size_5) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 5;

    if (procRank == 0) {
        randomVector = { 4, -2, -10, 5, -12 };
    }

    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);

    if (procRank == 0) {
        std::vector<int> reference = { -12, -10, -2, 4, 5 };

        std::vector<int> sequentalResult = radixSortSigned(randomVector);

        EXPECT_EQ(parallelResult, reference);
        EXPECT_EQ(sequentalResult, reference);
    }
}

TEST(RadixSortWithMergeMPI, Random_Size_97) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 97;

    if (procRank == 0) {
        randomVector = generateRandomVector<int>(vectorSize);
    }

    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);

    if (procRank == 0) {
        std::vector<int> sequentalResult = radixSortSigned(randomVector);

        std::sort(randomVector.begin(), randomVector.end());

        EXPECT_EQ(parallelResult, randomVector);
        EXPECT_EQ(sequentalResult, randomVector);
    }
}

TEST(RadixSortWithMergeMPI, Random_Size_991) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 991;

    if (procRank == 0) {
        randomVector = generateRandomVector<int>(vectorSize);
    }

    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);

    if (procRank == 0) {
        std::vector<int> sequentalResult = radixSortSigned(randomVector);

        std::sort(randomVector.begin(), randomVector.end());

        EXPECT_EQ(parallelResult, randomVector);
        EXPECT_EQ(sequentalResult, randomVector);
    }
}

TEST(RadixSortWithMergeMPI, DISABLED_Size_100000_With_Time) {
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    std::vector<int> randomVector;

    const int vectorSize = 100000;

    if (procRank == 0) {
        randomVector = generateRandomVector<int>(vectorSize);
    }

    auto parallelStart = MPI_Wtime();
    std::vector<int> parallelResult = parallelRadixSort(randomVector, vectorSize);
    auto parallelEnd = MPI_Wtime();

    if (procRank == 0) {
        std::cout << "Parallel duration: " << parallelEnd - parallelStart << std::endl;

        auto seqStart = MPI_Wtime();
        std::vector<int> sequentalResult = radixSortSigned(randomVector);
        auto seqEnd = MPI_Wtime();

        std::cout << "Seq duration: " << seqEnd - seqStart << std::endl;

        std::sort(randomVector.begin(), randomVector.end());

        EXPECT_EQ(parallelResult, randomVector);
        EXPECT_EQ(sequentalResult, randomVector);
    }
}

int main(int argc, char* argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners &listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
