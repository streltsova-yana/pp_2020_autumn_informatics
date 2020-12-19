// Copyright 2020 Gushchin Artem
#include <mpi.h>
#include <random>
#include <vector>
#include <algorithm>
#include "../../../modules/task_3/gushchin_a_radix_with_merge/radix_with_merge.h"

std::vector<int> parallelRadixSort(const std::vector<int>& source, const int sourceSize) {
    if (sourceSize == 0)
        return std::vector<int>();

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    const int delta = sourceSize / size;
    const int remainder = sourceSize % size;

    std::vector<int> displaces(size), sendCounts(size);
    int displacesCount = 0;

    for (int i = 0; i < size; i++) {
        if (i < remainder)
            sendCounts[i] = delta + 1;
        else
            sendCounts[i] = delta;

         displaces[i] = displacesCount;
         displacesCount += sendCounts[i];
    }

    std::vector<int> localVector(sendCounts[rank]);

    MPI_Scatterv(source.data(), sendCounts.data(), displaces.data(), MPI_INT,
        localVector.data(), sendCounts[rank], MPI_INT, 0, MPI_COMM_WORLD);

    localVector = radixSortSigned(localVector);

    std::vector<int> procs(size);
    for (int i = 0; i < static_cast<int>(procs.size()); i++)
        procs[i] = i;

    int vecSize = 0;

    while (procs.size() > 1) {
        vecSize = static_cast<int>(procs.size());
        for (int i = vecSize - 1; i > (vecSize % 2); i -= 2) {
            if (rank == procs[i]) {
                int mergedSize = static_cast<int>(localVector.size());
                MPI_Send(&mergedSize, 1, MPI_INT, procs[static_cast<size_t>(i) - 1], 0, MPI_COMM_WORLD);

                MPI_Send(localVector.data(), mergedSize, MPI_INT, procs[static_cast<size_t>(i) - 1], 0, MPI_COMM_WORLD);
            } else if (rank == procs[static_cast<size_t>(i) - 1]) {
                int mergeSize = 0;
                MPI_Status status;
                MPI_Recv(&mergeSize, 1, MPI_INT, procs[i], 0, MPI_COMM_WORLD, &status);

                std::vector<int> mergeVector(mergeSize);
                MPI_Recv(mergeVector.data(), mergeSize, MPI_INT, procs[i], 0, MPI_COMM_WORLD, &status);

                localVector = mergeVectors(localVector, mergeVector);
            }

            procs.erase(procs.begin() + i);
        }
    }

    return localVector;
}
