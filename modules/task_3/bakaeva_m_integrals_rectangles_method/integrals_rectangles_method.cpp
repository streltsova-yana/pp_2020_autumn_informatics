// Copyright 2020 Bakaeva Maria
#include "../../../modules/task_3/bakaeva_m_integrals_rectangles_method/integrals_rectangles_method.h"
#include <mpi.h>
#include <stddef.h>
#include <math.h>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <iostream>
#include <utility>

using std::vector;
using std::pair;


double getSequentialIntegrals(const int n, vector<pair<double, double> > a_b, double (*F)(vector<double>)) {
    // a_b - массив пределов интегрирования
    // n - количество отрезков интегрирования
    // h = (b - a) / n - шаг разбиения отрезка [a, b]
    // Integral = h * Summ(f(xi))
    // countIntegrals - количество интегралов

    int countIntegrals = a_b.size();
    int size = 1;
    vector<double> h(countIntegrals);
    for (int i = 0; i < countIntegrals; i++) {
        h[i] = (a_b[i].second - a_b[i].first) / n;
        size = size * n;
    }

    double result = 0.0;
    vector<double> forCalculateF(countIntegrals);
    for (int j = 0; j < size; j++) {
    for (int i = 0; i < countIntegrals; i++) {
        forCalculateF[i] = a_b[i].first + (j % n) * h[i] + h[i] * 0.5;
    }
    result += F(forCalculateF);
    }

    for (int i = 0; i < countIntegrals; i++) {
        result *= h[i];
    }

    return result;
}

double getParallelIntegrals(const int n, vector<pair<double, double> > a_b, double (*F)(vector<double>)) {
    int size, rank;

    //Число задействованных процессов
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //Получение номера текущего процесса в рамках коммуникатора
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int countIntegrals = a_b.size();
    vector<double> h(n);
    vector<pair<double, double>> ab(countIntegrals);
    int countElements;  // Количество всех одночленов

    // Находим шаги разбиения для каждого отрезка [a,b]
    if (rank == 0) {
        countElements = 1;
        for (int i = 0; i < countIntegrals; i++) {
            h[i] = (a_b[i].second - a_b[i].first) / n;
            ab[i] = a_b[i];
        }
        int j = 0;
        while (j != countIntegrals - 1) {
            countElements *= n;
            j++;
        }
    }

    // Рассылаем данные всем процессам
    int length = n;  // Количество отрезков интегрирования
    MPI_Bcast(&countElements, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&length, countIntegrals, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&h[0], countIntegrals, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(&ab[0], 2 * countIntegrals, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    int delta = countElements / size;
    int remainder = countElements % size;
    if (rank < remainder) {
        delta += 1;
    }

    int tmp = 0;
    if (rank < remainder) {
        tmp = rank * delta;
    } else {
        tmp = remainder + rank * delta;
    }

    // Каждый процесс вычисляет свое количество sum(f(x..))
    vector<vector<double>> forCalculateF(delta);
    for (int i = 0; i < delta; i++) {
        int number = tmp + i;
        for (int j = 0; j < countIntegrals - 1; j++) {
            forCalculateF[i].push_back(ab[j].first + h[j] * (number % n) + h[j] * 0.5);
        }
    }

    double result = 0.0;
    for (int i = 0; i < delta; i++) {
        for (int j = 0; j < n; j++) {
            forCalculateF[i].push_back(ab[countIntegrals - 1].first + (j + 0.5) * h[countIntegrals - 1]);
            result += F(forCalculateF[i]);
            forCalculateF[i].pop_back();
        }
    }

    // Суммирование всех полученых результатов
    double Integral = 0.0;
    MPI_Reduce(&result, &Integral, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    // Умножение на h по формуле
    for (int i = 0; i < countIntegrals; i++) {
        Integral *= h[i];
    }

    return Integral;
}
