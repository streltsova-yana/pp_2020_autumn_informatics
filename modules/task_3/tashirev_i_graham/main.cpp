// Copyright 2020 Tashirev Ivan
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <iostream>
#include <algorithm>
#include <vector>
#include "./tashirev_i_graham.h"

TEST(Parallel_Operations_MPI, Test1) {
    int proc_size, proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    std::vector<pixel> input_image, p_out_points, s_out_points;
    int side = 10;
    int points_in_image = 0;

    if (proc == 0) {
        input_image = get_random_image(side, side, &points_in_image);
    }

    MPI_Bcast(&points_in_image, 1, MPI_INT, 0, MPI_COMM_WORLD);
    p_out_points = greh_parallel(input_image, points_in_image);

    if (proc == 0) {
        s_out_points = greh_sequential(input_image);
        ASSERT_EQ(p_out_points, s_out_points);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test2) {
    int proc_size, proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    std::vector<pixel> input_image, p_out_points, s_out_points;
    int side = 50;
    int points_in_image = 0;

    if (proc == 0) {
        input_image = get_random_image(side, side, &points_in_image);
    }

    MPI_Bcast(&points_in_image, 1, MPI_INT, 0, MPI_COMM_WORLD);
    p_out_points = greh_parallel(input_image, points_in_image);

    if (proc == 0) {
        s_out_points = greh_sequential(input_image);
        ASSERT_EQ(p_out_points, s_out_points);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test3) {
    int proc_size, proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    std::vector<pixel> input_image, p_out_points, s_out_points;
    int side = 100;
    int points_in_image = 0;

    if (proc == 0) {
        input_image = get_random_image(side, side, &points_in_image);
    }

    MPI_Bcast(&points_in_image, 1, MPI_INT, 0, MPI_COMM_WORLD);
    p_out_points = greh_parallel(input_image, points_in_image);

    if (proc == 0) {
        s_out_points = greh_sequential(input_image);
        ASSERT_EQ(p_out_points, s_out_points);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test4) {
    int proc_size, proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    std::vector<pixel> input_image, p_out_points, s_out_points;
    int side = 1000;
    int points_in_image = 0;

    if (proc == 0) {
        input_image = get_random_image(side, side, &points_in_image);
    }

    MPI_Bcast(&points_in_image, 1, MPI_INT, 0, MPI_COMM_WORLD);
    p_out_points = greh_parallel(input_image, points_in_image);

    if (proc == 0) {
        s_out_points = greh_sequential(input_image);
        ASSERT_EQ(p_out_points, s_out_points);
    }
    MPI_Barrier(MPI_COMM_WORLD);
}

TEST(Parallel_Operations_MPI, Test5) {
    int proc_size, proc;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc);
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    std::vector<pixel> input_image, p_out_points, s_out_points;
    int side = 2000;
    int points_in_image = 0;

    if (proc == 0) {
        input_image = get_random_image(side, side, &points_in_image);
    }

    MPI_Bcast(&points_in_image, 1, MPI_INT, 0, MPI_COMM_WORLD);
    p_out_points = greh_parallel(input_image, points_in_image);

    if (proc == 0) {
        s_out_points = greh_sequential(input_image);
        ASSERT_EQ(p_out_points, s_out_points);
    }
    MPI_Barrier(MPI_COMM_WORLD);
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
