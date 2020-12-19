// Copyright 2020 Tashirev Ivan
#include <mpi.h>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include "../../../modules/task_3/tashirev_i_graham/tashirev_i_graham.h"

std::vector<pixel> greh_sequential(std::vector <pixel> pixels) {
    std::vector <int> point_pos({0, 1});
    pixel f;
    int size = pixels.size();
    int it, pos = 0;
    f = pixels[0];
    for (it = 1; it < size; ++it)
        if (f.x > pixels[it].x || (f.x == pixels[it].x && f.y > pixels[it].y)) {
            pos = it;
            f = pixels[it];
        }

    std::swap(pixels[0], pixels[pos]);
    sort(pixels.begin() + 1, pixels.end(), [&](pixel a, pixel b) {
        if (rot(f, a, b) > 0)
            return true;
        else if (rot(f, a, b) < 0)
            return false;
        else
            return dist(f, b, a);
    });

    it = 2;
    while (it < size) {
        if (point_pos.size() < 2) {
            point_pos.push_back(it);
            ++it;
        }
        if (rot(pixels[point_pos[point_pos.size() - 2]], pixels[point_pos[point_pos.size() - 1]], pixels[it]) <= 0) {
            point_pos.pop_back();
        } else {
            point_pos.push_back(it);
            ++it;
        }
    }

    std::vector<pixel> hull_points(point_pos.size());
    for (size_t i = 0; i < point_pos.size(); i++) {
        hull_points[i] = pixels[point_pos[i]];
    }

    return hull_points;
}

std::vector<pixel> greh_parallel(std::vector <pixel> pixels, int size) {
    MPI_Datatype MPI_POINT;
    MPI_Type_contiguous(sizeof(int64_t) / sizeof(int) * 2, MPI_INT, &MPI_POINT);
    MPI_Type_commit(&MPI_POINT);

    int proc_s, proc_n;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_s);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_n);

    int* sendc = new int[proc_s];
    int* sendd = new int[proc_s];

    if (proc_n == 0) {
        int distance = 0;
        for (int i = 0; i < proc_s; i++) {
            sendd[i] = distance;
            if (i < size % proc_s) {
                sendc[i] = size / proc_s + 1;
                distance += size / proc_s + 1;
            } else {
                sendc[i] = size / proc_s;
                distance += size / proc_s;
            }
        }
    }

    int l_image_size;
    if (proc_n < size % proc_s)
        l_image_size = size / proc_s + 1;
    else
        l_image_size = size / proc_s;

    std::vector<pixel> l_image(l_image_size);

    MPI_Scatterv(pixels.data(), sendc, sendd, MPI_POINT,
        l_image.data(), l_image_size, MPI_POINT, 0, MPI_COMM_WORLD);

    l_image = greh_sequential(l_image);

    l_image_size = l_image.size();

    int counter = proc_s, swipe = 1;

    while (counter > 1) {
        counter = counter / 2 + counter % 2;
        if ((proc_n - swipe) % (2 * swipe) == 0) {
            MPI_Send(&l_image_size, 1, MPI_INT, proc_n - swipe, 0, MPI_COMM_WORLD);

            MPI_Send(l_image.data(), l_image_size, MPI_POINT, proc_n - swipe, 0, MPI_COMM_WORLD);
        }
        if ((proc_n % (2 * swipe) == 0) && (proc_s - proc_n > swipe)) {
            MPI_Status stat;
            int r_size;

            MPI_Recv(&r_size, 1, MPI_INT, proc_n + swipe, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);

            std::vector<pixel> r_image(r_size);

            MPI_Recv(r_image.data(), r_size, MPI_POINT, proc_n + swipe, MPI_ANY_TAG,
                MPI_COMM_WORLD, &stat);

            l_image.insert(l_image.end(), r_image.begin(), r_image.end());

            l_image = greh_sequential(l_image);

            l_image_size = l_image.size();
        }
        swipe = 2 * swipe;
    }
    return l_image;
}

std::vector <pixel> get_random_image(int width, int height, int* size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    std::vector <pixel> out_image;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (gen() % 4 == 0) {
                out_image.push_back(pixel(j, i));
                ++(*size);
            }
        }
    }
    return out_image;
}

bool dist(pixel p1, pixel p2, pixel p3) {
    return (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y) >=
    (p1.x - p3.x) * (p1.x - p3.x) + (p1.y - p3.y) * (p1.y - p3.y);
}

int64_t rot(pixel p1, pixel p2, pixel p3) {
    return (p2.x - p1.x) * (p3.y - p1.y) - (p2.y - p1.y) * (p3.x - p1.x);
}
