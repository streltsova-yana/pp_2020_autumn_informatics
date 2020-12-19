// Copyright 2020 Bulychev Vladislav
#include <mpi.h>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/bulychev_v_calculation_of_integrals/calculation.h"

double SequentialCalculation(std::vector<double> a, std::vector<double> b,
    int n, double(*f)(std::vector<double>)) {
    int size_a = a.size();
    std::vector<double> h;
    double result = 0.0;
    std::vector <double> p(size_a);
    int num = pow(n, size_a);

    for (int i = 0; i < size_a; i++) {
        double t1 = b[i] - a[i];
        double t2 = t1 / n;
        h.push_back(t2);
    }

    for (int i = 0; i < num; i++) {
        for (int j = 0; j < size_a; j++) {
            double t3 = h[j] * 0.5;
            p[j] = (i % n) * h[j] + a[j] + t3;
        }
        result += f(p);
    }

    int t4 = size_a;
    double t5 = 1;
    for (int i = 0; i < t4; i++) {
        t5 = t5 * h[i];
    }

    result = result * t5;

    return result;
}

double ParallelCalculation(std::vector<double> a, std::vector<double> b,
    int n, double (*f)(std::vector<double>)) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int s = a.size();
    std::vector <double> p;
    double result = 0.0;
    double l_result = 0.0;

    int t3 = s - 1;
    int num = pow(n, t3);
    int score = num / size;
    int step = 0;
    if (rank < num % size) {
        score += 1;
        step = rank * score;
    } else {
        step = num % size + rank * score;
    }

    std::vector<double> h(s);
    if (rank == 0) {
        for (int i = 0; i < s; ++i) {
            double t1 = b[i] - a[i];
            double t2 = t1 / n;
            h[i] = t2;
        }
    }

    MPI_Bcast(&a[0], s, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h[0], s, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int t7 = s - 1;
    std::vector<double> tmp;
    for (int i = 0; i < score; ++i) {
        int number = step + i;
        p.resize(t7);
        for (int j = 0; j < s - 1; ++j) {
            double t6 = h[j] * 0.5;
            p[j] = (number % n) * h[j] + a[j] + t6;
        }
        for (int j = 0; j < n; ++j) {
            tmp = p;
            double t8 = h[t7] * 0.5;
            double t9 = j * h[t7] + t8 + a[t7];
            tmp.push_back(t9);
            l_result += f(tmp);
            tmp.clear();
        }
        p.clear();
    }

    int t4 = a.size();
    double t5 = 1;
    for (int i = 0; i < t4; i++) {
        t5 = t5 * h[i];
    }
    l_result = l_result * t5;

    MPI_Reduce(&l_result, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return result;
}
