// Copyright 2020 Chistov Vladimir

#include <mpi.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "../../../modules/task_3/chistov_v_gauss_block/gauss_block.h"

std::vector<double> Gauss_Sequential(std::vector<double> image, int width, int height) {
    std::vector<double> calc((width + 2) * (height + 2));
    for (int x = 0; x < width + 2; x++) {
        for (int y = 0; y < height + 2; y++) {
            if ((x == 0) || (y == 0) || (x == width + 1) || (y == height + 1)) {
                calc[x * (height + 2) + y] = 0;
            } else {
                calc[x * (height + 2) + y] = image[(x - 1) * height + y - 1];
            }
        }
    }
    int count = 0;
    std::vector<double> res(width * height);
    for (int x = 1; x < width + 1; x++) {
        for (int y = 1; y < height + 1; y++) {
            double sum = 0;
            for (int i = -1; i < 2; i++) {
                for (int j = -1; j < 2; j++) {
                    sum = sum + calc[(x + i) * height + y + j] * Gauss_Core[i + j + 2];
                }
            }
            res[count] = sum / 16;
            count++;
        }
    }
    return res;
}

std::vector<double> GenRandVec(int size) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));
    std::vector<double> vec(size);
    for (int i = 0; i < size; i++) {
        vec[i] = gen() % 256;
    }
    return vec;
}

std::vector <double> Block_Construct(std::vector<double> image, int num, int widthloc, int heightloc, int heigth) {
    std::vector<double> calc(widthloc * heightloc);
    std::vector<double> res(widthloc * heightloc);
    int a = 0;
    for (int i = 0; i < widthloc; i++) {
        for (int j = 0; j < heightloc; j++) {
            calc[a] = image[num + i * heigth + j];
            a++;
        }
    }
    a = 0;
    res = Gauss_Sequential(calc, widthloc, heightloc);
    return res;
}

std::vector <double> Block_Destruct(std::vector<double> empty, std::vector<double> block, int num,
int widthloc, int heightloc, int heigth) {
    int a = 0;
    for (int i = 0; i < widthloc; i++) {
        for (int j = 0; j < heightloc; j++) {
            empty[num + i * heigth + j] = block[a];
            a++;
        }
    }
    a = 0;
    return empty;
}

std::vector<double> Gauss_Parallel(std::vector<double> image, int width, int height) {
    int ProcNum, ProcRank;
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
    int sidenum = ceil(static_cast<double>(sqrt(ProcNum)));
    int blockwidth = width / sidenum;
    int blockwidthrem = width % sidenum;
    int blockheight = height / sidenum;
    int blockheightrem = height % sidenum;
    int ProcCalc = 0;
    int heightcalc, widthcalc, hrem, wrem;
    int locbh = 0;
    int locbw = 0;
    int start = 0;
    std::vector<double> loc_sum(width * height);
    std::vector<double> res(width * height);
    if (blockwidthrem > 0) {
        widthcalc = 1;
    } else {
        widthcalc = 0;
    }
    if (blockheightrem > 0) {
        heightcalc = 1;
    } else {
        heightcalc = 0;
    }
    wrem = blockwidthrem;
    for (int i = 0; i < sidenum; i++) {
        hrem = blockheightrem;
        if (wrem < 1) {
            widthcalc = 0;
        } else {
            wrem--;
            widthcalc = 1;
        }
        locbw = blockwidth + widthcalc;
        for (int j = 0; j < sidenum; j++) {
            if (hrem < 1) {
                heightcalc = 0;
            } else {
                heightcalc = 1;
                hrem--;
            }
            locbh = blockheight + heightcalc;
            if (ProcRank == ProcCalc) {
                auto block = Block_Construct(image, start, locbw, locbh, height);
                loc_sum = Block_Destruct(loc_sum, block, start, locbw, locbh, height);
            }
            start = start + locbh;
            ProcCalc++;
            if (ProcCalc > ProcNum - 1) {
                ProcCalc = 0;
            }
        }
        start = start + height * (locbw - 1);
    }
    MPI_Reduce(&loc_sum[0], &res[0], width * height, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    return res;
}

