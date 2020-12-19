// Copyright 2020 Makarychev Sergey
#ifndef  MODULES_TASK_3_MAKARYCHEV_S_FOXES_ALGORITHM_FOXES_ALGORITHM_H_
#define  MODULES_TASK_3_MAKARYCHEV_S_FOXES_ALGORITHM_FOXES_ALGORITHM_H_
#include <iostream>
#include <vector>

std::vector<double> getRandomMatrix(int orderM);
void BlockMult(double* pAblock, double* pBblock, double* pCblock, int blockSize);
std::vector<double> seqMult(const std::vector<double>& matA, const std::vector<double>& matB, int size);
std::vector<double> foxsAlgorithm(const std::vector<double>& matA, const std::vector<double>& matB, int orderM);
bool compareMat(const std::vector<double>& matrixA, const std::vector<double>& matrixB);

#endif  //  MODULES_TASK_3_MAKARYCHEV_S_FOXES_ALGORITHM_FOXES_ALGORITHM_H_
