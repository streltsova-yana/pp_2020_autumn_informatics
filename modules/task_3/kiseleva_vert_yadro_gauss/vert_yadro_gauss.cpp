// Copyright 2020 Kiseleva Anastasia
#include <mpi.h>
#include <vector>
#include <random>
#include <iostream>
#include <ctime>
#include <cmath>
#include "../../../modules/task_3/kiseleva_vert_yadro_gauss/vert_yadro_gauss.h"

int check(int v, int max, int min) {
    if (v > max) {
        return max;
    } else {
        if (v < min) {
            return min;
        }
    }
    return v;
}

std::vector<double> random(int str, int stlb) {
    std::vector<double> res(str * stlb);
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    for (int i = 0; i < str * stlb; i++)
        res[i] = gen() % 256;
    return res;
}

std::vector<double> transp(const std::vector<double>& image, int str, int stlb) {
    std::vector<double> res(str * stlb);
    for (int i = 0; i < str; i++) {
        for (int j = 0; j < stlb; j++) {
            res[i + j * str] = image[i * stlb + j];
        }
    }
    return res;
}

std::vector<double> yadro(int sigma) {
    double norma = 0;
    std::vector<double> yadroo(9);
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            int idx = (i + 1) * 3 + (j + 1);
            yadroo[idx] = exp(-(i * i + j * j) / (sigma * sigma));
            norma += yadroo[idx];
        }
    }
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            yadroo[i * 3 + j] /= norma;
        }
    }
    return yadroo;
}

std::vector<double> posled(const std::vector<double>& image, int xx, int xmax,
    int str, int stlb, int size_, int sigma) {
    double color = 0;
    std::vector<double> res(size_);
    std::vector<double> yadroo = yadro(sigma);
    for (int x = xx; x < xmax; x++) {
        for (int y = 0; y < stlb; y++) {
            color = 0;
            for (int i = -1; i <= 1; i++) {
                for (int j = -1; j <= 1; j++) {
                    int idx = (i + 1) * 3 + j + 1;
                    color += image[check(x + j, str - 1, 0) * stlb + check(y + i, stlb - 1, 0)] * yadroo[idx];
                }
            }
            res[(x-xx) * stlb + y] = check(color, 255, 0);
        }
    }
    return res;
}

std::vector<double> parallel(const std::vector<double>& image, int str, int stlb, int sigma) {
    int size;
    int rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    std::vector<double> res(str * stlb);
    if (size > str - 1) {
        MPI_Comm comm_world;
        MPI_Group group_world;
        comm_world = MPI_COMM_WORLD;
        MPI_Comm_group(comm_world, &group_world);
        int *members = new int[str];
        for (int i = 0; i < str; i++) {
            members[i] = i;
        }
        MPI_Group newworld;
        MPI_Group_incl(group_world, str, members, &newworld);
        MPI_Comm comm_newworld;
        MPI_Comm_create(comm_world, newworld, &comm_newworld);
        MPI_Group_rank(newworld, &rank);
        int size_;
        MPI_Group_size(newworld, &size_);
        if (comm_newworld != MPI_COMM_NULL) {
            int xx;
            int r;
            std::vector<double> local_res(stlb);
            if ((rank == 0) || (rank == str - 1)) {
                r = stlb * 2;
            } else {
                r = stlb * 3;
                xx = 1;
            }
            std::vector<double> local(r);
            if (rank == 0) {
                xx = 0;
            }
            if (rank == str - 1) {
                xx = 1;
            }
            if (rank == 0) {
                for (int i = 0; i < stlb*2; i++) {
                    local[i] = image[i];
                }
                for (int i = 1; i < size_ - 1; i++) {
                    MPI_Send(image.data() + stlb *(i-1), stlb*3, MPI_DOUBLE, i, 0, comm_newworld);
                }
                MPI_Send(image.data() + stlb *(size_-2), stlb*2, MPI_DOUBLE, size_-1, 1, comm_newworld);
            } else {
                if (rank != size_ - 1) {
                    MPI_Recv(local.data(), stlb*3, MPI_DOUBLE, 0, 0, comm_newworld, &status);
                } else {
                    MPI_Recv(local.data(), stlb*2, MPI_DOUBLE, 0, 1, comm_newworld, &status);
                }
            }
            local_res = posled(local, xx, xx+1, r/stlb, stlb, stlb, sigma);
            MPI_Gather(local_res.data(), stlb, MPI_DOUBLE, res.data(), stlb, MPI_DOUBLE, 0, comm_newworld);
        }
    } else {
        if (size == 1) {
            res = posled(image, 0, str, str, stlb, str*stlb, sigma);
        } else {
            int part = str / size;
         int ost = str % size;
         int r;
         if (ost == 0) {
             if ((rank == 0) || (rank == size - 1)) {
                 r = stlb * (part + 1);
             } else {
                 r = stlb * (part + 2);
             }
         } else {
             if (rank == 0) {
                 r = stlb * (part + 1);
             } else {
                 r = stlb * (part + 2);
             }
         }
            std::vector<double> local(r);
            std::vector<double> local_res(stlb*part);
            if (rank == 0) {
                for (int i = 0; i < stlb*(part+1); i++) {
                    local[i] = image[i];
                }
                if (ost == 0) {
                    for (int i = 1; i < size - 1; i++) {
                        MPI_Send(image.data() + stlb * (part - 1) + stlb * (i - 1)*part,
                            stlb*(part + 2), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                    }
                    MPI_Send(image.data() + stlb * (str - part - 1), stlb * (part + 1),
                        MPI_DOUBLE, size - 1, 1, MPI_COMM_WORLD);
                } else {
                    for (int i = 1; i < size; i++) {
                        MPI_Send(image.data() + stlb * (part - 1) + stlb * (i - 1)*part,
                            stlb*(part + 2), MPI_DOUBLE, i, 0, MPI_COMM_WORLD);
                    }
                }
            } else {
                if (ost == 0) {
                    if (rank != size - 1) {
                        MPI_Recv(local.data(), stlb *(part + 2), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);

                    } else {
                        MPI_Recv(local.data(), stlb *(part + 1), MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
                    }
                } else {
                    MPI_Recv(local.data(), stlb *(part + 2), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, &status);
                }
            }
            if (rank == 0) {
                local_res = posled(local, 0, part, part+1, stlb, stlb*part, sigma);
            } else {
                if (ost == 0) {
                    if (rank == size - 1) {
                        local_res = posled(local, 1, part + 1, part + 1, stlb, stlb*part, sigma);
                    } else {
                        local_res = posled(local, 1, part + 1, part + 2, stlb, stlb*part, sigma);
                    }
                } else {
                    local_res = posled(local, 1, part + 1, part + 2, stlb, stlb*part, sigma);
                }
            }
          MPI_Gather(local_res.data(), part*stlb, MPI_DOUBLE, res.data(), stlb*part, MPI_DOUBLE, 0, MPI_COMM_WORLD);
          if (rank == 0) {
              if (ost != 0) {
                  std::vector<double> lloc((ost+1)*stlb);
                  std::vector<double> local_ress(ost *stlb);
                  int f = 0;
                  for (int i = (part * size - 1)*stlb; i < str*stlb; i++) {
                      lloc[f] = image[i];
                      f++;
                  }
                  local_ress = posled(lloc, 1, ost+1, ost+1, stlb, stlb*ost, sigma);

                  for (int i = 0; i < stlb*ost; i++)
                      res[part * size * stlb + i] = local_ress[i];
              }
          }
        }
    }
    return res;
}
