// Copyright 2020 Makarychev Sergey
#include "../../../modules/task_2/makarychev_s_bubble_sort/bubble_sort.h"
#include <mpi.h>
#include <iostream>
#include <algorithm>
#include <vector>

std::vector<int> getRandomVector(int sizeVec) {
    if (sizeVec < 0)
        throw "Wrong vector size";
    std::vector<int> rVec(sizeVec);
    std::random_device rd;
    std::mt19937 mersenne(rd());
    std::uniform_int_distribution<> uid(-1000, 2000);
    for (int i = 0; i < sizeVec; i++)
        rVec[i] = uid(mersenne);
    return rVec;
}

void mergeOrderVec(int* vec1, int* vec2, bool flag, int size1, int size2) {
    std::vector<int> resVec(size1 + size2);
    int i = 0, s = 0, j = 0;
    while (i < size1 && j < size2) {
        if (vec1[i] < vec2[j])
            resVec[s++] = vec1[i++];
        else
            resVec[s++] = vec2[j++];
    }
    while (i < size1)
        resVec[s++] = vec1[i++];
    while (j < size2)
        resVec[s++] = vec2[j++];
    i = j = s = 0;
    while (flag && i < size1)
        vec1[i++] = resVec[s++];
    s = size1;
    while (!flag && j < size2)
        vec2[j++] = resVec[s++];
}

void sortVecParallel(int* vec, int sizeVec, double* time) {
    if (sizeVec < 0)
        throw "Wrong vector size";
    int ProcNum, ProcRank;
    double beginT = 0;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int blockSize = sizeVec / ProcNum;
    int rem = sizeVec % ProcNum;
    MPI_Status status;
    if (ProcRank == 0) {
        beginT = MPI_Wtime();
        for (int p = 1; p < ProcNum; p++) {
            MPI_Send(vec + rem + p * blockSize, blockSize, MPI_INT, p, 0, MPI_COMM_WORLD);
        }
    }
    int locSize;
    if (ProcRank == 0) {
        locSize = rem + blockSize;
    } else {
        locSize = blockSize;
    }
    std::vector<int> locVec(locSize);
    if (ProcRank == 0) {
        locVec = std::vector<int>(vec, vec + locSize);
    } else {
        MPI_Recv(locVec.data(), locSize, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    std::sort(locVec.begin(), locVec.begin() + locSize);
    MPI_Barrier(MPI_COMM_WORLD);
    std::vector<int> secondVec(blockSize);
    int secondSize = blockSize;
    bool flag = true;
    if (ProcNum != 1) {
        for (int i = 0; i < ProcNum; i++) {
            if (i % 2 != 0) {
                if (ProcRank % 2 != 0) {
                    if (ProcRank < ProcNum - 1) {
                        flag = true;
                        MPI_Send(&locSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(locVec.data(), locSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
                        MPI_Recv(&secondSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
                        secondVec.resize(secondSize);
                        MPI_Recv(secondVec.data(), secondSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
                        mergeOrderVec(locVec.data(), secondVec.data(), flag, locSize, secondSize);
                    }
                } else {
                    if (ProcRank > 0) {
                        flag = false;
                        MPI_Send(&locSize, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                        MPI_Send(locVec.data(), locSize, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                        MPI_Recv(&secondSize, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, &status);
                        secondVec.resize(secondSize);
                        MPI_Recv(secondVec.data(), secondSize, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, &status);
                        mergeOrderVec(secondVec.data(), locVec.data(), flag, secondSize, locSize);
                    }
                }
            } else {
                if (ProcRank % 2 == 0) {
                    if (ProcRank < ProcNum - 1) {
                        flag = true;
                        MPI_Send(&locSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
                        MPI_Send(locVec.data(), locSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD);
                        MPI_Recv(&secondSize, 1, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
                        secondVec.resize(secondSize);
                        MPI_Recv(secondVec.data(), secondSize, MPI_INT, ProcRank + 1, 0, MPI_COMM_WORLD, &status);
                        mergeOrderVec(locVec.data(), secondVec.data(), flag, locSize, secondSize);
                    }
                } else {
                    flag = false;
                    MPI_Send(&locSize, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                    MPI_Send(locVec.data(), locSize, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD);
                    MPI_Recv(&secondSize, 1, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, &status);
                    secondVec.resize(secondSize);
                    MPI_Recv(secondVec.data(), secondSize, MPI_INT, ProcRank - 1, 0, MPI_COMM_WORLD, &status);
                    mergeOrderVec(secondVec.data(), locVec.data(), flag, secondSize, locSize);
                }
            }
        }
    }
    int* revcount = new int[ProcNum];
    int* disps = new int[ProcNum];
    disps[0] = 0;
    revcount[0] = blockSize + rem;
    for (int i = 1; i < ProcNum; i++) {
        revcount[i] = blockSize;
        disps[i] = blockSize * i + rem;
    }
    MPI_Gatherv(locVec.data(), revcount[ProcRank], MPI_INT, vec, revcount, disps, MPI_INT, 0, MPI_COMM_WORLD);
    if (ProcRank == 0) {
        *time = MPI_Wtime() - beginT;
        std::cout << "MPI sort time:  " << MPI_Wtime() - beginT << std::endl;
    }
}
