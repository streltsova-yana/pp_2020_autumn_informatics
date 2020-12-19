// Copyright 2020 Solovev Aleksandr
#include <mpi.h>
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_3/solovev_a_building_image/building_image.h"

TEST(MyAlgos, Test_Data_Set_250) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 250;
    int width = 250;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = 1;
        }
    }

    std::vector<Point> points_parallel = interpriate_basic(image, height, width);
    std::vector<Point> points_sequence = points_parallel;
    std::vector<int> points_parallel_x;
    std::vector<int> points_parallel_y;
    std::vector<int> points_sequence_x;
    std::vector<int> points_sequence_y;

    points_parallel = buildConvexHullParallel(points_parallel);


    if (rank == 0) {
        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_parallel_x.emplace_back(points_parallel[i].x);
            points_parallel_y.emplace_back(points_parallel[i].y);
        }

        points_sequence = buildConvexHull(points_sequence);

        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_sequence_x.emplace_back(points_parallel[i].x);
            points_sequence_y.emplace_back(points_parallel[i].y);
        }


        ASSERT_EQ(points_parallel_x, points_sequence_x);
        ASSERT_EQ(points_parallel_y, points_sequence_y);
    }
}

TEST(MyAlgos2, Test_Data_Set_500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 500;
    int width = 500;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            image[i][j] = 1;
        }
    }

    std::vector<Point> points_parallel = interpriate_basic(image, height, width);
    std::vector<Point> points_sequence = points_parallel;
    std::vector<int> points_parallel_x;
    std::vector<int> points_parallel_y;
    std::vector<int> points_sequence_x;
    std::vector<int> points_sequence_y;

    points_parallel = buildConvexHullParallel(points_parallel);


    if (rank == 0) {
        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_parallel_x.emplace_back(points_parallel[i].x);
            points_parallel_y.emplace_back(points_parallel[i].y);
        }

        points_sequence = buildConvexHull(points_sequence);

        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_sequence_x.emplace_back(points_parallel[i].x);
            points_sequence_y.emplace_back(points_parallel[i].y);
        }


        ASSERT_EQ(points_parallel_x, points_sequence_x);
        ASSERT_EQ(points_parallel_y, points_sequence_y);
    }
}

TEST(MyAlgos3, Test_Data_Set_1000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 1000;
    int width = 1000;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = 1;
        }
    }

    std::vector<Point> points_parallel = interpriate_basic(image, height, width);
    std::vector<Point> points_sequence = points_parallel;
    std::vector<int> points_parallel_x;
    std::vector<int> points_parallel_y;
    std::vector<int> points_sequence_x;
    std::vector<int> points_sequence_y;

    points_parallel = buildConvexHullParallel(points_parallel);


    if (rank == 0) {
        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_parallel_x.emplace_back(points_parallel[i].x);
            points_parallel_y.emplace_back(points_parallel[i].y);
        }

        points_sequence = buildConvexHull(points_sequence);

        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_sequence_x.emplace_back(points_parallel[i].x);
            points_sequence_y.emplace_back(points_parallel[i].y);
        }


        ASSERT_EQ(points_parallel_x, points_sequence_x);
        ASSERT_EQ(points_parallel_y, points_sequence_y);
    }
}

TEST(MyAlgos4, Test_Data_Set_2000) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 2000;
    int width = 2000;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = 1;
        }
    }

    std::vector<Point> points_parallel = interpriate_basic(image, height, width);
    std::vector<Point> points_sequence = points_parallel;
    std::vector<int> points_parallel_x;
    std::vector<int> points_parallel_y;
    std::vector<int> points_sequence_x;
    std::vector<int> points_sequence_y;

    points_parallel = buildConvexHullParallel(points_parallel);


    if (rank == 0) {
        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_parallel_x.emplace_back(points_parallel[i].x);
            points_parallel_y.emplace_back(points_parallel[i].y);
        }

        points_sequence = buildConvexHull(points_sequence);

        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_sequence_x.emplace_back(points_parallel[i].x);
            points_sequence_y.emplace_back(points_parallel[i].y);
        }


        ASSERT_EQ(points_parallel_x, points_sequence_x);
        ASSERT_EQ(points_parallel_y, points_sequence_y);
    }
}

TEST(MyAlgos5, Test_Data_Set_2500) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int height = 2500;
    int width = 2500;
    int ** image = new int*[height];
    for (int i = 0; i < height; i++) {
        image[i] = new int[width];
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = 1;
        }
    }

    std::vector<Point> points_parallel = interpriate_basic(image, height, width);
    std::vector<Point> points_sequence = points_parallel;
    std::vector<int> points_parallel_x;
    std::vector<int> points_parallel_y;
    std::vector<int> points_sequence_x;
    std::vector<int> points_sequence_y;

    points_parallel = buildConvexHullParallel(points_parallel);


    if (rank == 0) {
        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_parallel_x.emplace_back(points_parallel[i].x);
            points_parallel_y.emplace_back(points_parallel[i].y);
        }

        points_sequence = buildConvexHull(points_sequence);

        for (size_t i = 0; i < points_parallel.size(); i++) {
            points_sequence_x.emplace_back(points_parallel[i].x);
            points_sequence_y.emplace_back(points_parallel[i].y);
        }


        ASSERT_EQ(points_parallel_x, points_sequence_x);
        ASSERT_EQ(points_parallel_y, points_sequence_y);
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
