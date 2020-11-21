// Copyright 2020 Panova Olga
#include <mpi.h>
#include <iostream>
#include <ctime>
#include <random>
#include "../../../modules/task_2/panova_o_mesh/mesh.h"
int GetNum(int size) {
    std::mt19937 gen(time(0));
    gen.seed(static_cast<unsigned int>(time(0)));
    int num;
    num = gen() % size;
    return num;
}
MPI_Comm CreateMesh(int ndims) {
    int proc_size;
    int proc_rank;
    int* dims = new int[ndims]();
    int* periods = new int[ndims];
    for (int i = 0; i < ndims; i++) {
        periods[i] = 1;
    }
    MPI_Comm test_comm;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);
    MPI_Dims_create(proc_size, ndims, dims);
    MPI_Cart_create(MPI_COMM_WORLD, ndims, dims, periods, 0, &test_comm);
    return test_comm;
}
int SendData(void* buf, int count, MPI_Datatype type, int *dest, MPI_Comm test_comm) {
    int res = 0, rank = 0;
    MPI_Request request;
    MPI_Cart_rank(test_comm, dest, &rank);
    res = MPI_Isend(buf, count, type, rank, 0, test_comm, &request);
    return res;
}
int RecvData(void* buf, int count, MPI_Datatype type, int *source, MPI_Comm test_comm, MPI_Status* status) {
    int res = 0, rank = 0;
    MPI_Cart_rank(test_comm, source, &rank);
    res = MPI_Recv(buf, count, type, rank, 0, test_comm, status);
    return res;
}
void SendRecvIntData(int s_buf, int ndims, int s_rank, int f_rank, int* f_buf) {
    int rank = 0, size = 0;
    MPI_Comm test_comm = CreateMesh(ndims);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int* s_coords = new int[ndims];
    MPI_Cart_coords(test_comm, s_rank, ndims, s_coords);
    int* f_coords = new int[ndims];
    MPI_Cart_coords(test_comm, f_rank, ndims, f_coords);
    if (rank == s_rank)
        SendData(&s_buf, 1, MPI_INT, f_coords, test_comm);
    else if (rank == f_rank)
        RecvData(&f_buf[0], 1, MPI_INT, s_coords, test_comm, &status);
}
void SendRecvDoubleData(double s_buf, int ndims, int s_rank, int f_rank, double* f_buf) {
    int rank = 0, size = 0;
    MPI_Comm test_comm = CreateMesh(ndims);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int* s_coords = new int[ndims];
    MPI_Cart_coords(test_comm, s_rank, ndims, s_coords);
    int* f_coords = new int[ndims];
    MPI_Cart_coords(test_comm, f_rank, ndims, f_coords);
    if (rank == s_rank)
        SendData(&s_buf, 1, MPI_DOUBLE, f_coords, test_comm);
    else if (rank == f_rank)
        RecvData(&f_buf[0], 1, MPI_DOUBLE, s_coords, test_comm, &status);
}
void SendRecvPackedData(int int_data, double doub_data, double doub_data1,
    int ndims, int s_rank, int f_rank, bool was) {
    int rank = 0, size = 0;
    MPI_Comm test_comm = CreateMesh(ndims);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int bufpos = 0;
    void* buf[1000];
    MPI_Status status;
    int* s_coords = new int[ndims];
    MPI_Cart_coords(test_comm, s_rank, ndims, s_coords);
    int* f_coords = new int[ndims];
    MPI_Cart_coords(test_comm, f_rank, ndims, f_coords);
    if (rank == s_rank) {
        MPI_Pack(&int_data, 1, MPI_INT, buf, 1000, &bufpos, test_comm);
        MPI_Pack(&doub_data, 1, MPI_DOUBLE, buf, 1000, &bufpos, test_comm);
        MPI_Pack(&doub_data1, 1, MPI_DOUBLE, buf, 1000, &bufpos, test_comm);
        SendData(&buf, bufpos, MPI_PACKED, f_coords, test_comm);
    } else if (rank == f_rank) {
        RecvData(&buf, 20, MPI_PACKED, s_coords, test_comm, &status);
        bufpos = 0;
        int f_int_data = 0;
        double f_doub_data = 0;
        double f_doub_data1 = 0;
        MPI_Unpack(buf, 1000, &bufpos, &f_int_data, 1, MPI_INT, test_comm);
        MPI_Unpack(buf, 1000, &bufpos, &f_doub_data, 1, MPI_DOUBLE, test_comm);
        MPI_Unpack(buf, 1000, &bufpos, &f_doub_data1, 1, MPI_DOUBLE, test_comm);
        if (f_int_data == 0 || f_doub_data == 0 || f_doub_data1 == 0) was = false;
    }
}
