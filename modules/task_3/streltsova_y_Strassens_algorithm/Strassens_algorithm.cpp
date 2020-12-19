// Copyright 2020 Streltsova Yana
#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include "../../modules/task_3/streltsova_y_Strassens_algorithm/Strassens_algorithm.h"

Matrix::Matrix() {
    rows = 0;
    cols = 0;
}
Matrix::Matrix(int _rows, int _cols) {
    rows = _rows;
    cols = _cols;
    m = std::vector<double>(rows * cols, 0.);
}
Matrix::Matrix(int _rows, int _cols, std::mt19937 _gen) {
    rows = _rows;
    cols = _cols;
    m = std::vector<double>(_rows * _cols);
    for (size_t i = 0; i < m.size(); i++) {
        std::uniform_real_distribution<> urd(0, 100);
            m[i] = urd(_gen);
    }
}
Matrix::~Matrix() {}
Matrix Matrix::operator-() const {
    Matrix tmp(rows, cols);
    for (size_t i = 0; i < m.size(); i++)
            tmp.m[i] = -m[i];
    return tmp;
}
const Matrix& Matrix :: operator=(const Matrix& _a) {
    if (this == &_a)
        return *this;
    rows = _a.rows;
    cols = _a.cols;
    m = _a.m;
    return *this;
}
void Matrix::print() const {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++)
            std::cout << m[i * cols + j] << " ";
        std::cout << std::endl;
    }
}

Matrix* get_square_matrix(const Matrix& _a, int size) {
    Matrix* tmp = new Matrix();
    int n = 2;
    while (size > pow(2, n)) n++;
    if (_a.rows == _a.cols && size == pow(2, n)) {
        *tmp = _a;
        return tmp;
    }
    size = pow(2, n);
    *tmp = Matrix(size, size);
    for (int i = 0; i < _a.rows; i++)
        for (int j = 0; j < _a.cols; j++)
            tmp->m[i * size + j] = _a.m[i * _a.cols + j];
    return tmp;
}

Matrix* get_orig_size_matrix(const Matrix& _a, int size) {
    Matrix* tmp = new Matrix(size, size);
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            tmp->m[i * size + j] = _a.m[i * _a.cols + j];
    return tmp;
}

void get_four_matrix(const Matrix& _a, Matrix** _a11, Matrix** _a12, Matrix** _a21, Matrix** _a22) {
    int n = _a.rows / 2;
    *_a11 = new Matrix(n, n);
    *_a12 = new Matrix(n, n);
    *_a21 = new Matrix(n, n);
    *_a22 = new Matrix(n, n);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            (*_a11)->m[i * n + j] = _a.m[i * _a.cols + j];
            (*_a12)->m[i * n + j] = _a.m[i * _a.cols + n + j];
            (*_a21)->m[i * n + j] = _a.m[(n + i) * _a.cols + j];
            (*_a22)->m[i * n + j] = _a.m[(n + i) * _a.cols + n + j];
        }
}

Matrix* get_one_matrix(const Matrix& _a11, const Matrix& _a12, const Matrix& _a21, const Matrix& _a22) {
    int n = _a11.rows;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Matrix* result = new Matrix(n * 2, n * 2);
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            result->m[i * result->cols + j] = _a11.m[i * n + j];
            result->m[i * result->cols + n + j] = _a12.m[i * n + j];
            result->m[(n + i) * result->cols + j] = _a21.m[i * n + j];
            result->m[(n + i) * result->cols + n + j] = _a22.m[i * n + j];
        }
    return result;
}

Matrix* sequential_sum(const Matrix& _a, const Matrix& _b) {
    Matrix* result = new Matrix(_a.rows, _a.cols);
    for (size_t i = 0; i < _a.m.size(); i++)
        result->m[i] = _a.m[i] + _b.m[i];
    return result;
}

Matrix* parallel_sum(const Matrix& _a, const Matrix& _b) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int n = _a.rows;
    int part = n / size;
    std::vector<int> scounts(size, part * n);
    int remain = n % size;
    if (rank < remain)
        scounts[rank] += n;
    std::vector<int> displs(size, 0);
    for (int i = 1; i < size; i++)
        displs[i] = displs[i - 1] + scounts[i - 1];

    std::vector<double> local_vec1(scounts[rank]), local_vec2(scounts[rank]);
    MPI_Scatterv(_a.m.data(), scounts.data(), displs.data(), MPI_DOUBLE,
        local_vec1.data(), scounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(_b.m.data(), scounts.data(), displs.data(), MPI_DOUBLE,
        local_vec2.data(), scounts[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    for (int i = 0; i < scounts[rank]; i++)
        local_vec1[i] += local_vec2[i];

    Matrix* result = new Matrix(n, n);
    MPI_Gatherv(local_vec1.data(), scounts[rank], MPI_DOUBLE,
        result->m.data(), scounts.data(), displs.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);
    return result;
}

Matrix* sequential_mul(const Matrix& _a, const Matrix& _b) {
    if (_a.cols != _b.rows)
        throw "Matrices are not consistent";
    Matrix* C = new Matrix(_a.rows, _b.cols);
    for (int i = 0; i < _a.rows; i++)
        for (int j = 0; j < _b.cols; j++)
            for (int k = 0; k < _a.cols; k++)
                C->m[i * _b.cols + j] += _a.m[i * _a.cols + k] * _b.m[k * _b.cols + j];
    return C;
}

Matrix* Strassen_alg(const Matrix& _a, const Matrix& _b) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int n = 0;
    if (rank == 0)
        n = _a.rows;
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    Matrix a(n, n), b(n, n);
    if (rank == 0) {
        a = _a;
        b = _b;
    }
    MPI_Bcast(a.m.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(b.m.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n <= 64)
        return sequential_mul(a, b);
    Matrix* A = get_square_matrix(a, n);
    Matrix* B = get_square_matrix(b, n);
    Matrix *a11, *a12, *a21, *a22, *b11, *b12, *b21, *b22;
    get_four_matrix(*A, &a11, &a12, &a21, &a22);
    get_four_matrix(*B, &b11, &b12, &b21, &b22);

    Matrix* P1 = Strassen_alg(*parallel_sum(*a11, *a22), *parallel_sum(*b11, *b22));
    Matrix* P2 = Strassen_alg(*parallel_sum(*a21, *a22), *b11);
    Matrix* P3 = Strassen_alg(*a11, *parallel_sum(*b12, -*b22));
    Matrix* P4 = Strassen_alg(*a22, *parallel_sum(*b21, -*b11));
    Matrix* P5 = Strassen_alg(*parallel_sum(*a11, *a12), *b22);
    Matrix* P6 = Strassen_alg(*parallel_sum(*a21, -*a11), *parallel_sum(*b11, *b12));
    Matrix* P7 = Strassen_alg(*parallel_sum(*a12, -*a22), *parallel_sum(*b21, *b22));

    Matrix* c11 = parallel_sum(*parallel_sum(*P1, *P4), *parallel_sum(-*P5, *P7));
    Matrix* c12 = parallel_sum(*P3, *P5);
    Matrix* c21 = parallel_sum(*P2, *P4);
    Matrix* c22 = parallel_sum(*parallel_sum(*P1, -*P2), *parallel_sum(*P3, *P6));
    return get_one_matrix(*c11, *c12, *c21, *c22);
}
