// Copyright 2020 Yurin Stanislav
#include <mpi.h>
#include <vector>
#include <numeric>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_3/yurin_s_shortest_path_by_Moore/shortest_path_by_Moore.h"

#define POS_INF_IMIT 10000

unsigned int time_increase = static_cast<unsigned int>(time(0));

unsigned int get_random_time() {
    return time_increase++;
}

int getRandomNumber(int min, int max) {
    std::mt19937 gen;
    gen.seed(get_random_time());

    return gen() % ((max + 1) - min) + min;
}

std::vector<int> getRandomWeightMatrix(int rows_num, int min, int max) {
    std::vector<int> weight_matrix(rows_num * rows_num);

    // !!!Заполняем нижнюю половину и главную диагональ бесконечностями
    for (int i = 0; i < rows_num; i++)
        for (int j = 0; j < (i + 1); j++)
            weight_matrix[(i * rows_num) + j] = POS_INF_IMIT;
    // Заполняем нижнюю половину и главную диагональ бесконечностями!!!

    // !!!Заполняем верхнюю половину рандомными числами от min до max
    for (int i = 0; i < rows_num; i++)
        for (int j = i + 1; j < rows_num; j++)
            weight_matrix[(i * rows_num) + j] =
                getRandomNumber(min, max);
    // Заполняем верхнюю половину рандомными числами от min до max!!!

    return weight_matrix;
}

std::vector<int> getSequentialShortestPath(std::vector<int> weight_matrix, int rows_num,
                                            int start_vert_index, int end_vert_index) {
    // !!!Заполняем все метки всех вершин кроме начальной POS_INF_IMI'ами,
    // а метку начальной - нулем
    std::vector<int> marks;
    marks.push_back(0);
    for (int i = 1; i < rows_num; i++)
        marks.push_back(POS_INF_IMIT);
    // Заполняем все метки всех вершин кроме начальной POS_INF_IMI'ами,
    // а метку начальной - нулем!!!

    // !!!Записываем в очередь начальую вершину
    std::vector<int> queue;
    queue.push_back(start_vert_index);
    // Записываем в очередь начальую вершину!!!


    // !!!Корректировка меток в очереди
    while (1) {
        int cur_vert_index = queue[0];  // Устанавливаем текущую вершину - первую в очереди

        queue.erase(queue.begin());  // Удаляем первую вершину из очереди

        int neighb_verts_num = (rows_num - 1) - cur_vert_index;


        // !!!Пересчитываем метки для всех достижимых вершин
        for (int i = 0; i < neighb_verts_num; i++) {
            int old_mark_val = marks[(cur_vert_index + 1) + i];  // Запоминаем старое значение метки

            // !!!Пересчитываем метку
            marks[(cur_vert_index + 1) + i] = std::min(old_mark_val,
                                                    marks[cur_vert_index] +
                                                        weight_matrix[((cur_vert_index + 1) + i) +
                                                                        (cur_vert_index *
                                                                        rows_num)]);
            // Пересчитываем метку!!!

            // !!!Корректируем очередь
            if (marks[(cur_vert_index + 1) + i] < old_mark_val) {
                if (old_mark_val < POS_INF_IMIT) {
                    // !!!Если вершина уже была в очереди или сейчас в ней
                    queue.erase(std::remove(queue.begin(), queue.end(), (cur_vert_index + 1) + i),
                        queue.end());
                    queue.insert(queue.begin(), (cur_vert_index + 1) + i);
                    // Если вершина уже была в очереди или сейчас в ней!!!
                } else {
                    // !!!Если вершина ранее не была в очереди
                    queue.push_back((cur_vert_index + 1) + i);
                    // Если вершина ранее не была в очереди!!!
                }
            }
            // Корректируем очередь!!!
        }
        // Пересчитываем метки для всех достижимых вершин!!!

        // !!!Проверка очереди на пустоту. Если очередь пуста - катчайшие пути до всех вершин найдены
        if (queue.empty())
            break;
        // Проверка очереди на пустоту. Если очередь пуста - катчайшие пути до всех вершин найдены!!!
    }
    // Корректировка меток в очереди!!!

    // !!!Построение кратчайшего пути
    std::vector<int> result_vec;
    result_vec.push_back(end_vert_index);

    int cur_vert_index = end_vert_index;
    int prev_vert_val;
    while (1) {
        for (int i = 0; i < rows_num; i++) {
            prev_vert_val = weight_matrix[cur_vert_index + (i * rows_num)];
            if (prev_vert_val < POS_INF_IMIT) {
                if ((marks[i] + prev_vert_val) == marks[cur_vert_index]) {
                    result_vec.push_back(i);
                    cur_vert_index = i;
                    break;
                }
            }
        }
        if (cur_vert_index == start_vert_index)
            break;
    }
    // Построение кратчайшего пути!!!

    std::reverse(std::begin(result_vec), std::end(result_vec));
    return result_vec;
}


std::vector<int> getParallelShortestPath(std::vector<int> weight_matrix, int rows_num,
                                            int start_vert_index, int end_vert_index) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // !!!Передача матрицы от нулевого процесса остальным
    MPI_Bcast(weight_matrix.data(), weight_matrix.size(), MPI_INT, 0, MPI_COMM_WORLD);
    // Передача матрицы от нулевого процесса остальным!!!

    // Полный массив меток и полная очередь хранятся только в нулевом процессе
    std::vector<int> marks;
    std::vector<int> overall_to_queue_start, overall_to_queue_end;
    if (rank == 0) {
        // !!!Заполняем все метки всех вершин кроме начальной POS_INF_IMI'ами,
        // а метку начальной - нулем
        marks.push_back(0);
        for (int i = 1; i < rows_num; i++)
            marks.push_back(POS_INF_IMIT);
        // Заполняем все метки всех вершин кроме начальной POS_INF_IMI'ами,
        // а метку начальной - нулем!!!

        // !!!Записываем в очередь начальую вершину
        overall_to_queue_start.push_back(start_vert_index);
        // Записываем в очередь начальую вершину!!!
    }

    // !!!Корректировка меток в очереди
    while (1) {
        int cur_vert_index;
        int cur_vert_mark;
        if (rank == 0) {
            if (!overall_to_queue_start.empty()) {
                cur_vert_index = overall_to_queue_start[0];  // Устанавливаем текущую вершину - первую в очереди
                overall_to_queue_start.erase(overall_to_queue_start.begin());  // Удаляем первую вершину из очереди
            } else {
                cur_vert_index = overall_to_queue_end[0];  // Устанавливаем текущую вершину - первую в очереди
                overall_to_queue_end.erase(overall_to_queue_end.begin());  // Удаляем первую вершину из очереди
            }
            cur_vert_mark = marks[cur_vert_index];
        }

        MPI_Bcast(&cur_vert_index, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cur_vert_mark, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int cur_vert_neighbs_num, one_proc_neighb_verts_num_int, overload_procs_num;
        std::vector<int> each_proc_verts_num, displs, each_proc_start_neighb_vert_index;

        if (rank == 0) {
            cur_vert_neighbs_num = (rows_num - 1) - cur_vert_index;
            one_proc_neighb_verts_num_int = cur_vert_neighbs_num / size;
            overload_procs_num = cur_vert_neighbs_num % size;

            for (int i = 0; i < size; i++)
                if (i < overload_procs_num)
                    each_proc_verts_num.push_back(one_proc_neighb_verts_num_int + 1);
                else
                    each_proc_verts_num.push_back(one_proc_neighb_verts_num_int);

            displs.push_back(0);
            for (int i = 1; i < size; i++)
                displs.push_back(displs[i - 1] + each_proc_verts_num[i - 1]);

            each_proc_start_neighb_vert_index.push_back(cur_vert_index + 1);
            for (int i = 1; i < size; i++)
                each_proc_start_neighb_vert_index.push_back(each_proc_start_neighb_vert_index[i - 1]
                                                            + each_proc_verts_num[i - 1]);
        }

        // !!!Рассылаем количество обрабатываемых вершин для каждого процесса
        int this_proc_neighb_verts_num;
        MPI_Scatter(each_proc_verts_num.data(), 1, MPI_INT,
                    &this_proc_neighb_verts_num, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // Рассылаем количество обрабатываемых вершин для каждого процесса!!!

        // !!!Рассылаем индексы обрабатываемых вершин для каждого процесса
        int this_proc_start_neighb_vert_index;
        MPI_Scatter(each_proc_start_neighb_vert_index.data(), 1, MPI_INT,
                        &this_proc_start_neighb_vert_index, 1, MPI_INT, 0, MPI_COMM_WORLD);
        // Рассылаем индексы обрабатываемых вершин для каждого процесса!!!

        int *start_marks_index;

        if (rank == 0) {
            if ((cur_vert_index + 1) < static_cast<int>(marks.size()))
                start_marks_index = &marks[cur_vert_index + 1];
            else
                start_marks_index = nullptr;
        } else {
            start_marks_index = nullptr;
        }

        // !!!Рассылаем текущие значения меток для обрабатываемых вершин
        std::vector<int> this_proc_marks(this_proc_neighb_verts_num);
        MPI_Scatterv(start_marks_index, each_proc_verts_num.data(), displs.data(), MPI_INT,
                        this_proc_marks.data(), this_proc_neighb_verts_num, MPI_INT, 0, MPI_COMM_WORLD);
        // Рассылаем текущие значения меток для обрабатываемых вершин!!!

        // !!!Создаем промежуточные переменные для каждого процесса,
        // которые потом будем передавать в нулевой процесс
        std::vector<int> to_queue_start;
        std::vector<int> to_queue_end;
        // Создаем промежуточные переменные для каждого процесса,
        // которые потом будем передавать в нулевой процесс!!!

        // !!!Пересчитываем метки для всех достижимых вершин
        for (int i = 0; i < this_proc_neighb_verts_num; i++) {
            int old_mark_val = this_proc_marks[i];  // Запоминаем старое значение метки

            // !!!Пересчитываем метку
            this_proc_marks[i] = std::min(old_mark_val, cur_vert_mark +
                                        weight_matrix[(cur_vert_index * rows_num) +
                                                        (this_proc_start_neighb_vert_index + i)]);
            // Пересчитываем метку!!!

            // !!!Корректируем очередь
            if (this_proc_marks[i] < old_mark_val) {
                if (old_mark_val < POS_INF_IMIT) {
                    // !!!Если вершина уже была в очереди или сейчас в ней
                    to_queue_start.push_back(this_proc_start_neighb_vert_index + i);
                    // Если вершина уже была в очереди или сейчас в ней!!!
                } else {
                    // !!!Если вершина ранее не была в очереди
                    to_queue_end.push_back(this_proc_start_neighb_vert_index + i);
                    // Если вершина ранее не была в очереди!!!
                }
            }
            // Корректируем очередь!!!
        }

        // !!!Отправляем из ненулевых процессов рассчитанные данные

        // !!!Собираем marks с каждого процесса в нулевом
        MPI_Gatherv(this_proc_marks.data(), this_proc_neighb_verts_num, MPI_INT,
                    start_marks_index, each_proc_verts_num.data(), displs.data(), MPI_INT, 0, MPI_COMM_WORLD);
        // Собираем marks с каждого процесса в нулевом!!!

        // !!!Собираем размеры to_queue_start и to_queue_end с каждого процесса в нулевом
        std::vector<int> to_queue_start_sizes, to_queue_end_sizes;
        if (rank == 0) {
            to_queue_start_sizes.resize(size);
            to_queue_end_sizes.resize(size);
        }

        int this_proc_to_queue_start_size = to_queue_start.size();
        int this_proc_to_queue_end_size  = to_queue_end.size();
        MPI_Gather(&this_proc_to_queue_start_size, 1, MPI_INT,
                    to_queue_start_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Gather(&this_proc_to_queue_end_size, 1, MPI_INT,
                    to_queue_end_sizes.data(), 1, MPI_INT, 0, MPI_COMM_WORLD);
        // Собираем размеры to_queue_start и to_queue_end с каждого процесса в нулевом!!!

        // !!!Собираем данные to_queue_start и to_queue_end с каждого процесса в нулевом
        std::vector<int> to_queue_start_displs, to_queue_end_displs;
        std::vector<int> tmp_overall_to_queue_start, tmp_overall_to_queue_end;

        if (rank == 0) {
            tmp_overall_to_queue_start.resize(std::accumulate(to_queue_start_sizes.begin(),
                                                            to_queue_start_sizes.end(), 0));
            tmp_overall_to_queue_end.resize(std::accumulate(to_queue_end_sizes.begin(),
                                                            to_queue_end_sizes.end(), 0));

            to_queue_start_displs.push_back(0);
            for (int i = 1; i < size; i++)
                to_queue_start_displs.push_back(to_queue_start_displs[i - 1] + to_queue_start_sizes[i - 1]);

            to_queue_end_displs.push_back(0);
            for (int i = 1; i < size; i++)
                to_queue_end_displs.push_back(to_queue_end_displs[i - 1] + to_queue_end_sizes[i - 1]);
        }

        MPI_Gatherv(to_queue_start.data(), to_queue_start.size(), MPI_INT,
                    tmp_overall_to_queue_start.data(), to_queue_start_sizes.data(),
                    to_queue_start_displs.data(), MPI_INT,
                    0, MPI_COMM_WORLD);

        MPI_Gatherv(to_queue_end.data(), to_queue_end.size(), MPI_INT,
                    tmp_overall_to_queue_end.data(), to_queue_end_sizes.data(), to_queue_end_displs.data(), MPI_INT,
                    0, MPI_COMM_WORLD);

        for (int i = 0; i < static_cast<int>(tmp_overall_to_queue_start.size()); i++) {
            overall_to_queue_start.erase(std::remove(overall_to_queue_start.begin(),
                                                        overall_to_queue_start.end(),
                                                        tmp_overall_to_queue_start[i]),
                                                            overall_to_queue_start.end());
            overall_to_queue_end.erase(std::remove(overall_to_queue_end.begin(),
                                                        overall_to_queue_end.end(),
                                                        tmp_overall_to_queue_start[i]),
                                                            overall_to_queue_end.end());
        }
        overall_to_queue_start.insert(overall_to_queue_start.end(),
                                        tmp_overall_to_queue_start.begin(),
                                        tmp_overall_to_queue_start.end() );

        overall_to_queue_end.insert(overall_to_queue_end.end(),
                                        tmp_overall_to_queue_end.begin(),
                                        tmp_overall_to_queue_end.end() );

        int break_flag = 0;
        if (rank == 0) {
            // !!!Проверка очереди на пустоту
            if (overall_to_queue_end.empty() && overall_to_queue_start.empty())
                break_flag = 1;
            // Проверка очереди на пустоту!!!
        }
        MPI_Bcast(&break_flag, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (break_flag == 1)
            break;
    }

    // !!!Построение кратчайшего пути
    std::vector<int> result_vec;
    if (rank == 0) {
        result_vec.push_back(end_vert_index);

        int cur_vert_index = end_vert_index;
        int prev_vert_val;
        while (1) {
            for (int i = 0; i < rows_num; i++) {
                prev_vert_val = weight_matrix[cur_vert_index + (i * rows_num)];
                if (prev_vert_val < POS_INF_IMIT) {
                    if ((marks[i] + prev_vert_val) == marks[cur_vert_index]) {
                        result_vec.push_back(i);
                        cur_vert_index = i;
                        break;
                    }
                }
            }
            if (cur_vert_index == start_vert_index)
                break;
        }
        std::reverse(std::begin(result_vec), std::end(result_vec));
    }
    // Построение кратчайшего пути!!!
    return result_vec;
}
