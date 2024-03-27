#include "dynarrandutils.h"

//--------------------------------------- Constructors 

Dynamic_array::Dynamic_array() {
    _array = new Video[_capacity];
    prepare_data();
    std::cout << "Done importing\n";
  };

Dynamic_array::Dynamic_array(int capacity, Dynamic_array *arr) : _capacity(capacity) {
    // ensuring that array won't be bigger than filtered dataset
    if (_capacity > arr->_size) {
      _capacity = arr->_size;
    }

    _array = new Video[_capacity];
    for (int i = 0; i < _capacity; i++) {
      if (_size == _capacity) {
        grow_array();
      }
      this->_array[i] = arr->_array[i];
      _size++;
    }

    time_measure();
  };

Dynamic_array::~Dynamic_array() { delete[] _array; };

// -------------------------------------- Utilities
void Dynamic_array::grow_array() {
  _capacity = _capacity * 2;
  Video *temporary_array = new Video[_capacity];

  for (int i = 0; i < _size; i++) {
    temporary_array[i] = _array[i];
  }

  delete[] _array;
  _array = temporary_array;
}

void Dynamic_array::prepare_data() {
  std::ifstream database;
  database.open("projekt1_dane.csv");
  if (database.is_open()) {
    std::cout << "File opened\n";
  } else {
    std::cout << "Could not find the file\n";
    return;
  }

  std::string line;
  bool first_row = true;
  while (std::getline(database, line)) {
    // skiping the first line without data
    if (first_row) {
      first_row = false;
      continue;
    }
    std::istringstream iss(line);
    std::string token;
    int j = 0; // ensuring end parsing
    Video temporary;
    bool has_comma = false;
    while (std::getline(iss, token, ',')) {
      switch (j) {
      case 0:
        temporary.number = std::stoi(token);
        j++;
        break;
      case 1:
        if (token[0] == '"') {
          temporary.title = token;
          has_comma = true;
          break;
        }
        if (has_comma) {
          temporary.title += token;
          if (token[token.size() - 1] == '"') {
            j++;
          }
          break;
        } else {
          temporary.title = token;
          j++;
          break;
        }
      case 2:
        temporary.rating = std::stof(token);
        j++;
        break;
      default:
        break;
      }
    }
    // filtering the _array
    if (!(temporary.rating == -1 || temporary.number == -1 ||
          temporary.title == "wrong")) {
      if (_size == _capacity) {
        grow_array();
      }
      _array[_size] = temporary;
      _size++;
    }
  }
  database.close();
}

bool Dynamic_array::right_sorted(Video *arr) {
  for (int i = _size - 1; i > 0; i--) {
    if (!(arr[i].rating >= arr[i - 1].rating)) {
      return false;
    }
  }
  return true;
}

double Dynamic_array::calc_median(Video *arr) {
  int const center = _size / 2;
  if (_size % 2 == 0) {
    return (arr[center].rating + arr[center + 1].rating) / 2;
  } else {
    return arr[center + 1].rating;
  }
}

double Dynamic_array::calc_mean(Video *arr) {
  double sum = 0;
  for (int i = 0; i < _size; i++) {
    sum += arr[i].rating;
  }
  return sum / _size;
}

void Dynamic_array::time_measure() {
  using namespace std::chrono;

  for (int i = 0; i < 3; i++) {
    Video *copy_for_sort = new Video[_size];

    for (int k = 0; k < _size; k++) {
      copy_for_sort[k] = _array[k];
    }

    switch (i) {
    case 0: {
      steady_clock::time_point begin = steady_clock::now();
      quick_sort(copy_for_sort, 0, _size - 1);
      steady_clock::time_point end = steady_clock::now();
      auto duration = duration_cast<milliseconds>(end - begin).count();
      std::cout << "Finished sorting array of size " << _size
                << " in: " << duration << " ms\n";
      if (right_sorted(copy_for_sort)) {
        std::cout << "The array was sorted correctly\n";
      }
      break;
    }
    case 1: {
      steady_clock::time_point begin = steady_clock::now();
      merge_sort(copy_for_sort, 0, _size - 1);
      steady_clock::time_point end = steady_clock::now();
      auto duration = duration_cast<milliseconds>(end - begin).count();
      std::cout << "Finished sorting array of size " << _size
                << " in: " << duration << " ms\n";
      if (right_sorted(copy_for_sort)) {
        std::cout << "The array was sorted correctly\n";
      }
      break;
    }
    case 2: {
      int max_depth = 2 * std::log2(_size);
      steady_clock::time_point begin = steady_clock::now();
      intro_sort(copy_for_sort, 0, _size - 1, max_depth);
      steady_clock::time_point end = steady_clock::now();
      auto duration = duration_cast<milliseconds>(end - begin).count();
      std::cout << "Finished sorting array of size " << _size
                << " in: " << duration << " ms\n";
      if (right_sorted(copy_for_sort)) {
        std::cout << "The array was sorted correctly\n";
      }
      break;
    }
    }
    
    if (i == 0) {
        std::cout << "The median of this dataset is " << calc_median(copy_for_sort)
            << std::endl;
        std::cout << "The arithmetic mean of this dataset is "
            << calc_mean(copy_for_sort) << std::endl;
    }
    delete[] copy_for_sort;
  }
}

//--------------------------------------------------Sorting

void Dynamic_array::quick_sort(Video *arr, int const start, int const end) {
  // base case
  if (start >= end)
    return;

  int const p = arr[(start + end) / 2].rating;
  int i = start;
  int j = end;

  while (i <= j) {
    while (arr[i].rating < p) {
      i++;
    }

    while (arr[j].rating > p) {
      j--;
    }

    if (i <= j) {
      std::swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }

  // _sorting the start part
  quick_sort(arr, start, j);
  // _sorting the end part
  quick_sort(arr, i, end);
}

void Dynamic_array::merge(Video *arr, int const start, int const mid,
                         int const end) {
  int const sub_array_first = mid - start + 1;
  int const sub_array_second = end - mid;

  // Create temp arrays
  Video *start_array = new Video[sub_array_first],
        *end_array = new Video[sub_array_second];

  // Copy data to temp arrays start_array[]
  // and end_array[]
  for (int i = 0; i < sub_array_first; i++) {
    start_array[i] = arr[start + i];
  }
  for (int i = 0; i < sub_array_second; i++) {
    end_array[i] = arr[mid + 1 + i];
  }

  // Initial index of first sub-array
  // Initial index of second sub-array
  int index_sub_array_first = 0, index_sub_array_second = 0;

  // Initial index of merged array
  int index_merged_array = start;

  // Merge the temp arrays back into
  // array[start..end]
  while (index_sub_array_first < sub_array_first && index_sub_array_second < sub_array_second) {
    if (start_array[index_sub_array_first].rating <=
        end_array[index_sub_array_second].rating) {
      arr[index_merged_array] = start_array[index_sub_array_first];
      index_sub_array_first++;
    } else {
      arr[index_merged_array] = end_array[index_sub_array_second];
      index_sub_array_second++;
    }
    index_merged_array++;
  }

  // Copy the remaining elements of
  // start array, if there are any
  while (index_sub_array_first < sub_array_first) {
    arr[index_merged_array] = start_array[index_sub_array_first];
    index_sub_array_first++;
    index_merged_array++;
  }

  // the same for the end arra
  while (index_sub_array_second < sub_array_second) {
    arr[index_merged_array] = end_array[index_sub_array_second];
    index_sub_array_second++;
    index_merged_array++;
  }
}

void Dynamic_array::merge_sort(Video *arr, int const start, int const end) {
  // Returns recursively
  if (start >= end)
    return;

  int const mid = start + (end - start) / 2;
  merge_sort(arr, start, mid);
  merge_sort(arr, mid + 1, end);
  merge(arr, start, mid, end);
}

void Dynamic_array::heapify(Video *arr, int const n, int const i) {
  int largest = i;
  int start = 2 * i + 1;
  int end = 2 * i + 2;

  if (start < n && arr[start].rating > arr[largest].rating)
    largest = start;

  if (end < n && arr[end].rating > arr[largest].rating)
    largest = end;

  if (largest != i) {
    std::swap(arr[i], arr[largest]);
    heapify(arr, n, largest);
  }
}

void Dynamic_array::heap_sort(Video *arr, int const start, int const end) {
  int const n = end - start + 1;
  for (int i = n / 2 - 1; i >= 0; i--) {
    heapify(arr, n, i + start);
  }

  for (int i = n - 1; i > 0; i--) {
    std::swap(arr[start], arr[start + i]);
    heapify(arr, i, start);
  }
}

void Dynamic_array::insertion_sort(Video *arr, int const start, int const end) {
  for (int i = start + 1; i <= end; ++i) {
    Video key = arr[i];
    int j = i - 1;

    while (j >= start && arr[j].rating > key.rating) {
      arr[j + 1] = arr[j];
      j--;
    }
    arr[j + 1] = key;
  }
}

void Dynamic_array::intro_sort(Video *arr, int const start, int const end,
                              int const max_depth) {

  int const current_size = end - start;
  if (current_size < 16) {
    insertion_sort(arr, start, end);
    return;
  }
  if (max_depth == 0) {
    heap_sort(arr, start, end);
    return;
  }

  int const p = arr[(start + end) / 2].rating;
  int i = start;
  int j = end;

  while (i <= j) {
    while (arr[i].rating < p) {
      i++;
    }

    while (arr[j].rating > p) {
      j--;
    }

    if (i <= j) {
      std::swap(arr[i], arr[j]);
      i++;
      j--;
    }
  }

  // _sorting the start part
  intro_sort(arr, start, j, max_depth - 1);
  // _sorting the end part
  intro_sort(arr, i, end, max_depth - 1);
  return;
}

