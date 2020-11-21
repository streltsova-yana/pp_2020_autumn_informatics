// Copyright 2020 Kulandin Denis
#include <mpi.h>
#include <vector>
#include <algorithm>
#include <cassert>
#include <ctime>
#include <cmath>
#include <random>
#include <numeric>
#include <iostream>
#include <utility>
#include "../../../modules/task_2/kulandin_d_iteration_method_zeidel/zeidel.h"

std::vector<double> randomVectorA(const size_t sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> ans(sz * sz);
    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = 0; j < sz; ++j) {
            double tmp = gen() % 1000;
            ans[i * sz + j] = i == j ? tmp + 1234 * sz : tmp;
        }
    }
    return ans;
}

std::vector<double> randomVectorB(const size_t sz) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> ans(sz);
    for (size_t i = 0; i < sz; ++i) {
        ans[i] = gen() % 100;
    }
    return ans;
}

bool converge(const std::vector<double>& x, const std::vector<double>& last, double eps) {
    double ans = 0;
    for (size_t i = 0; i < x.size(); ++i) {
        double tmp = x[i] - last[i];
        ans += tmp * tmp;
    }
    return sqrt(ans) < eps;
}

std::pair<bool, std::vector<double> > zeidelSequential(const std::vector<double>& a, const std::vector<double>& b,
                                                       size_t n, double eps) {
    int cntIterations = 100;
    std::vector<double> x(n, 0), last(n, 0);
    do {
        last = x;
        for (size_t i = 0; i < n; ++i) {
            x[i] = 0;
            double gg = 0;
            for (size_t j = 0; j < n; ++j) {
                gg += a[i * n + j] * x[j];
            }
            x[i] = (b[i] - gg) / a[i * n + i];
        }
    } while (!converge(x, last, eps) && cntIterations--);
    return std::make_pair(cntIterations != 0, x);
}

double calcParallel(const std::vector<double>& a, const std::vector<double>& x, size_t row, size_t n) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    const size_t delta = n / procNum;
    const size_t rem = n % procNum;
    if (procRank == 0) {
        for (int j = 1; j < procNum; ++j) {
            MPI_Send(&a[0] + row * n + rem + j * delta, delta, MPI_DOUBLE, j, 0, MPI_COMM_WORLD);
            MPI_Send(&x[0] + rem + j * delta, delta, MPI_DOUBLE, j, 1, MPI_COMM_WORLD);
        }
    }
    std::vector<double> local(rem + delta);
    std::vector<double> localX(rem + delta);
    if (procRank == 0) {
        local = std::vector<double>(a.begin() + row * n, a.begin() + row * n + delta + rem);
        localX = std::vector<double>(x.begin(), x.begin() + delta + rem);
    } else {
        MPI_Status status;
        MPI_Recv(&local[0], delta, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&localX[0], delta, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
    }
    double localSum = 0;
    double globalSum = 0;
    for (size_t i = 0; i < (procRank == 0 ? rem + delta : delta); ++i) {
        localSum += local[i] * localX[i];
    }
    MPI_Reduce(&localSum, &globalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return globalSum;
}

std::pair<bool, std::vector<double> > zeidelParallel(const std::vector<double>& a, const std::vector<double>& b,
                                                     size_t n, double eps) {
    int procNum, procRank;
    MPI_Comm_size(MPI_COMM_WORLD, &procNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
    int cntIterations = 100;
    std::vector<double> x(n, 0), last(n, 0);
    char result;
    do {
        last = x;
        for (size_t i = 0; i < n; ++i) {
            x[i] = 0;
            double gg = calcParallel(a, x, i, n);
            x[i] = (b[i] - gg) / a[i * n + i];
        }
        if (!converge(x, last, eps) && cntIterations--) {
            result = 1;
        } else {
            result = 0;
        }
        MPI_Bcast(&result, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    } while (result);

    return std::make_pair(cntIterations != 0, x);
}
