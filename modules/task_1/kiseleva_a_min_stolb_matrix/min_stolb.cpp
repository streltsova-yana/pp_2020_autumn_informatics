// Copyright 2020 Kiseleva Anastasia
#include "../../../modules/task_1/kiseleva_a_min_stolb_matrix/min_stolb.h"
#include <mpi.h>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> RandomMatrix(int str, int stlb) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<std::vector<int>> matrix(str*stlb);
    std::vector<int> stolb(stlb);
    for (int i = 0; i < str; i++) {
        stolb.clear();
        for (int j = 0; j < stlb; j++) {
            stolb.push_back(gen() % 1000);
        }
        matrix[i] = stolb;
    }
    return matrix;
}

std::vector<int> VVconvertV(std::vector<std::vector<int>> matrix, int str, int stlb) {
    std::vector<int> linm(str * stlb);
    for (int i = 0; i < stlb; i++) {
        for (int j = 0; j < str; j++) {
            linm[i*str + j] = matrix[j][i];
        }
    }
    return linm;
}

int min_vector(std::vector<int> linm) {
    int res;
    res = *std::min_element(linm.begin(), linm.end());
    return res;
}

std::vector<int> min_posled(std::vector<int> linm, int str, int stlb) {
    std::vector<int> res(stlb);
    for (int i = 0; i < stlb; i++) {
        res[i] = *std::min_element(linm.begin() + str * i, linm.begin() + str * (i + 1));
    }
    return res;
}

std::vector<int> Min(std::vector<int> linm, int str, int stlb) {
    int size, rank;
    std::vector<int> res(stlb);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size > stlb - 1) {
        MPI_Comm comm_world;
        MPI_Group group_world;
        comm_world = MPI_COMM_WORLD;
        MPI_Comm_group(comm_world, &group_world);
        int *members = new int[stlb];
        for (int i = 0; i < stlb; i++) {
            members[i] = i;
        }
        MPI_Group newworld;
        MPI_Group_incl(group_world, stlb, members, &newworld);
        MPI_Comm comm_newworld;
        MPI_Comm_create(comm_world, newworld, &comm_newworld);
        MPI_Group_rank(newworld, &rank);
        int size_;
        MPI_Group_size(newworld, &size_);
        std::vector<int> local(str);
        if (comm_newworld != MPI_COMM_NULL) {
            if (rank == 0) {
                local = std::vector<int>(linm.begin(), linm.begin() + str);
                for (int i = 1; i < size_; i++) {
                    MPI_Send(linm.data() + i * str, str, MPI_INT, i, 0, comm_newworld);
                }
            } else {
                MPI_Status status;
                MPI_Recv(local.data(), str, MPI_INT, 0, 0, comm_newworld, &status);
            }
            int local_min;
            local_min = min_vector(local);
            if (rank != 0) {
                MPI_Send(&local_min, 1, MPI_INT, 0, 0, comm_newworld);
            }
            if (rank == 0) {
                res[0] = local_min;
                for (int i = 1; i < size_; i++) {
                    MPI_Status status;
                    MPI_Recv(res.data() + i, 1, MPI_INT, i, 0, comm_newworld, &status);
                }
            }
        }
    } else {
            MPI_Comm_rank(MPI_COMM_WORLD, &rank);
            int delta = stlb / size;
            int ost = stlb % size;
            int local_count;
            int local_stlb;
            if (rank == 0) {
                local_count = delta * str + ost * str;
                local_stlb = delta + ost;
            } else {
                local_count = delta * str;
                local_stlb = delta;
            }
            std::vector<int> local_(local_count);
            std::vector<int> local_min(local_stlb);
            if (rank == 0) {
                local_ = std::vector<int>(linm.begin(), linm.begin() + local_count);
                local_min = min_posled(local_, str, local_stlb);
                for (int i = 0; i < delta + ost; i++) {
                    res[i] = local_min[i];
                }
                for (int i = 1; i < size; i++) {
                    MPI_Send(linm.data() + i * delta * str + ost * str, delta*str, MPI_INT, i, 0, MPI_COMM_WORLD);
                }
            } else {
                MPI_Status status;
                MPI_Recv(local_.data(), local_count, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            }
            local_min = min_posled(local_, str, local_stlb);
            if (rank != 0) {
                MPI_Send(local_min.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
            if (rank == 0) {
                for (int i = 1; i < size; i++) {
                    MPI_Status status;
                    MPI_Recv(res.data() + ost + delta * i, delta, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
                }
            }
    }
    return res;
}

