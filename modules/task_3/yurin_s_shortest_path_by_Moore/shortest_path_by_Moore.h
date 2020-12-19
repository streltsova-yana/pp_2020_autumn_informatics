// Copyright 2020 Yurin Stanislav
#ifndef MODULES_TASK_3_YURIN_S_SHORTEST_PATH_BY_MOORE_SHORTEST_PATH_BY_MOORE_H_
#define MODULES_TASK_3_YURIN_S_SHORTEST_PATH_BY_MOORE_SHORTEST_PATH_BY_MOORE_H_

#include <vector>
#include <string>

unsigned int get_random_time();
int getRandomNumber(int min, int max);
std::vector<int> getRandomWeightMatrix(int num_of_rows, int min, int max);
std::vector<int> getSequentialShortestPath(std::vector<int> weight_matrix, int num_of_rows,
                                            int start_vert_index, int end_vert_index);
std::vector<int> getParallelShortestPath(std::vector<int> weight_matrix, int num_of_rows,
                                            int start_vert_index, int end_vert_index);
#endif  // MODULES_TASK_3_YURIN_S_SHORTEST_PATH_BY_MOORE_SHORTEST_PATH_BY_MOORE_H_
