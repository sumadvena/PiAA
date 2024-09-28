#pragma once

#ifndef LIST_GRAPH_H
#define LIST_GRAPH_H

#include "graph.h"

class List_graph : public Graph {
  std::list<int_pair> *adj; // vertex and weight of every edge
  void insert_edge(int first_vertex, int second_vertex, int weight) override;

  int dijkstra_to_others(int source) override;
  int dijkstra_to_chosen(int source, int destination) override;

public:
  List_graph(int vertices, float density_percent);
};

#endif // !LIST_GRAPH_H
