// Copyright 2020 Makarov Alexander
#ifndef MODULES_TASK_3_MAKAROV_A_GAUSS_FILTER_COL_GAUSS_FILTER_COL_H_
#define MODULES_TASK_3_MAKAROV_A_GAUSS_FILTER_COL_GAUSS_FILTER_COL_H_

#include <vector>

std::vector<unsigned int> generate_image(unsigned int w, unsigned int h);

std::vector<double> createGaussianKernel(double sigma, unsigned int radius);

std::vector<unsigned int> gaussFilter(
                               const std::vector<unsigned int>& exp_image,
                               unsigned int w, unsigned int h, double sigma,
                               unsigned int radius);

std::vector<unsigned int> transpose(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h);

std::vector<unsigned int> expand(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   unsigned int radius);

std::vector<unsigned int> gaussFilterSequential(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   double sigma, unsigned int radius);
std::vector<unsigned int> gaussFilterParallel(
                                   const std::vector<unsigned int>& image,
                                   unsigned int w, unsigned int h,
                                   double sigma, unsigned int radius);

#endif  // MODULES_TASK_3_MAKAROV_A_GAUSS_FILTER_COL_GAUSS_FILTER_COL_H_
