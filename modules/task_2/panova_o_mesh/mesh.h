// Copyright 2020 Panova Olga
#ifndef MODULES_TASK_2_PANOVA_O_MESH_MESH_H_
#define MODULES_TASK_2_PANOVA_O_MESH_MESH_H_
MPI_Comm CreateMesh(int ndims);
int GetNum(int size);
int SendData(void *buf, int count, MPI_Datatype type, int *dest, MPI_Comm comm);
int RecvData(void *buf, int count, MPI_Datatype type, int *source, MPI_Comm comm, MPI_Status *status);
void SendRecvIntData(int s_buf, int ndims, int s_rank, int f_rank, int* f_buf);
void SendRecvDoubleData(double s_buf, int ndims, int s_rank, int f_rank, double* f_buf);
void SendRecvPackedData(int int_data, double doub_data, double doub_data1, int ndims, int s_rank, int f_rank, bool was);
#endif  // MODULES_TASK_2_PANOVA_O_MESH_MESH_H_
