// Copyright 2020 Kolesin Andrey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <cstring>
#include "../../../modules/task_1/kolesin_a_count_words/count_words.h"


std::string getRandomString(int words) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::string res = "";
    for (int  i = 0; i < words; i++) {
        int len = gen()%20 + 1;
        for (int j = 0; j < len; j++) {
            res += ('a' + gen()%26);
        }
        len = gen()%3 + 1;
        for (int j = 0; j < len; j++) {
            res += " ";
        }
    }
    return res;
}

int shareStrings(int rank, int size, std::string str, char** buff_out) {
    int n;
    if (rank == 0) {
        n = str.size();
    }
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
    if (n <= size*2) {
        return -1;
    }
    int delta = n/size;
    int remainder = n - delta*size;
    int buffsize =   delta + remainder;

    char* buff = new char[buffsize+1];
    MPI_Scatter(str.c_str(), delta, MPI_CHAR, buff, delta, MPI_CHAR, 0, MPI_COMM_WORLD);
    if (remainder != 0) {
        if (rank == 0) {
            MPI_Send(str.c_str()+size*delta, remainder, MPI_CHAR, size-1, 0, MPI_COMM_WORLD);
        } else if (rank == size-1) {
            MPI_Status status;
            MPI_Recv(buff+delta, remainder, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        }
    }
    if (rank == size-1) {
        buff[buffsize] = 0;
    } else {
        buff[delta] = 0;
    }
    *buff_out = buff;
    return 0;
}
int count(const char* str) {
    int i = -1;
    int res = 0;
    bool prevIsSpace = true;
    while (str[i+1] != 0) {
        i++;
        if (prevIsSpace == (str[i] == ' ')) {
            continue;
        }
        if (str[i] != ' ') {
            res += 1;
        }
        prevIsSpace = (str[i] == ' ');
    }
    return res;
}
int countWords(std::string str) {
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char* buff;
        if (shareStrings(rank, size, str, &buff) == -1) {
        if (rank == 0) {
            return count(str.c_str());
        }
        return 0;
    }
    int local_sum = count(buff);
    int* result_array = new int[size];
    MPI_Gather(&local_sum, 1, MPI_INT, result_array, 1, MPI_INT, 0, MPI_COMM_WORLD);
    int result = 0;
    for (int i = 0; i < size; i++) {
        result += result_array[i];
    }
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            int p1 = str.size()/size*i;
            int p2 = p1 - 1;
            if (str[p2] != ' ' &&  str[p1] != ' ') {
                result--;
            }
        }
    }
    delete[] buff;
    delete[] result_array;
    return result;
}
