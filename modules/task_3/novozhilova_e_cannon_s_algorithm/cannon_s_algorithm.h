// Copyright 2020 Novozhilova Ekaterina
#ifndef MODULES_TASK_3_NOVOZHILOVA_E_CANNON_S_ALGORITHM_CANNON_S_ALGORITHM_H_
#define MODULES_TASK_3_NOVOZHILOVA_E_CANNON_S_ALGORITHM_CANNON_S_ALGORITHM_H_
#include <vector>

std::vector<double> GenMatrix(int size);
std::vector<double> SeqMultiply(std::vector<double> A, std::vector<double> B, int size);
std::vector<double> CannonAlgorithm(std::vector<double> A, std::vector<double> B, int size);

#endif  // MODULES_TASK_3_NOVOZHILOVA_E_CANNON_S_ALGORITHM_CANNON_S_ALGORITHM_H_
