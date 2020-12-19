// Copyright 2020 Alibekov Murad
#include <mpi.h>
#include <time.h>
#include <random>
#include <vector>
#include <algorithm>
#include <utility>
#include "../../../modules/task_3/alibekov_m_component_labeling/component_labeling.h"

std::vector<int> remarking(const std::vector<int>& image, int width, int height) {
    int size = width * height;
    std::vector<int> result(size);
    std::vector<int> last_labels(size / 2 + 1);
    std::vector<int> new_labels(size / 2 + 1);
    int max_label = 0;

    for (int i = 0; i < size; i++) {
        int pixel = image[i];
        if (pixel != 0) {
            int idx = -1;
            for (int k = 1; last_labels[k] != 0; k++) if (last_labels[k] == pixel) { idx = k; break; }
            if (idx == -1) {
                last_labels[++max_label] = pixel;
                new_labels[max_label] = max_label;
                result[i] = new_labels[max_label];
            } else {
                result[i] = new_labels[idx];
            }
        }
    }
    return result;
}


std::vector<int> generate_random_image(int width, int height) {
    std::mt19937 gen;
    gen.seed(static_cast<unsigned int>(time(0)));

    std::vector<int> image(height * width);
    for (int i = 0; i < height * width; ++i)
        image[i] = static_cast<int>(gen()) % 2;

    return image;
}


std::pair<std::vector<int>, std::pair<std::vector<int>, int> >
first_pass(const std::vector<int>& image, int width, int height, int begin_label) {
    int labels_count = 0;
    int size = width * height;
    std::vector<int> disjoint_sets(size);  // disjoint sets of labels
    std::vector<int> tmp_image(image.begin(), image.begin() + size);
    for (int x = 0; x < size; x++) disjoint_sets[x] = x + begin_label;

    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            int idx = row * width + column;
            if (tmp_image[idx] != 0) {
                // [0]      [A]
                // [B] [tmp_image[idx]]
                int A = idx < width ? 0 : tmp_image[idx - width];
                int B = ((idx < 1) || ((idx - 1) / width != row)) ? 0 : tmp_image[idx - 1];

                if ((A == 0) && (B == 0)) { tmp_image[idx] = idx + begin_label + 1; labels_count++; }
                if ((A == 0) && (B != 0)) tmp_image[idx] = B;
                if ((A != 0) && (B == 0)) tmp_image[idx] = A;
                if ((A != 0) && (B != 0)) {
                    if (A == B) {
                        tmp_image[idx] = A;
                    } else {
                        int root_max_AB = std::max(A, B);
                        while (disjoint_sets[root_max_AB - begin_label] != root_max_AB)
                            root_max_AB = disjoint_sets[root_max_AB - begin_label];

                        int root_min_AB = std::min(A, B);
                        while (disjoint_sets[root_min_AB - begin_label] != root_min_AB)
                            root_min_AB = disjoint_sets[root_min_AB - begin_label];

                        if (root_max_AB != root_min_AB) {
                            disjoint_sets[root_max_AB - begin_label] = root_min_AB;
                            labels_count--;
                        }

                        tmp_image[idx] = root_min_AB;
                    }
                }
            }
        }
    }
    return {tmp_image, {disjoint_sets, labels_count}};
}


std::vector<int> second_pass(std::vector<int> map, std::vector<int> disjoint_sets, int width, int height) {
    int size = width * height;
    std::vector<int> result(size);
    int pixels_count = size;
    int A = map[size - width];
    int B = map[size - 2];
    if ((A == 0) && (B == 0)) result[size - 1] = map[--pixels_count];
    if ((A != 0) && (B == 0)) result[size - 1] = A;
    if ((A == 0) && (B != 0)) result[size - 1] = B;
    if ((A != 0) && (B != 0)) result[size - 1] = std::min(A, B);

    for (int idx = 0; idx < pixels_count; idx++) {
        int pixel = map[idx];
        if (pixel != 0) {
            if (disjoint_sets[pixel] == pixel) {
                result[idx] = pixel;
            } else {
                while (disjoint_sets[pixel] != pixel) pixel = disjoint_sets[pixel];
                result[idx] = pixel;
            }
        }
    }

    return result;
}


std::pair<std::vector<int>, int> component_labeling_sequential(const std::vector<int>& image,
                                                               int width,
                                                               int height) {
    std::pair<std::vector<int>, std::pair<std::vector<int>, int> >
        first_pass_result = first_pass(image, width, height);
    std::vector<int> map = first_pass_result.first;
    std::vector<int> disjoint_sets = first_pass_result.second.first;
    int labels_count = first_pass_result.second.second;
    std::vector<int> result = second_pass(map, disjoint_sets, width, height);

    return std::make_pair(result, labels_count);
}


std::pair<std::vector<int>, int>
component_labeling_parallel(const std::vector<int>& image, int width, int height) {
    int proc_count, proc_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &proc_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_rank);

    if (proc_count == 1)
        return component_labeling_sequential(image, width, height);

    int size = width * height;
    const int delta = height / proc_count * width;
    const int remain = (height % proc_count) * width;
    std::vector<int> result(size);

    if (delta == 0)
        return proc_rank == 0 ? component_labeling_sequential(image, width, height)
                              : std::make_pair(result, 0);

    if (proc_rank == 0) {
        for (int proc = 1; proc < proc_count; proc++)
            MPI_Send(image.data() + remain + proc * delta, delta, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }

    std::vector<int> local_image(delta + remain);

    if (proc_rank == 0) {
        local_image = std::vector<int>(image.cbegin(), image.cbegin() + delta + remain);
    } else {
        MPI_Status status;
        MPI_Recv(local_image.data(), delta, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    }

    std::pair<std::vector<int>, std::pair<std::vector<int>, int> > first_pass_result =
        first_pass(local_image,
                   width,
                   (proc_rank == 0 ? remain + delta : delta) / width,
                   (proc_rank != 0 ? remain : 0) + delta * proc_rank);

    std::vector<int> map = first_pass_result.first;
    std::vector<int> dis_set = first_pass_result.second.first;
    int local_labels_counts = first_pass_result.second.second;

    std::vector<int> displs(proc_count);
    displs[1] = remain + delta;
    std::vector<int> recvcounts(proc_count);
    recvcounts[0] = remain + delta;
    recvcounts[1] = delta;
    for (int proc = 2; proc < proc_count; proc++) {
        displs[proc] = displs[proc - 1] + delta;
        recvcounts[proc] = delta;
    }

    std::vector<int> global_disjoint_sets(size);
    MPI_Gatherv(dis_set.data(),
                proc_rank == 0 ? remain + delta : delta,
                MPI_INT,
                global_disjoint_sets.data(),
                recvcounts.data(),
                displs.data(),
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    std::vector<int> global_map(size);
    MPI_Gatherv(map.data(),
                proc_rank == 0 ? remain + delta : delta,
                MPI_INT,
                global_map.data(),
                recvcounts.data(),
                displs.data(),
                MPI_INT,
                0,
                MPI_COMM_WORLD);

    int global_labels_count = 0;
    MPI_Reduce(&local_labels_counts, &global_labels_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (proc_rank == 0) {
        for (int x = 1; x < proc_count; x++) {
            int second_line_begin = (x != 0 ? remain : 0) + delta * x;
            int first_line_begin = second_line_begin - width;
            for (int offset = 0; offset < width; offset++) {
                // ... [A] ...
                // ... [B] ...
                int A = global_map[first_line_begin + offset];
                int B = global_map[second_line_begin + offset];
                if ((A != 0) && (B != 0)) {
                    int dis_set_A = global_disjoint_sets[A];
                    int dis_set_B = global_disjoint_sets[B];
                    if (dis_set_A != dis_set_B) {
                        int root_max_AB = std::max(A, B);
                        while (global_disjoint_sets[root_max_AB] != root_max_AB)
                            root_max_AB = global_disjoint_sets[root_max_AB];

                        int root_min_AB = std::min(A, B);
                        while (global_disjoint_sets[root_min_AB] != root_min_AB)
                            root_min_AB = global_disjoint_sets[root_min_AB];

                        if (root_max_AB != root_min_AB) {
                            global_disjoint_sets[root_max_AB] = root_min_AB;
                            global_labels_count--;
                        }
                    }
                }
            }
        }

        result = second_pass(global_map, global_disjoint_sets, width, height);
    }

    return {result, global_labels_count};
}
