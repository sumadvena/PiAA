#include "graph.h"
#include <iostream>
#include <random>

std::random_device dev;
std::mt19937 rng(dev());

int Graph::value_gen(char type, int vertices_number) {
  int value = -1;
  switch (type) {
  case 'v': {
    std::uniform_int_distribution<> vertex_distribution(0, vertices_number - 1);
    value = vertex_distribution(rng);
    break;
  }
  case 'w': {
    std::uniform_int_distribution<> weight_distribution(1, 20);
    value = weight_distribution(rng);
    break;
  }
  default:
    break;
  }
  return value;
}

void Graph::print_measures_mean() {
  std::cout << "Calculating the shortest path from source to all of the "
               "vertices took: "
            << arithmetic_mean(time_for_all) << " us\n";
  std::cout << "Calculating the shortest path from source to one of the "
               "vertices took: "
            << arithmetic_mean(time_for_two) << " us\n";
}
