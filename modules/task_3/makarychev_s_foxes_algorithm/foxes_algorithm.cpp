// Copyright 2020 Makarychev Sergey
#include "../../../modules/task_3/makarychev_s_foxes_algorithm/foxes_algorithm.h"
#include <mpi.h>
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <limits>
#include <ctime>
#include <cmath>

MPI_Comm GridComm;
MPI_Comm ColComm;
MPI_Comm RowComm;

std::vector<double> getRandomMatrix(int orderM) {
  int sizeM = orderM * orderM;
  if (sizeM < 0)
    throw "Wrong matrix size";
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<> urd(-100, 100);
  std::vector<double> randV(sizeM);
  for (int i = 0; i < sizeM; i++) {
      randV[i] = urd(gen);
  }
  return randV;
}

bool compareMat(const std::vector<double>& matA, const std::vector<double>& matB) {
    for (size_t i = 0; i < matA.size(); i++) {
        if ((std::fabs(matA[i] - matB[i]) >= std::numeric_limits<double>::epsilon() * 1000000000.0))
            return false;
    }
    return true;
}

std::vector<double> seqMult(const std::vector<double>& matA, const std::vector<double>& matB, int size) {
    if (size < 0 || matA.size() != matB.size())
        throw "Wrong matrix size";
    std::vector<double> matC(size * size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            double tmp = 0;
            for (int k = 0; k < size; k++) {
                tmp += matA[i * size + k] * matB[k * size + j];
            }
            matC[i * size + j] += tmp;
        }
    }
    return matC;
}

void createGridCommunicators(int gridSize, int ProcRank, int* gridCoords) {
    std::vector<int> dimSize(2, gridSize);
    std::vector<int> periodic(2, 0);
    std::vector<int> subdims(2);
    MPI_Cart_create(MPI_COMM_WORLD, 2, dimSize.data(), periodic.data(), 0, &GridComm);
    MPI_Cart_coords(GridComm, ProcRank, 2, gridCoords);
    subdims[0] = 0;
    subdims[1] = 1;
    MPI_Cart_sub(GridComm, subdims.data(), &RowComm);
    subdims[0] = 1;
    subdims[1] = 0;
    MPI_Cart_sub(GridComm, subdims.data(), &ColComm);
}


void BlockMult(double* pAblock, double* pBblock, double* pCblock, int blockSize) {
    for (int i = 0; i < blockSize; i++) {
        for (int j = 0; j < blockSize; j++) {
            double tmp = 0;
            for (int k = 0; k < blockSize; k++)
                tmp += pAblock[i * blockSize + k] * pBblock[k * blockSize + j];
            pCblock[i * blockSize + j] += tmp;
        }
    }
}
std::vector<double> foxsAlgorithm(const std::vector<double>& matA, const std::vector<double>& matB, int orderM) {
    if (orderM < 0)
        throw "Wrong matrix size";
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    MPI_Status status;
    int gridSize = sqrt(ProcNum);
    if (gridSize * gridSize != ProcNum)
        throw "Wrong number of processes";
    std::vector<int> gridCoords(2);
    createGridCommunicators(gridSize, ProcRank, gridCoords.data());
    int blockSize;
    if (ProcRank == 0) {
        blockSize = orderM / gridSize;
    }
    MPI_Bcast(&blockSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
    std::vector<double> blockA(blockSize * blockSize);
    std::vector<double> blockB(blockSize * blockSize);
    std::vector<double> blockC(blockSize * blockSize, 0);
    if (ProcRank == 0) {
        for (int i = 0; i < blockSize; i++) {
            for (int j = 0; j < blockSize; j++) {
                blockA[i * blockSize + j] = matA[i * orderM + j];
                blockB[i * blockSize + j] = matB[i * orderM + j];
            }
        }
    }
    MPI_Datatype BlockMat;
    MPI_Type_vector(blockSize, blockSize, blockSize * gridSize, MPI_DOUBLE, &BlockMat);
    MPI_Type_commit(&BlockMat);
    if (ProcRank == 0) {
        for (int p = 1; p < ProcNum; p++) {
            MPI_Send(matA.data() + (p % gridSize) * blockSize + (p / gridSize) * orderM * blockSize,
                1, BlockMat, p, 0, GridComm);
            MPI_Send(matB.data() + (p % gridSize) * blockSize + (p / gridSize) * orderM * blockSize,
                1, BlockMat, p, 1, GridComm);
        }
    } else {
        MPI_Recv(blockA.data(), blockSize * blockSize, MPI_DOUBLE, 0, 0, GridComm, &status);
        MPI_Recv(blockB.data(), blockSize * blockSize, MPI_DOUBLE, 0, 1, GridComm, &status);
    }
    for (int i = 0; i < gridSize; i++) {
        std::vector<double> tmpblockA(blockSize * blockSize);
        int pivot = (gridCoords[0] + i) % gridSize;
        if (gridCoords[1] == pivot) {
            tmpblockA = blockA;
        }
        MPI_Bcast(tmpblockA.data(), blockSize * blockSize, MPI_DOUBLE, pivot, RowComm);
        BlockMult(tmpblockA.data(), blockB.data(), blockC.data(), blockSize);
        int nextPr = gridCoords[0] + 1;
        if (gridCoords[0] == gridSize - 1)
            nextPr = 0;
        int prevPr = gridCoords[0] - 1;
        if (gridCoords[0] == 0)
            prevPr = gridSize - 1;
        MPI_Sendrecv_replace(blockB.data(), blockSize * blockSize, MPI_DOUBLE, prevPr, 0, nextPr , 0, ColComm, &status);
    }
    std::vector<double> resultM(orderM * orderM);
    if (ProcRank == 0) {
        for (int i = 0; i < blockSize; i++) {
            for (int j = 0; j < blockSize; j++)
                resultM[i * orderM + j] = blockC[i * blockSize + j];
        }
    }
    if (ProcRank != 0) {
        MPI_Send(blockC.data(), blockSize * blockSize, MPI_DOUBLE, 0, 3, MPI_COMM_WORLD);
    } else {
        for (int p = 1; p < ProcNum; p++) {
            MPI_Recv(resultM.data() + (p % gridSize) * blockSize + (p / gridSize) * orderM * blockSize,
                blockSize * blockSize, BlockMat, p, 3, MPI_COMM_WORLD, &status);
        }
    }
    MPI_Type_free(&BlockMat);
    return resultM;
}
