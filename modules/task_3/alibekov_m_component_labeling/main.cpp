// Copyright 2020 Alibekov Murad
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <vector>
#include <utility>
#include "./component_labeling.h"

TEST(Component_Labeling, my_image_9x11_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 9, width = 11;
    std::vector<int> image(width * height);
    std::vector<int> right_result(width * height);

    if (proc_rank == 0) {
        image = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0,
            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        right_result = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0,
            0, 1, 1, 0, 0, 3, 0, 0, 0, 2, 0,
            0, 1, 0, 0, 3, 3, 3, 0, 2, 2, 0,
            0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0,
            0, 0, 4, 0, 0, 3, 0, 0, 5, 5, 0,
            0, 4, 4, 0, 0, 0, 0, 5, 5, 5, 0,
            0, 4, 4, 0, 0, 0, 0, 0, 5, 5, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    std::pair<std::vector<int>, int> result = component_labeling_parallel(image, width, height);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        std::vector<int> new_result = remarking(result.first, width, height);
        for (int i = 0; i < height * width; i++)
            ASSERT_EQ(right_result[i], new_result[i]);
    }
}


TEST(Component_Labeling, my_image_9x11_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 9, width = 11;
    std::vector<int> image(width * height);
    std::vector<int> right_result(width * height);

    if (proc_rank == 0) {
        image = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0,
            0, 1, 0, 0, 1, 1, 1, 0, 1, 1, 0,
            0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 1, 1, 1, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        right_result = {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 1, 1, 0, 0, 0, 0, 0, 2, 2, 0,
            0, 1, 1, 0, 0, 3, 0, 0, 0, 2, 0,
            0, 1, 0, 0, 3, 3, 3, 0, 2, 2, 0,
            0, 0, 0, 0, 3, 3, 3, 0, 0, 0, 0,
            0, 0, 4, 0, 0, 3, 0, 0, 5, 5, 0,
            0, 4, 4, 0, 0, 0, 0, 5, 5, 5, 0,
            0, 4, 4, 0, 0, 0, 0, 0, 5, 5, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        start_time = MPI_Wtime();
        std::pair<std::vector<int>, int> result = component_labeling_sequential(image, width, height);
        end_time = MPI_Wtime();

        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        std::vector<int> new_result = remarking(result.first, width, height);
        for (int i = 0; i < height * width; i++)
            ASSERT_EQ(right_result[i], new_result[i]);
    }
}


TEST(Component_Labeling, my_image_10x9_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 10, width = 9;
    std::vector<int> image_M(width * height);

    if (proc_rank == 0) {
        image_M = {
            0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0,
        };
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    std::pair<std::vector<int>, int> result = component_labeling_parallel(image_M, width, height);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        ASSERT_EQ(result.second, 1);
    }
}


TEST(Component_Labeling, my_image_10x9_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 10, width = 9;
    std::vector<int> image_M(width * height);

    if (proc_rank == 0) {
        image_M = {
            0, 0, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 1, 0, 0, 0, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 0, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 1, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 1, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            1, 1, 0, 0, 0, 0, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        start_time = MPI_Wtime();
        std::pair<std::vector<int>, int> result = component_labeling_sequential(image_M, width, height);
        end_time = MPI_Wtime();

        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        ASSERT_EQ(result.second, 1);
    }
}


TEST(Component_Labeling, my_image_18x22_parallel) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 18, width = 22;
    std::vector<int> image_MMMM(width * height);

    if (proc_rank == 0) {
        image_MMMM = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,  0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,  0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };
    }

    if (proc_rank == 0) start_time = MPI_Wtime();
    std::pair<std::vector<int>, int> result = component_labeling_parallel(image_MMMM, width, height);
    if (proc_rank == 0) end_time = MPI_Wtime();

    if (proc_rank == 0) {
        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        ASSERT_EQ(result.second, 4);
    }
}


TEST(Component_Labeling, my_image_18x22_sequential) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time, end_time;

    int height = 18, width = 22;
    std::vector<int> image_MMMM(width * height);

    if (proc_rank == 0) {
        image_MMMM = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,  0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,  0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0,
        0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
        0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,  0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,  0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        };

        start_time = MPI_Wtime();
        std::pair<std::vector<int>, int> result = component_labeling_sequential(image_MMMM, width, height);
        end_time = MPI_Wtime();

        printf("\tTime  = %f\n", end_time - start_time);
        printf("\tCount of components: %i\n\n", result.second);

        ASSERT_EQ(result.second, 4);
    }
}


TEST(Component_Labeling, random_image_1080x1920) {
    int proc_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    double start_time_par, end_time_par, start_time_seq, end_time_seq;

    int height = 1080, width = 1920;
    std::vector<int> generated_image(width * height);
    std::pair<std::vector<int>, int> result_seq;

    if (proc_rank == 0) {
        generated_image = generate_random_image(width, height);
    }

    if (proc_rank == 0) {
        start_time_seq = MPI_Wtime();
        result_seq = component_labeling_sequential(generated_image, width, height);
        end_time_seq = MPI_Wtime();
    }

    if (proc_rank == 0) start_time_par = MPI_Wtime();
    std::pair<std::vector<int>, int> result_par = component_labeling_parallel(generated_image, width, height);
    if (proc_rank == 0) end_time_par = MPI_Wtime();

    if (proc_rank == 0) {
        int component_counts_seq = result_seq.second;
        printf("\tTime (sequential) = %f\n", end_time_seq - start_time_seq);
        printf("\tCount of components (sequential): %i\n\n", component_counts_seq);

        int component_counts_par = result_par.second;
        printf("\tTime (parallel) = %f\n", end_time_par - start_time_par);
        printf("\tCount of components (parallel): %i\n", component_counts_par);

        ASSERT_EQ(component_counts_seq, component_counts_par);
    }
}


int main(int argc, char* argv[]) {
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
