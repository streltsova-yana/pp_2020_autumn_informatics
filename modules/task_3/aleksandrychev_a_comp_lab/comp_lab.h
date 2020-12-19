// Copyright 2020 Alekasndrychev Andrey
#ifndef MODULES_TASK_3_ALEKSANDRYCHEV_A_COMP_LAB_COMP_LAB_H_
#define MODULES_TASK_3_ALEKSANDRYCHEV_A_COMP_LAB_COMP_LAB_H_

#include <vector>

std::vector<uint32_t> getRandomBinaryImage(int height, int width);
std::vector<uint32_t> markComponentsNotParall(std::vector<uint32_t> image, int height, int width);
std::vector<uint32_t> markComponents(std::vector<uint32_t> image, int height, int width);

#endif  // MODULES_TASK_3_ALEKSANDRYCHEV_A_COMP_LAB_COMP_LAB_H_
