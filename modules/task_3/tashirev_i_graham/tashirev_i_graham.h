// Copyright 2020 Tashirev Ivan
#ifndef MODULES_TASK_3_TASHIREV_I_GRAHAM_TASHIREV_I_GRAHAM_H_
#define MODULES_TASK_3_TASHIREV_I_GRAHAM_TASHIREV_I_GRAHAM_H_

#include <vector>

struct pixel {
    pixel() = default;
    pixel(int64_t x, int64_t y) : x(x), y(y) {}

    friend bool operator!=(pixel p1, pixel p2) {
        return !operator==(p1, p2);
    }
    friend bool operator==(pixel p1, pixel p2) {
        return p2.x == p1.x && p2.y == p1.y;
    }

    int64_t x;
    int64_t y;
};

int64_t rot(pixel a, pixel b, pixel c);
bool dist(pixel a, pixel b, pixel c);

std::vector<pixel> get_random_image(int width, int height, int* size);

std::vector<pixel> greh_sequential(std::vector <pixel> pixels);
std::vector<pixel> greh_parallel(std::vector <pixel> pixels, int size);

#endif  // MODULES_TASK_3_TASHIREV_I_GRAHAM_TASHIREV_I_GRAHAM_H_
