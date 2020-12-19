// Copyright 2020 Streltsova Yana
#ifndef MODULES_TASK_3_STRELTSOVA_Y_STRASSENS_ALGORITHM_STRASSENS_ALGORITHM_H_
#define MODULES_TASK_3_STRELTSOVA_Y_STRASSENS_ALGORITHM_STRASSENS_ALGORITHM_H_
#include <vector>

class Matrix {
 public:
    int rows, cols;
    std::vector<double> m;

    Matrix();
    Matrix(int _rows, int _cols);
    Matrix(int _rows, int _cols, std::mt19937 _gen);
    ~Matrix();
    Matrix operator-() const;
    const Matrix& operator=(const Matrix& a);
    void print() const;
};

Matrix* get_square_matrix(const Matrix& _a, int size);
Matrix* get_orig_size_matrix(const Matrix& _a, int size);
void get_four_matrix(const Matrix& _a, Matrix** _a11, Matrix** _a12, Matrix** _a21, Matrix** _a22);
Matrix* get_one_matrix(const Matrix& _a11, const Matrix& _a12, const Matrix& _a21, const Matrix& _a22);
Matrix* parallel_sum(const Matrix& _a, const Matrix& _b);
Matrix* sequential_sum(const Matrix& _a, const Matrix& _b);
Matrix* sequential_mul(const Matrix& _a, const Matrix& _b);
Matrix* parallel_mul(const Matrix& _a, const Matrix& _b);
Matrix* Strassen_alg(const Matrix& _a, const Matrix& _b);

#endif  // MODULES_TASK_3_STRELTSOVA_Y_STRASSENS_ALGORITHM_STRASSENS_ALGORITHM_H_
