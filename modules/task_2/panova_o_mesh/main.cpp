// Copyright 2020 Panova Olga
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <mpi.h>
#include <iostream>
#include "../../../modules/task_2/panova_o_mesh/mesh.h"
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
TEST(Transmission, SendDataPacked_FromLastTo1_Dims2) {
    int ndims = 2;
    int int_data = 12345;
    double doub_data = 12.00045389;
    double doub_data1 = 1.4512654687;
    MPI_Comm test_comm = CreateMesh(ndims);
    int rank, size;
    MPI_Comm_rank(test_comm, &rank);
    MPI_Comm_size(test_comm, &size);
    if (size == 1) {
        ASSERT_EQ(1, 1);
    } else {
        bool was = true;
        int f_rank = 0;
        int s_rank = size - 1;
        SendRecvPackedData(int_data, doub_data, doub_data1, ndims, s_rank, f_rank, was);
        if (rank == f_rank) {
            ASSERT_EQ(was, true);
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
TEST(Transmission, SendDataDouble_FromAToB_Dims4) {
    double s_buf = 12.00045389;
    double* f_buf = new double[100];
    int ndims = 4;
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
        SendRecvDoubleData(s_buf, ndims, A, B, f_buf);
        if (rank == B) {
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
