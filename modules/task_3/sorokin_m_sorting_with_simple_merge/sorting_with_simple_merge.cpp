// Copyright 2020 Sorokin Maxim

#include <mpi.h>
#include <string>
#include <vector>
#include "../../../modules/task_3/sorokin_m_sorting_with_simple_merge/sorting_with_simple_merge.h"

int D_heap_cntr(int root, int size) {
  if (root >= size) return 0;
  int base = 1;
  if (root * 2 + 1 < size) base += D_heap_cntr(root * 2 + 1, size);
  if (root * 2 + 2 < size) base += D_heap_cntr(root * 2 + 2, size);
  return base;
}
