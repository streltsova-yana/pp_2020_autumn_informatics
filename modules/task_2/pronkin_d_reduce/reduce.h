// Copyright 2020 Pronkin Dmitry
#ifndef MODULES_TASK_2_PRONKIN_D_REDUCE_REDUCE_H_
#define MODULES_TASK_2_PRONKIN_D_REDUCE_REDUCE_H_

#include <mpi.h>

int Reduce(void *sendbuf, void *recvbuf, int count, MPI_Datatype type, MPI_Op op, int root, MPI_Comm comm);

#endif  // MODULES_TASK_2_PRONKIN_D_REDUCE_REDUCE_H_
