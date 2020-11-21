// Copyright 2020 Kolesin Andrey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <stdio.h>
#include <random>
#include "./count_words.h"

TEST(Count_Words, Test_Empty) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("");
    if (rank == 0) {
        ASSERT_EQ(res, 0);
    }
}
TEST(Count_Words, Test_Zero) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("   ");
    if (rank == 0) {
        ASSERT_EQ(res, 0);
    }
}
TEST(Count_Words, Test_One) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("d");
    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}
TEST(Count_Words, Test_SpacesBeforeLetter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("            f");
    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}
TEST(Count_Words, Test_SpacesAfterLetter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("f     ");
    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}
TEST(Count_Words, Test_SpacesAroundLetter) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int res = countWords("     f     ");
    if (rank == 0) {
        ASSERT_EQ(res, 1);
    }
}
TEST(Count_Words, Test_Random) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int count = gen()%30;
    int res = countWords(getRandomString(count));
    if (rank == 0) {
        ASSERT_EQ(res, count);
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
