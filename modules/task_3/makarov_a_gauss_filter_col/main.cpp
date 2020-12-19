// Copyright 2020 Makarov Alexander
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./gauss_filter_col.h"

TEST(GaussFilter, 5x10_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 5;
    unsigned int h = 10;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(GaussFilter, 100x200_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 100;
    unsigned int h = 200;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(GaussFilter, 300x200_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 300;
    unsigned int h = 200;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(GaussFilter, 1000x2000_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 1000;
    unsigned int h = 2000;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(GaussFilter, 3000x2000_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 3000;
    unsigned int h = 2000;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
    }
}

TEST(GaussFilter, 5000x5000_generation_test) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double sigma = 1.;
    unsigned int radius = 1;
    unsigned int w = 5000;
    unsigned int h = 5000;
    double start_time, end_time;
    std::vector<unsigned int> image;
    if (rank == 0) {
        image = generate_image(w, h);
    }
    if (rank == 0) start_time = MPI_Wtime();
    std::vector<unsigned int> result_par = gaussFilterParallel(image, w, h,
                                                                sigma, radius);
    if (rank == 0) end_time = MPI_Wtime();
    if (rank == 0) {
        double parr_time = end_time - start_time;
        start_time = MPI_Wtime();
        std::vector<unsigned int> result_seq = gaussFilterSequential(image,
                                                        w, h, sigma, radius);
        end_time = MPI_Wtime();
        double seq_time = end_time - start_time;
        std::cout << "Sequential time = " << seq_time << " s" << std::endl;
        std::cout << "Parallel time = " << parr_time << " s" << std::endl;
        ASSERT_EQ(result_par, result_seq);
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
