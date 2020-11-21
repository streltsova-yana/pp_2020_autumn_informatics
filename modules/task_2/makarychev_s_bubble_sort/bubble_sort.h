// Copyright 2020 Makarychev Sergey
#ifndef MODULES_TASK_2_MAKARYCHEV_S_BUBBLE_SORT_BUBBLE_SORT_H_
#define MODULES_TASK_2_MAKARYCHEV_S_BUBBLE_SORT_BUBBLE_SORT_H_

#include <iostream>
#include <vector>
#include <random>

std::vector<int> getRandomVector(int sizeVec);
void sortVecParallel(int* vec, int sizeV, double* time);
std::vector<int> getRandomVector(int sizeVec);

#endif  // MODULES_TASK_2_MAKARYCHEV_S_BUBBLE_SORT_BUBBLE_SORT_H_
