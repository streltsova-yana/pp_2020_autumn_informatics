// Copyright 2020 Panova Olga
#include <mpi.h>
#include <utility>
#include <list>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include "../../../modules/task_3/panova_o_optimization_splt_area/optimization_split_area.h"
double GetMForLipschitz(int num, const std::vector<double>& vec, std::function<double(double*)> my_function) {
    double difference = GetValue(vec[num], my_function) - GetValue(vec[num - 1], my_function);
    double res = std::abs(difference / (vec[num] - vec[num - 1]));
    return res;
}
double GetmForLipschitz(double M, double reliability) {
    if (M == 0) {
        return 1;
    } else {
        return M * reliability;
    }
}
double GetProbability(double m, int num, const std::vector<double>& vec, std::function<double(double*)> my_function) {
    double difference = GetValue(vec[num], my_function) - GetValue(vec[num - 1], my_function);
    double sum = GetValue(vec[num], my_function) + GetValue(vec[num - 1], my_function);
    double xdifference = (vec[num] - vec[num - 1]) * m;
    double res = xdifference + (difference * difference) / xdifference - 2 * sum;
    return res;
}
double GetValue(double x, std::function<double(double*)> my_function) {
    double* argument = &x;
    return my_function(argument);
}
double SequentialOptimization(double start, double end, std::function<double(double*)> my_function, double eps) {
    int count = 1;
    int index = 1;
    double reliability = 2;
    double probability;
    std::vector<double> res(1001, -1e+300);
    double m = GetmForLipschitz(GetMForLipschitz(1, res, my_function), reliability);
    res[0] = start;
    res[1] = end;
    double difference = GetValue(res[1], my_function) - GetValue(res[0], my_function);
    res[2] = (res[1] + res[0]) / 2 - difference / (2 * m);
    ++count;
    while (count < 1000) {
        sort(res.begin(), res.begin() + count + 1);
        double M = GetMForLipschitz(1, res, my_function);
        for (int i = 1; i <= count; i++) {
            M = std::max(M, GetMForLipschitz(i, res, my_function));
        }
        m = GetmForLipschitz(M, reliability);
        probability = GetProbability(m, 1, res, my_function);
        index = 1;
        for (int i = 1; i <= count; i++) {
            if (probability < GetProbability(m, i, res, my_function)) {
                probability = GetProbability(m, i, res, my_function);
                index = i;
            }
        }
        double difference = GetValue(res[index], my_function) - GetValue(res[index - 1], my_function);
        res[count + 1] = (res[index] + res[index - 1]) / 2 - difference / (2 * m);
        ++count;
        if (res[index] - res[index - 1] < eps) {
            break;
        }
    }
    return res[index];
}
double ParallelOptimization(double start, double end, std::function<double(double*)> my_function, double eps) {
    int count = 1;
    int index = 1;
    double reliability = 2;
    double probability;
    double tmp;
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size == 1) {
        return SequentialOptimization(start, end, my_function, eps);
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    double local_res;
    std::vector<double> total_res(size);
    std::vector<double> x(65, -1e+300);
    x[0] = start;
    x[1] = end;
    double m = GetmForLipschitz(GetMForLipschitz(1, x, my_function), reliability);
    double difference = GetValue(x[1], my_function) - GetValue(x[0], my_function);
    x[2] = (x[1] + x[0]) / 2 - difference / (2 * m);
    ++count;
    while (count < 64) {
        sort(x.begin(), x.begin() + count + 1);
        double M = GetMForLipschitz(1, x, my_function);
        for (int i = 1; i <= count; i++) {
            M = std::max(M, GetMForLipschitz(i, x, my_function));
        }
        m = GetmForLipschitz(M, reliability);
        probability = GetProbability(m, 1, x, my_function);
        index = 1;
        for (int i = 1; i <= count; i++) {
            tmp = GetProbability(m, i, x, my_function);
            if (probability < tmp) {
                probability = tmp;
                index = i;
            }
        }
        tmp = 2 * (x[count] - x[count - 1]) - 4 * GetValue(x[count - 1], my_function) /
            GetMForLipschitz(1, x, my_function);
        if (probability < tmp) {
            probability = tmp;
            index = count;
        }
        double difference = GetValue(x[index], my_function) - GetValue(x[index - 1], my_function);
        x[count + 1] = (x[index] + x[index - 1]) / 2 - difference / (2 * m);
        ++count;
        if (x[index] - x[index - 1] < eps) {
            break;
        }
    }
    sort(x.begin(), x.begin() + count + 1);
    double h = 64 / size;
    double local_start;
    double local_end;
    if (rank != size - 1) {
        local_start = x[rank * h];
        local_end = x[(rank + 1) * h];
    } else {
        local_start = x[rank * h];
        local_end = x[63];
    }
    local_res = SequentialOptimization(local_start, local_end, my_function, eps);
    MPI_Gather(&local_res, 1, MPI_DOUBLE, &total_res[0], 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            if (GetValue(total_res[i], my_function) < GetValue(total_res[0], my_function)) {
                std::swap(total_res[i], total_res[0]);
            }
        }
    }
    return total_res[0];
}
