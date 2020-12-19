// Copyright 2020 Panova Olga
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "../../../modules/task_2/panova_o_mesh/mesh.h"
TEST(Creature, TheMeshWasCreatedCorrectly) {
    int rank, size;
    int dims[2], coords[2], periods[2];
    MPI_Comm test_comm = CreateMesh(2);
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    int ndims;
    MPI_Cartdim_get(test_comm, &ndims);
    MPI_Cart_get(test_comm, ndims, dims, periods, coords);
    EXPECT_TRUE(periods[0] & periods[1]);
    ASSERT_EQ(ndims, 2);
}
TEST(Transmission, SendDataInt_From1ToLast_Dims2) {
    int s_buf = 12345;
    int* f_buf = new int[100];
    int ndims = 2;
    MPI_Comm test_comm = CreateMesh(ndims);
    int rank, size;
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        int s_rank = 0;
        int f_rank = size - 1;
        SendRecvIntData(s_buf, ndims, s_rank, f_rank, f_buf);
        if (rank == f_rank) {
            ASSERT_EQ(s_buf, f_buf[0]);
        }
    }
}
TEST(Transmission, SendDataInt_FromLastTo1_Dims2) {
    int s_buf = 12345;
    int* f_buf = new int[100];
    int ndims = 2;
    MPI_Comm test_comm = CreateMesh(ndims);
    int rank, size;
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        int f_rank = 0;
        int s_rank = size - 1;
        SendRecvIntData(s_buf, ndims, s_rank, f_rank, f_buf);
        if (rank == f_rank) {
            ASSERT_EQ(s_buf, f_buf[0]);
        }
    }
}
TEST(Transmission, SendDataInt_FromAToB_Dims2) {
    int s_buf = 12345;
    int* f_buf = new int[100];
    int ndims = 2;
    MPI_Comm test_comm = CreateMesh(ndims);
    int rank, size;
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        int A = GetNum(size);
        int B = 0;
        do {
            B = GetNum(size);
        } while (B == A);
        SendRecvIntData(s_buf, ndims, A, B, f_buf);
        if (rank == B) {
            ASSERT_EQ(s_buf, f_buf[0]);
        }
    }
}
TEST(Transmission, SendDataDouble_From1ToLast_Dims3) {
    double s_buf = 12.00045389;
    double* f_buf = new double[100];
    int ndims = 3;
    MPI_Comm test_comm = CreateMesh(ndims);
    int rank, size;
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        int s_rank = 0;
        int f_rank = size - 1;
        SendRecvDoubleData(s_buf, ndims, s_rank, f_rank, f_buf);
        if (rank == f_rank) {
            ASSERT_DOUBLE_EQ(s_buf, f_buf[0]);
        }
    }
}
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);
    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& lst = ::testing::UnitTest::GetInstance()->listeners();
    lst.Release(lst.default_result_printer());
    lst.Release(lst.default_xml_generator());
    lst.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
