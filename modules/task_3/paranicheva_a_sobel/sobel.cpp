// Copyright 2020 Paranicheva Alyona
#include <mpi.h>
#include <iostream>
#include <random>
#include <ctime>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/paranicheva_a_sobel/sobel.h"

std::vector<int> getRandomMatrix(int rows, int cols) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<int> array(rows * cols);
    for (int i = 0; i < rows * cols; i++)
        array[i] = static_cast<int>(gen() % 256);
    return array;
}

int check(int tmp, int min, int max) {
    if (tmp > max)
        return max;
    else if (tmp < min)
        return min;
    return tmp;
}

int SobelXY(std::vector<int> mat, int cols, int posr, int posc) {
    std::vector<int> x = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    std::vector<int> y = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
    int sobx = 0, soby = 0, res, count = 0;
    for (int i = -1; i < 2; i++)
        for (int j = -1; j < 2; j++) {
            sobx += mat[(posr + i) * cols + (posc + j)] * x[count];
            soby += mat[(posr + i) * cols + (posc + j)] * y[count];
            count++;
        }
    res = check(sqrt(sobx * sobx + soby * soby), 0, 255);
    return res;
}

std::vector<int> getSequentialSobel(std::vector<int> mat, int rows, int cols) {
    std::vector<int> sobarr(rows * cols);
    for (int i = 0; i < cols; i++)
        sobarr[i] = 0;
    for (int i = 1; i < rows - 1; i++)
        for (int j = 0; j < cols; j++) {
            if (j == 0 || j == cols - 1)
                sobarr[i * cols + j] = 0;
            else
                sobarr[i * cols + j] = SobelXY(mat, cols, i, j);
        }
    for (int i = 0; i < cols; i++)
        sobarr[(rows - 1) * cols + i] = 0;
    return sobarr;
}

std::vector<int> getParalSobel(std::vector<int> mat, int rows, int cols) {
    int size, rank;
    std::vector<int> sobmat(rows * cols);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int count = (rows - 1) / size;
    int rem = (rows - 1) % size;
    if (rank == 0) {
        for (int i = 0; i < cols; i++)
            sobmat[i] = 0;
        for (int i = 1; i < count + rem; i++)
            for (int j = 0; j < cols; j++) {
                if (j == 0 || j == cols - 1)
                    sobmat[i * cols + j] = 0;
                else
                    sobmat[i * cols + j] = SobelXY(mat, cols, i, j);
            }
        for (int i = 1; i < size; i++)
            MPI_Send(mat.data() + rem * cols + count * cols * i - cols,
                     (count + 2) * cols, MPI_INT, i, 0, MPI_COMM_WORLD);
    } else {
        std::vector<int> tmpmat((count + 2) * cols);
        std::vector<int> tmpsob(count * cols);
        MPI_Status status;
        MPI_Recv(tmpmat.data(), (count + 2) * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        for (int i = 0; i < count; i++) {
            for (int j = 0; j < cols; j++) {
                if (j == 0 || j == cols - 1)
                    tmpsob[i * cols + j] = 0;
                else
                    tmpsob[i * cols + j] = SobelXY(tmpmat, cols, i + 1, j);
            }
        }
        MPI_Send(tmpsob.data(), count * cols, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            MPI_Status status;
            MPI_Recv(sobmat.data() + rem * cols + count * cols * i,
                     count * cols, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }
        for (int i = 0; i < cols; i++)
            sobmat[(rows - 1) * cols + i] = 0;
    }
    return sobmat;
}
