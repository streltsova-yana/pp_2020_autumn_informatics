// Copyright 2020 Novozhilova Ekaterina
#include <mpi.h>
#include <vector>
#include <iostream>
#include <random>
#include <ctime>
#include "../../../modules/task_3/novozhilova_e_cannon_s_algorithm/cannon_s_algorithm.h"

std::vector<double> GenMatrix(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<double>(time(0)));
    std::vector<double> m(size*size);
    for (int i = 0; i < size*size; i++) {
        m[i] = gen() % 1000;
        m[i] = m[i] / 100.0;
    }
    return m;
}
std::vector<double> SeqMultiply(std::vector<double> A, std::vector<double> B, int size) {
    std::vector<double> C(size*size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            for (int k = 0; k < size; k++) {
                C[i * size + j] += A[i * size + k] * B[k * size + j];
            }
        }
    }
    return C;
}
std::vector<double> CannonAlgorithm(std::vector<double> A, std::vector<double> B, int size) {
    int World_size, World_rank;
    int block_dim = size / 2;
    int square = block_dim * block_dim;
    std::vector<double> blockA(square), blockB(square), blockC(square);
    std::vector<double> C(size*size);
    std::vector<double> local_C(square);
    int right_neigh, left_neigh, upper_neigh, lower_neigh;
    MPI_Comm_size(MPI_COMM_WORLD, &World_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &World_rank);
    int blockA_tag = 0;
    int blockB_tag = 1;
    int localC_tag = 2;
        if (World_size >= 4) {
            MPI_Group MPI_NEW_GROUP;
            MPI_Comm MPI_NEW_COMM;
            MPI_Group MPI_GROUP_WORLD;
            MPI_Comm_group(MPI_COMM_WORLD, &MPI_GROUP_WORLD);
            int process_ranks[4];

            for (int i = 0; i < 4; i++) {
                process_ranks[i] = i;
            }
            MPI_Group_incl(MPI_GROUP_WORLD, 4, process_ranks, &MPI_NEW_GROUP);
            MPI_Comm_create(MPI_COMM_WORLD, MPI_NEW_GROUP, &MPI_NEW_COMM);
            if (World_rank < 4) {
                int Comm_size, Comm_rank;
                MPI_Comm_size(MPI_NEW_COMM, &Comm_size);
                MPI_Comm_rank(MPI_NEW_COMM, &Comm_rank);
                MPI_Comm MPI_CART_COMM;
                int dim_size[2];
                dim_size[0] = 2;
                dim_size[1] = 2;
                int periods[2];
                periods[0] = 1;
                periods[1] = 1;
                MPI_Cart_create(MPI_NEW_COMM, 2, dim_size, periods, true, &MPI_CART_COMM);
                int coords[2];
                MPI_Cart_coords(MPI_CART_COMM, Comm_rank, 2, coords);

                if (Comm_rank == 0) {
                    std::vector<double> tmpBlockA(square), tmpBlockB(square);
                    MPI_Cart_coords(MPI_CART_COMM, Comm_rank, 2, coords);
                    int A_counter, B_counter, counter;
                    counter = 0;
                    for (int i = 0; i < 2; i++) {
                        for (int j = 0; j < 2; j++) {
                            A_counter = 0;
                            B_counter = 0;
                            int tmp[2];
                            tmp[0] = i;
                            tmp[1] = j;
                            int rank;
                            MPI_Cart_rank(MPI_CART_COMM, tmp, &rank);

                            for (int k = 0; k < square; k++) {
                                blockA[k] = A[k + A_counter * block_dim + counter * block_dim];
                                if ((k + 1) % block_dim == 0) {
                                    A_counter += 1;
                                }
                            }
                            for (int k = 0; k < square; k++) {
                                blockB[k] = B[k + B_counter * block_dim + counter * block_dim];
                                if ((k + 1) % block_dim == 0) {
                                    B_counter += 1;
                                }
                            }
                            if (rank == 0) {
                                for (int i = 0; i < square; i++) {
                                    tmpBlockA[i] = blockA[i];
                                    tmpBlockB[i] = blockB[i];
                                }
                            } else {
                                MPI_Send(&blockA[0], square, MPI_DOUBLE, rank, blockA_tag, MPI_CART_COMM);
                                MPI_Send(&blockB[0], square, MPI_DOUBLE, rank, blockB_tag, MPI_CART_COMM);
                            }
                            counter++;
                        }
                        counter = block_dim * 2;
                    }
                    for (int i = 0; i < square; i++) {
                        blockA[i] = tmpBlockA[i];
                        blockB[i] = tmpBlockB[i];
                    }
                }
                if (Comm_rank != 0) {
                    MPI_Recv(&blockA[0], square, MPI_DOUBLE, 0, blockA_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                    MPI_Recv(&blockB[0], square, MPI_DOUBLE, 0, blockB_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                }
                MPI_Cart_shift(MPI_CART_COMM, 0, -1, &lower_neigh, &upper_neigh);
                MPI_Cart_shift(MPI_CART_COMM, 1, -1, &right_neigh, &left_neigh);

                for (int i = 0; i < square; i++) {
                    local_C[i] = 0;
                }
                if (coords[0] == 1) {
                    MPI_Sendrecv_replace(&blockA[0], square, MPI_DOUBLE, left_neigh, blockA_tag,
                        right_neigh, blockA_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                }
                if (coords[1] == 1) {
                    MPI_Sendrecv_replace(&blockB[0], square, MPI_DOUBLE, upper_neigh, blockB_tag,
                        lower_neigh, blockB_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                }
                blockC = SeqMultiply(blockA, blockB, block_dim);
                for (int i = 0; i < square; i++) {
                    local_C[i] += blockC[i];
                }
                MPI_Sendrecv_replace(&blockA[0], square, MPI_DOUBLE, left_neigh, blockA_tag,
                    right_neigh, blockA_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                MPI_Sendrecv_replace(&blockB[0], square, MPI_DOUBLE, upper_neigh, blockB_tag,
                    lower_neigh, blockB_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                blockC = SeqMultiply(blockA, blockB, block_dim);
                for (int i = 0; i < square; i++) {
                    local_C[i] += blockC[i];
                }
                if (Comm_rank == 0) {
                    std::vector<double> arr(square);
                    int C_counter;
                    for (int i = 1; i < 4; i++) {
                        MPI_Recv(&arr[0], square, MPI_DOUBLE, i, localC_tag, MPI_CART_COMM, MPI_STATUS_IGNORE);
                        int _coords[2];
                        MPI_Cart_coords(MPI_CART_COMM, i, 2, _coords);
                        C_counter = 0;
                        for (int k = 0; k < square; k++) {
                            C[k + _coords[1] * block_dim + _coords[0] * block_dim*block_dim * 2 + C_counter] = arr[k];
                            if ((k + 1) % block_dim == 0) {
                                C_counter += block_dim;
                            }
                        }
                    }
                    C_counter = 0;
                    for (int k = 0; k < square; k++) {
                        C[k + C_counter] = local_C[k];
                        if ((k + 1) % block_dim == 0) {
                            C_counter += block_dim;
                        }
                    }
                } else {
                    MPI_Send(&local_C[0], square, MPI_DOUBLE, 0, localC_tag, MPI_CART_COMM);
                }
            }
        }
        return C;
    }
