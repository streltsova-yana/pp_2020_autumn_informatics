// Copyright 2020 Makarov Alexander
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include "../../../modules/task_3/makarov_a_gauss_filter_col/gauss_filter_col.h"


std::vector<unsigned int> generate_image(unsigned int w, unsigned int h) {
    unsigned int size = w * h;
    if (size == 0) return std::vector<unsigned int>();
    std::random_device rd;
    std::mt19937 gen(rd());
    size = w * h;
    std::vector<unsigned int> image(size);
    for (unsigned int i = 0; i < h; i++) {
        for (unsigned int j = 0; j < w; j++)
            image[i * w + j] = static_cast<unsigned int>(gen() % 256);
    }
    return image;
}

std::vector<double> createGaussianKernel(double sigma, unsigned int radius) {
    int m_radius = static_cast<int>(radius);
    unsigned int size = 2 * m_radius + 1;
    std::vector<double> result(size * size);
    double norm = 0;

    for (int i = -m_radius; i <= m_radius; i++)
        for (int j = -m_radius; j <= m_radius; j++) {
            int idx = (i + m_radius) * size + (j + m_radius);
            result[idx] = exp(-static_cast<double>(i * i + j * j) /
                         static_cast<double>(2. * sigma * sigma));
            norm += result[idx];
        }
    for (int i = -m_radius; i <= m_radius; i++)
        for (int j = -m_radius; j <= m_radius; j++) {
            int idx = (i + m_radius) * size + (j + m_radius);
            result[idx] /= norm;
        }
    return result;
}

std::vector<unsigned int> gaussFilter(
                               const std::vector<unsigned int>& exp_image,
                               unsigned int w, unsigned int h, double sigma,
                               unsigned int radius) {
    std::vector<double> gauss_kernel = createGaussianKernel(sigma, radius);
    int diam = static_cast<int>(2 * radius);
    int kern_radius = static_cast<int>(radius);
    int kern_size = kern_radius * 2 + 1;
    int result_h = static_cast<int>(h);
    int result_w = static_cast<int>(w);
    std::vector<unsigned int> result(result_h * result_w);
    for (int i = 0; i < result_h; i++)
        for (int j = 0; j < result_w; j++) {
            double conv = 0.;
            for (int k = -kern_radius; k <= kern_radius; k++)
                for (int t = -kern_radius; t <= kern_radius; t++)
                    conv += (static_cast<double>(exp_image[
                                   (kern_radius + i + k) * (result_w + diam) +
                                   kern_radius + j + t]) *
                                   gauss_kernel[
                                   (k + kern_radius) * kern_size + t +
                                   kern_radius]);
            result[i * result_w + j] = static_cast<unsigned int>(conv);
        }
    return result;
}

std::vector<unsigned int> transpose(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h) {
    std::vector<unsigned int> result(w * h);
    for (unsigned int i = 0; i < h; i++)
        for (unsigned int j = 0; j < w; j++)
            result[j * h + i] = image[i * w + j];
    return result;
}

std::vector<unsigned int> expand(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   unsigned int radius) {
    unsigned int diam = radius * 2;
    std::vector<unsigned int> result((w + diam) * (h + diam));
    // transpose image
    for (unsigned int i = 0; i < h; i++)
        for (unsigned int j = 0; j < w; j++)
            result[(i + radius) * (w + diam) + (j + radius)] =
                                                             image[i * w + j];
    // frame
    for (unsigned int i = 0; i < h; i++) {
        for (unsigned int j = 0; j < radius; j++) {
            result[(i + radius) * (w + diam) + j] = result[
                                          (i + radius) * (w + diam) + radius];
            result[(i + radius) * (w + diam) + (w + radius) + j] =
                         result[(i + radius) * (w + diam) + (w + radius - 1)];
        }
    }
    for (unsigned int i = 0; i < w + diam; i++) {
        for (unsigned int j = 0; j < radius; j++) {
            result[(w + diam) * j + i] = result[(w + diam) * radius + i];
            result[(h + radius + j) * (w + diam) + i] = result[
                                           (h + radius - 1) * (w + diam) + i];
        }
    }
    return result;
}

std::vector<unsigned int> gaussFilterSequential(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   double sigma, unsigned int radius) {
    std::vector<unsigned int> expanded_img = expand(
                                                    transpose(image, w, h), h,
                                                    w, radius);
    std::vector<unsigned int> result = transpose(
                               gaussFilter(expanded_img, h, w, sigma, radius),
                               h, w);
    return result;
}

std::vector<unsigned int> gaussFilterParallel(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   double sigma, unsigned int radius) {
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int proc_count;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    if (proc_count <= 1) return gaussFilterSequential(image, w, h, sigma,
                                                      radius);
    unsigned int diam = radius * 2;
    unsigned int delta = w / (proc_count - 1);
    unsigned int remain = w % (proc_count - 1);

    unsigned int first_count = (h + diam) * (remain + delta + diam);
    unsigned int col_count = (h + diam) * (delta + diam);
    if (rank == 0) {
        std::vector<unsigned int> t_image = expand(transpose(image, w, h),
                                                       h, w, radius);
        MPI_Send(t_image.data(), first_count, MPI_INT, 1, 0,
                 MPI_COMM_WORLD);
        for (int i = 2; i < proc_count; i++) {
            // start_pos = (h + 2) * (1 + remain + (i - 1) * delta - 1);
            unsigned int start_pos = (h + diam) * (remain + (i - 1) * delta);
            MPI_Send(t_image.data() + start_pos, col_count, MPI_INT, i, 0,
                     MPI_COMM_WORLD);
        }
        MPI_Status status;
        std::vector<unsigned int> result(h * w);
        std::vector<unsigned int> tmp((remain + delta) * h);
        MPI_Recv(tmp.data(), (remain + delta) * h, MPI_INT, 1, MPI_ANY_TAG,
                 MPI_COMM_WORLD, &status);
        for (unsigned int i = 0; i < h; i++) {
            for (unsigned int j = 0; j < remain + delta; j++)
                result[i * w + j] = tmp[i * (remain + delta) + j];
        }
        for (int i = 2; i < proc_count; i++) {
            unsigned int start_col = remain + delta * (i - 1);
            MPI_Recv(tmp.data(), delta * h, MPI_INT, i, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &status);
            for (unsigned int j = 0; j < h; j++) {
                for (unsigned int k = 0; k < delta; k++)
           // result[w * k + ((i - 1) * delta + remain + j)] = tmp[j * h + k];
                    result[j * w + start_col + k] = tmp[j * delta + k];
            }
        }
        return result;
    } else {
        std::vector<unsigned int> local_image;
        std::vector<unsigned int> local_result;
        MPI_Status status;
        if (rank == 1) {
            local_image.resize(first_count);
            MPI_Recv(local_image.data(), first_count, MPI_INT, 0,
                     MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            local_result = transpose(gaussFilter(local_image, h,
                                     remain + delta, sigma, radius), h,
                                     remain + delta);
            MPI_Send(local_result.data(), (remain + delta) * h, MPI_INT, 0,
                     0, MPI_COMM_WORLD);
        } else {
            local_image.resize(col_count);
            MPI_Recv(local_image.data(), col_count, MPI_INT, 0, MPI_ANY_TAG,
                                                     MPI_COMM_WORLD, &status);
            local_result = transpose(
                            gaussFilter(local_image, h, delta, sigma, radius),
                            h, delta);
            MPI_Send(local_result.data(), delta * h, MPI_INT, 0, 0,
                                                       MPI_COMM_WORLD);
        }
        return local_result;
    }
}
