// Copyright 2020 Gurylev Nikita
#include <mpi.h>
#include <random>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include"../../../modules/task_3/gurylev_n_integral_monte_carlo/integral_monte_carlo.h"

double getSequentialIntegralMCarlo(double(*function)(std::vector<double>), const std::vector<double>& llim,
    const std::vector<double>& ulim, int n) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    int set_points = llim.size();
    std::vector<std::uniform_real_distribution<double>> points(set_points);
    std::vector<double> tmp(set_points);
    double result = 0.0;
    for (int i = 0; i < set_points; i++) {
        points[i] = std::uniform_real_distribution<double>(llim[i], ulim[i]);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < set_points; j++)
            tmp[j] = points[j](gen);
        result += function(tmp);
    }
    for (int i = 0; i < set_points; i++) {
        result *= (ulim[i] - llim[i]);
    }
    result /= n;
    return result;
}

double getParallelIntegralMCarlo(double(*function)(std::vector<double>), const std::vector<double>& llim,
    const std::vector<double>& ulim, int n) {
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int set_points = static_cast<int>(llim.size());
    std::vector<std::uniform_real_distribution<double>> points(set_points);
    std::vector<double> tmp(set_points);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < set_points; i++) {
        points[i] = std::uniform_real_distribution<double>(llim[i], ulim[i]);
    }
    double result = 0.0;
    double n_proc = n / size + (rank < n% size ? 1 : 0);
    for (int i = 0; i < n_proc; i++) {
        for (int j = 0; j < set_points; j++) {
            tmp[j] = points[j](gen);
        }
        result += function(tmp);
    }
    double g_result = 0.0;
    MPI_Reduce(&result, &g_result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < set_points; i++) {
            g_result *= (ulim[i] - llim[i]);
        }
        g_result /= n;
    }
    return g_result;
}
