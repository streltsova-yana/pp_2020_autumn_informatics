// Copyright 2020 Gogov Vladislav
#include <mpi.h>
#include <vector>
#include <random>
#include <algorithm>
#include "../../../modules/task_1/gogov_v_matrix_max/matrix_max.h"

Matrix createRandomMatrix(int rows, int cols) {
    std::random_device rd;
    std::mt19937 mersenne(rd());
    Matrix matrix(rows * cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i * cols + j] = static_cast<int>(mersenne() % 100u);
        }
    }
    return matrix;
}

int findMaxSequential(const Matrix& matrix) {
    if (matrix.size() == 0) {
       throw "Invalid matrix size.";
    }
    return *std::max_element(matrix.begin(), matrix.end());
}

int findMaxParallel(const Matrix& matrix, int rows, int cols) {
    int procNum, procRank;
    int elementsCount = rows * cols;
    if (elementsCount == 0) {
       throw "Invalid matrix size.";
    }
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const int n = elementsCount / procNum;
    int delta = elementsCount - n * procNum;
    if (procRank == 0) {
        for (int i = 1; i < procNum; i++)
            MPI_Send(&matrix[0] + delta + i * n, n, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
    Matrix localMatrix(n + (procRank == 0 ? delta : 0));
    if (procRank == 0) {
        localMatrix = Matrix(matrix.begin(), matrix.begin() + n + delta);
    } else {
        MPI_Status status;
        MPI_Recv(&localMatrix[0], n, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }
    int globalMax;
    int localMax = findMaxSequential(localMatrix);
    MPI_Reduce(&localMax, &globalMax, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
    return globalMax;
}
