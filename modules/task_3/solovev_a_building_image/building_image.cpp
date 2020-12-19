// Copyright 2020 Solovev Aleksandr
#include <vector>
#include "../../../modules/task_3/solovev_a_building_image/building_image.h"

std::vector<Point> interpriate_basic(int ** image, int height, int width) {
    int count = 0;
     for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image[i][j] == 1)
            count++;
        }
    }
    std::vector<Point> result(count);
    count = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (image[i][j] == 1) {
            result[count].x = i;
            result[count].y = j;
            count++;
            }
        }
    }
    return result;
}

bool pointCheck(const Point& previous, const Point& current, const Point& potential) {
    double x;
    int potential_x = potential.x;
    int potential_y = potential.y;
    int prev_x = previous.x;
    int prev_y = previous.y;
    int curr_x = current.x;
    int curr_y = current.y;
    if (potential_x == curr_x) {
        if (potential_x == prev_x) {
            if (potential_y > curr_y) {
                if (potential_y > prev_y) {
                    return true;
                } else {
                    return false;
                }
            }
            if (potential_y < curr_y) {
                if (potential_y < prev_y) {
                    return true;
                } else {
                    return false;
                }
            }
        }
        x = potential_x;
    } else {
        if (potential_y == curr_y) {
            x = prev_x;
            if (potential_x > curr_x) {
                if (potential_y == prev_y) {
                    if (prev_x > potential_x) {
                        return false;
                    } else {
                        if ((prev_x == potential_x) && (potential_y > prev_y)) {
                            return false;
                        }
                        return true;
                    }
                } else {
                    return potential_y <= prev_y;
                }
            } else {
                if (potential_x < curr_x) {
                    if (potential_y == prev_y) {
                        if (prev_x > potential_x) {
                            return true;
                        } else {
                            if ((prev_x == potential_x) && (prev_y > potential_y)) {
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if (potential_y <= prev_y) {
                            return false;
                        } else {
                            return true;
                        }
                    }
                }
            }
        } else {
            double tan1 = (static_cast<double>(potential_y - curr_y)) /
            (static_cast<double>(potential_x - curr_x));
            double tan2 = tan1 + 1.0;
            if ((prev_x - curr_x) != 0) {
                tan2 = (prev_y - curr_y) / (prev_x - curr_x);
            }
            if (tan1 == tan2) {
                if (potential_y > curr_y) {
                    return potential_y > prev_y;
                } else {
                    return potential_y < prev_y;
                }
            } else {
                x = static_cast<double>(prev_y - potential_y) /
                tan1 + static_cast<double>(potential_x);
            }
        }
    }
    if (potential_y > curr_y) {
        if (potential_x > curr_x) {
            return !(x <= prev_x);
        } else {
            return x > prev_x;
        }
    } else {
        if ((potential_y - curr_y) != 0) {
            return x <= prev_x;
        }
    }
    return false;
}

size_t searchFirstPoint(const std::vector<Point>& mas) {
    size_t start = 0;
    for (int i = 0; i < static_cast<int>(mas.size()); i++) {
        if (mas[start].x > mas[i].x) {
            start = i;
        } else {
            if ((mas[start].x == mas[i].x) && (mas[start].y > mas[i].y)) {
                start = i;
            }
        }
    }
    return start;
}

Point searchFirstPointParallel(const std::vector<Point>& mas, int rank, int procCount) {
    Point local_right_point = mas[searchFirstPoint(mas)];
    std::vector<Point> local_points(procCount);
    MPI_Gather(&local_right_point, 2, MPI_INT, &local_points[0], 2, MPI_INT, 0, MPI_COMM_WORLD);
    if (rank == 0) {
        local_right_point = local_points[searchFirstPoint(local_points)];
    }
    MPI_Bcast(&local_right_point, 2, MPI_INT, 0, MPI_COMM_WORLD);
    return local_right_point;
}

std::vector<Point> buildConvexHullParallel(const std::vector<Point>& mas) {
    int rank, procCount;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &procCount);
    int elements_count = static_cast<int>(mas.size());
    if (elements_count == 1) {
        return mas;
    }
    if (elements_count == 2) {
        return mas;
    }
    int elements_part_count = elements_count / procCount;
    int remainder = elements_count % procCount;
    if ((elements_part_count == 1) || (static_cast<size_t>(procCount) > mas.size())) {
        if (rank == 0) {
            return buildConvexHull(mas);
        } else {
            return mas;
        }
    }
    int* recvcount = new int[procCount];
    int* delay = new int[procCount];
    recvcount[0] = 2 * (elements_part_count + remainder);
    delay[0] = 0;
    for (int i = 1; i < procCount; i++) {
        recvcount[i] = elements_part_count * 2;
    }
    for (int i = 1; i < procCount; i++) {
        delay[i] = (elements_part_count * i + remainder) * 2;
    }
    std::vector<Point> part_vector(0);
    if (rank == 0) {
        part_vector.resize(elements_part_count + remainder);
    } else {
        part_vector.resize(elements_part_count);
    }
    MPI_Scatterv(mas.data(), recvcount, delay, MPI_INT, part_vector.data(),
        recvcount[rank], MPI_INT, 0, MPI_COMM_WORLD);
    Point left_point = searchFirstPointParallel(part_vector, rank, procCount);
    std::vector<Point> convex_hull;
    convex_hull.emplace_back(left_point);
    std::vector<Point> local_points(procCount);
    Point next;
    do {
        next = part_vector[0];
        if (next == convex_hull.back()) {
            next = part_vector[1];
        }
        for (int i = 0; i < static_cast<int>(part_vector.size()); i++) {
            if ((part_vector[i] == convex_hull.back()) || (convex_hull.back() == next)) {
                continue;
            }
            if (pointCheck(convex_hull.back(), next, part_vector[i]) || (part_vector[i] == next)) {
                next = part_vector[i];
            }
        }
        MPI_Gather(&next, 2, MPI_INT, &local_points[0], 2, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank == 0) {
            for (int i = 0; i < static_cast<int>(local_points.size()); i++) {
                if (pointCheck(convex_hull.back(), next, local_points[i])) {
                    next = local_points[i];
                }
            }
        }
        MPI_Bcast(&next, 2, MPI_INT, 0, MPI_COMM_WORLD);
        convex_hull.emplace_back(next);
        if (convex_hull.size() == mas.size()) {
            break;
        }
    } while (convex_hull.back() != left_point);
    return convex_hull;
}

std::vector<Point> buildConvexHull(const std::vector<Point>& mas) {
    if ((mas.size() == 1) || (mas.size() == 2)) {
        return mas;
    }
    std::vector<Point> part_vector(mas);
    Point left_point = part_vector[searchFirstPoint(part_vector)];
    std::vector<Point> convex_hull;
    convex_hull.emplace_back(left_point);
    Point next;
    do {
        next = part_vector[0];
        if (next == convex_hull.back()) {
            next = part_vector[1];
        }
        for (int i = 0; i < static_cast<int>(part_vector.size()); i++) {
            if ((part_vector[i] == convex_hull.back()) || (convex_hull.back() == next)) {
                continue;
            }
            if ((part_vector[i] == next) || pointCheck(convex_hull.back(), next, part_vector[i])) {
                next = part_vector[i];
            }
        }
        convex_hull.push_back(next);
        if (convex_hull.size() == mas.size()) {
            break;
        }
    } while (convex_hull.back() != left_point);

    return convex_hull;
}
