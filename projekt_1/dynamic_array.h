#pragma once

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#include "video.h"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>

class Dynamic_array {
  Video *_array = NULL;
  int _capacity = 1;
  int _size = 0;

public:
  Dynamic_array();
  Dynamic_array(int capacity, Dynamic_array *arr);
  ~Dynamic_array();

private:
  // utilites
  void grow_array();
  void prepare_data();
  bool right_sorted(Video *arr);
  double calc_median(Video *arr);
  double calc_mean(Video *arr);
  void time_measure();

  // sorting algorithms
  void quick_sort(Video *arr, int const start, int const end);
  void merge(Video *arr, int const start, int const mid, int const end);
  void merge_sort(Video *arr, int const start, int const end);
  void heapify(Video *arr, int const n, int const i);
  void heap_sort(Video *arr, int const start, int const end);
  void insertion_sort(Video *arr, int const start, int const end);
  void intro_sort(Video *arr, int const start, int const end,
                  int const max_depth);
};

#endif // !DYNAMIC_ARRAY_H
