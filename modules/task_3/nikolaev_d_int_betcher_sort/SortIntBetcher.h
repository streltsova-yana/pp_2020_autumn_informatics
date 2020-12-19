// Copyright 2020 Nikolaev Denis
#ifndef MODULES_TASK_3_NIKOLAEV_D_INT_BETCHER_SORT_SORTINTBETCHER_H_
#define MODULES_TASK_3_NIKOLAEV_D_INT_BETCHER_SORT_SORTINTBETCHER_H_
#include <mpi.h>
#include <vector>

std::vector<int> OddSpliter(std::vector<int>vec1, std::vector<int>vec2, std::vector<int> res);
std::vector<int> EvenSpliter(std::vector<int>vec1, std::vector<int>vec2, std::vector<int> res);
std::vector<int> SimpleComparator(std::vector<int>res, std::vector<int> even, std::vector<int> odd);
std::vector<int> genRandVector(int n);
bool degree_2(int n);
std::vector<int> SequentialRadixSort(std::vector<int> vec);
std::vector<int> BetcherMerge(std::vector<int> vec, int n);

#include <ctime>
#include <random>
#include <algorithm>
#include <iostream>

#endif  // MODULES_TASK_3_NIKOLAEV_D_INT_BETCHER_SORT_SORTINTBETCHER_H_
