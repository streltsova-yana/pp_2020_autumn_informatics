// Copyright 2020 Yurin Stanislav
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <random>
#include <iostream>
#include "./shortest_path_by_Moore.h"

TEST(Shortest_Paths_By_Moore, Rows_20_Min_neg5_Max_5) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_of_rows = 20;
    int min_val_of_weight = -5;
    int max_val_of_weight = 5;


    std::vector<int> weight_matrix(num_of_rows * num_of_rows);
    weight_matrix = getRandomWeightMatrix(num_of_rows, min_val_of_weight, max_val_of_weight);

    int start_vert_index = 0, end_vert_index = num_of_rows - 1;

    std::vector<int> shortest_path_traj_seq, shortest_path_traj_par;

    // double time1, time2;
    if (rank == 0) {
        // time1 = MPI_Wtime();
        shortest_path_traj_seq = getSequentialShortestPath(weight_matrix, num_of_rows,
                                                            start_vert_index,
                                                            end_vert_index);
        // time2 = MPI_Wtime();
        // std::cout << "seq_time = " << time2 - time1 << std::endl;
    }

    // if (rank == 0)
    //     time1 = MPI_Wtime();

    shortest_path_traj_par = getParallelShortestPath(weight_matrix, num_of_rows,
                                                        start_vert_index,
                                                        end_vert_index);

    // if (rank == 0) {
    //     time2 = MPI_Wtime();
    //     std::cout << "par_time = "  << time2 - time1 << std::endl;
    // }

    if (rank == 0) {
        ASSERT_EQ(shortest_path_traj_seq, shortest_path_traj_par);
    }
}

TEST(Shortest_Paths_By_Moore, Rows_43_Min_neg14_Max_19) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_of_rows = 43;
    int min_val_of_weight = -14;
    int max_val_of_weight = 19;


    std::vector<int> weight_matrix(num_of_rows * num_of_rows);
    weight_matrix = getRandomWeightMatrix(num_of_rows, min_val_of_weight, max_val_of_weight);

    int start_vert_index = 0, end_vert_index = num_of_rows - 1;

    std::vector<int> shortest_path_traj_seq, shortest_path_traj_par;

    // double time1, time2;
    if (rank == 0) {
        // time1 = MPI_Wtime();
        shortest_path_traj_seq = getSequentialShortestPath(weight_matrix, num_of_rows,
                                                            start_vert_index,
                                                            end_vert_index);
        // time2 = MPI_Wtime();
        // std::cout << "seq_time = " << time2 - time1 << std::endl;
    }

    // if (rank == 0)
    //     time1 = MPI_Wtime();

    shortest_path_traj_par = getParallelShortestPath(weight_matrix, num_of_rows,
                                                        start_vert_index,
                                                        end_vert_index);

    // if (rank == 0) {
    //     time2 = MPI_Wtime();
    //     std::cout << "par_time = "  << time2 - time1 << std::endl;
    // }

    if (rank == 0) {
        ASSERT_EQ(shortest_path_traj_seq, shortest_path_traj_par);
    }
}

TEST(Shortest_Paths_By_Moore, Rows_67_Min_neg35_Max_52) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_of_rows = 43;
    int min_val_of_weight = -14;
    int max_val_of_weight = 19;


    std::vector<int> weight_matrix(num_of_rows * num_of_rows);
    weight_matrix = getRandomWeightMatrix(num_of_rows, min_val_of_weight, max_val_of_weight);

    int start_vert_index = 0, end_vert_index = num_of_rows - 1;

    std::vector<int> shortest_path_traj_seq, shortest_path_traj_par;

    // double time1, time2;
    if (rank == 0) {
        // time1 = MPI_Wtime();
        shortest_path_traj_seq = getSequentialShortestPath(weight_matrix, num_of_rows,
                                                            start_vert_index,
                                                            end_vert_index);
        // time2 = MPI_Wtime();
        // std::cout << "seq_time = " << time2 - time1 << std::endl;
    }

    // if (rank == 0)
    //     time1 = MPI_Wtime();

    shortest_path_traj_par = getParallelShortestPath(weight_matrix, num_of_rows,
                                                        start_vert_index,
                                                        end_vert_index);

    // if (rank == 0) {
    //     time2 = MPI_Wtime();
    //     std::cout << "par_time = "  << time2 - time1 << std::endl;
    // }

    if (rank == 0) {
        ASSERT_EQ(shortest_path_traj_seq, shortest_path_traj_par);
    }
}

TEST(Shortest_Paths_By_Moore, Rows_89_Min_neg74_Max_57) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_of_rows = 43;
    int min_val_of_weight = -14;
    int max_val_of_weight = 19;


    std::vector<int> weight_matrix(num_of_rows * num_of_rows);
    weight_matrix = getRandomWeightMatrix(num_of_rows, min_val_of_weight, max_val_of_weight);

    int start_vert_index = 0, end_vert_index = num_of_rows - 1;

    std::vector<int> shortest_path_traj_seq, shortest_path_traj_par;

    // double time1, time2;
    if (rank == 0) {
        // time1 = MPI_Wtime();
        shortest_path_traj_seq = getSequentialShortestPath(weight_matrix, num_of_rows,
                                                            start_vert_index,
                                                            end_vert_index);
        // time2 = MPI_Wtime();
        // std::cout << "seq_time = " << time2 - time1 << std::endl;
    }

    // if (rank == 0)
    //     time1 = MPI_Wtime();

    shortest_path_traj_par = getParallelShortestPath(weight_matrix, num_of_rows,
                                                        start_vert_index,
                                                        end_vert_index);

    // if (rank == 0) {
    //     time2 = MPI_Wtime();
    //     std::cout << "par_time = "  << time2 - time1 << std::endl;
    // }

    if (rank == 0) {
        ASSERT_EQ(shortest_path_traj_seq, shortest_path_traj_par);
    }
}

TEST(Shortest_Paths_By_Moore, Rows_100_Min_neg49_Max_31) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int num_of_rows = 100;
    int min_val_of_weight = -49;
    int max_val_of_weight = 31;


    std::vector<int> weight_matrix(num_of_rows * num_of_rows);
    weight_matrix = getRandomWeightMatrix(num_of_rows, min_val_of_weight, max_val_of_weight);

    int start_vert_index = 0, end_vert_index = num_of_rows - 1;

    std::vector<int> shortest_path_traj_seq, shortest_path_traj_par;

    // double time1, time2;
    if (rank == 0) {
        // time1 = MPI_Wtime();
        shortest_path_traj_seq = getSequentialShortestPath(weight_matrix, num_of_rows,
                                                            start_vert_index,
                                                            end_vert_index);
        // time2 = MPI_Wtime();
        // std::cout << "seq_time = " << time2 - time1 << std::endl;
    }

    // if (rank == 0)
    //     time1 = MPI_Wtime();

    shortest_path_traj_par = getParallelShortestPath(weight_matrix, num_of_rows,
                                                        start_vert_index,
                                                        end_vert_index);

    // if (rank == 0) {
    //     time2 = MPI_Wtime();
    //     std::cout << "par_time = "  << time2 - time1 << std::endl;
    // }

    if (rank == 0) {
        ASSERT_EQ(shortest_path_traj_seq, shortest_path_traj_par);
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
