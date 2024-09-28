#pragma once

#ifndef MATRIX_GRAPH_H
#define MATRIX_GRAPH_H

#include "graph.h"

class Matrix_graph : public Graph {
  std::vector<std::vector<int>> adj;
  void insert_edge(int first_vertex, int second_vertex, int weight) override;

  int dijkstra_to_others(int source) override;
  int dijkstra_to_chosen(int source, int destination) override;

public:
  Matrix_graph(int vertices, float density_percent);
};

#endif // !MATRIX_GRAPH_H
