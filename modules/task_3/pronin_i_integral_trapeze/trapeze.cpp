// Copyright 2020 Pronin Igor
#include <mpi.h>
#include <vector>
#include <cmath>
#include "../../../modules/task_3/pronin_i_integral_trapeze/trapeze.h"
double SequentialOperations(double(*function)(std::vector<double>),
    std::vector<double> a, std::vector<double> b, int n) {
    size_t mer = a.size();
    double result = 0;
    std::vector<double> h(mer);
    std::vector<double> points(mer);
    for (size_t i = 0; i < mer; i++)
        h[i] = (b[i] - a[i]) / n;
    int count = pow(n, mer);
    for (int i = 0; i < count; i++) {
        for (size_t j = 0; j < mer; j++)
            points[j] = a[j] + h[j] * (i % n + 0.5);
        double f = function(points);
        result = result + f;
    }
    for (size_t i = 0; i < mer; i++)
        result = result * h[i];
    return result;
}

double ParllelOperations(double(*function)(std::vector<double>),
    std::vector<double> a, std::vector<double> b, int n) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    size_t mer = a.size();
    std::vector<double> h(mer);
    std::vector<double> points(mer);

    std::vector<double> acopy(mer);
    std::vector<double> bcopy(mer);
    int ncopy = n;

    int count = 0;
    if (rank == 0) {
        for (size_t i = 0; i < mer; i++) {
            h[i] = (b[i] - a[i]) / n;
            acopy[i] = a[i];
            bcopy[i] = b[i];
        }
        count = pow(n, mer);
    }
    MPI_Bcast(&count, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ncopy, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h[0], mer, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&acopy[0], mer, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&bcopy[0], mer, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int ostatok = count % size;
    int step = count / size;
    int rankstep = 0;
    double result = 0.0;

    if (rank == 0) {
        rankstep = step + ostatok;
        for (int i = 0; i < rankstep; i++) {
            for (size_t j = 0; j < mer; j++)
                points[j] = a[j] + h[j] * (i % n + 0.5);
            double f = function(points);
            result = result + f;
        }
    } else {
        rankstep = rank * step + ostatok;
        for (int i = 0; i < step; i++) {
            for (size_t j = 0; j < mer; j++)
                points[j] = a[j] + h[j] * (rankstep % n + 0.5);
            double f = function(points);
            result = result + f;
            rankstep++;
        }
    }
    double itog = 0.0;
    MPI_Reduce(&result, &itog, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    for (size_t i = 0; i < mer; i++)
        itog = itog * h[i];
    return itog;
}
