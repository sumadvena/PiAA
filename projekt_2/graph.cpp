#include "graph.h"
#include <algorithm>
#include <chrono>
#include <iostream>
#include <queue>
#include <utility>

typedef std::pair<int, int> int_pair;
using namespace std::chrono;

/*

 GRAPH UTILITIES

*/
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

/*

   adjacency list implementation
 GRAPH UTILITIES

*/

List_graph::List_graph(int vertices, float density_percent)
    : Graph(vertices, density_percent) {

  int edge_number = calculate_edges();
  for (int test = 0; test < number_of_tests; test++) {
    adj = new std::list<int_pair>[V];
    // graph initialization
    // std::cout << "\nStarted test no. " << test + 1 << std::endl;

    for (int i = 0; i < edge_number; i++) {
      int first_vertex = value_gen('v', V); // choosing random vertex
      int weight = value_gen('w');          // and weight
      int second_vertex = value_gen('v', V);
      while (first_vertex == second_vertex) {
        second_vertex = value_gen('v', V);
      }

      // for avoiding double edges to same vertices
      bool doubled_edge = false;
      for (auto elem : adj[first_vertex]) {
        if (elem.first == second_vertex) {
          doubled_edge = true;
          break;
        }
      }
      if (doubled_edge) {
        i--;
        continue;
      }
      insert_edge(first_vertex, second_vertex, weight);
    }

    // measurements
    time_for_all += dijkstra_to_others(value_gen('v', V));
    double t_t_t = dijkstra_to_chosen(value_gen('v', V), value_gen('v', V));
    while (t_t_t == -1) {
      // std::cout << t_t_t << " ";
      t_t_t = dijkstra_to_chosen(value_gen('v', V), value_gen('v', V));
    }
    time_for_two += t_t_t;

    delete[] adj;
  }

  std::cout << "\n\tList graph\t|V| = " << V << "\tD = " << density_percent
            << "\n";
  print_measures_mean();
}

void List_graph::insert_edge(int first_vertex, int second_vertex, int weight) {
  adj[first_vertex].emplace_back(second_vertex, weight);
  adj[second_vertex].emplace_back(first_vertex, weight);
}

int List_graph::dijkstra_to_others(int source) {
  // std::cout << "  hello from dto " << V << " " << density_percent << "\n";
  steady_clock::time_point begin = steady_clock::now();
  std::priority_queue<int_pair, std::vector<int_pair>, std::greater<int_pair>>
      pq;
  std::vector<int> distances(V, 999);
  std::vector<int> parents(
      V, -1); // To store the parent of each vertex in the shortest path tree
  pq.emplace(0, source);
  distances[source] = 0;

  while (!pq.empty()) {
    int min_distance_vertex = pq.top().second;
    pq.pop();
    for (auto &elem : adj[min_distance_vertex]) {
      int vertex = elem.first;
      int weight = elem.second;

      int next_check = distances[min_distance_vertex] + weight;
      if (distances[vertex] > next_check) {
        distances[vertex] = next_check;
        parents[vertex] =
            min_distance_vertex; // Update the parent of the vertex
        pq.emplace(distances[vertex], vertex);
      }
    }
  }

  if (V <= 10 && number_of_tests == 1) {
    printf(
        "\nPaths and distances from source vertex %d to all other vertices:\n",
        source);
    for (int i = 0; i < V; ++i) {
      if (i != source) {
        int distance = distances[i];
        std::vector<int> path;
        int curr = i;
        while (curr != -1) {
          path.push_back(curr);
          curr = parents[curr];
        }
        std::reverse(path.begin(), path.end());
        printf("Vertex %d: Distance = %d, Path: ", i, distance);
        for (int vertex : path) {
          printf("%d -> ", vertex);
        }
        printf("and back\n");
      }
    }
  }

  steady_clock::time_point end = steady_clock::now();
  return duration_cast<microseconds>(end - begin).count();
}

int List_graph::dijkstra_to_chosen(int source, int destination) {
  // std::cout << "  hello from dtc " << V << " " << density_percent << "\n";

  // ensuring that different vertices were chosen
  while (destination == source) {
    source = value_gen('v', V);
  }

  steady_clock::time_point begin = steady_clock::now();
  std::priority_queue<int_pair, std::vector<int_pair>, std::greater<int_pair>>
      pq;
  std::vector<int> distances(V, 999);
  std::vector<int> parents(
      V, -1); // To store the parent of each vertex in the shortest path tree
  pq.emplace(0, source);
  distances[source] = 0;

  while (!pq.empty()) {
    int min_distance_vertex = pq.top().second;
    pq.pop();

    if (min_distance_vertex == destination) {
      break;
    }

    for (auto &elem : adj[min_distance_vertex]) {
      int vertex = elem.first;
      int weight = elem.second;

      int next_check = distances[min_distance_vertex] + weight;
      if (distances[vertex] > next_check) {
        distances[vertex] = next_check;
        parents[vertex] =
            min_distance_vertex; // Update the parent of the vertex
        pq.emplace(distances[vertex], vertex);
      }
    }
  }

  // Check if a path from source to destination exists
  if (distances[destination] == 999) {
    // printf("\nNo path exists from source vertex %d to destination vertex
    // %d\n",
    //        source, destination);
    return -1;
  }

  int distance = distances[destination];
  std::vector<int> path;
  int curr = destination;
  while (curr != -1) {
    path.push_back(curr);
    curr = parents[curr];
  }
  if (V <= 10 && number_of_tests == 1) {
    std::reverse(path.begin(), path.end());

    printf(
        "\nPath and distance from source vertex %d to destination vertex %d:\n",
        source, destination);
    printf("Distance = %d, Path: ", distance);
    for (int vertex : path) {
      printf("%d -> ", vertex);
    }
    printf("and back\n");
  }

  steady_clock::time_point end = steady_clock::now();
  return duration_cast<microseconds>(end - begin).count();
}

/*

   adjacency matrix implementation
 GRAPH UTILITIES

*/
Matrix_graph::Matrix_graph(int vertices, float density_percent)
    : Graph(vertices, density_percent) {
  int edge_number = calculate_edges();
  adj.resize(V, std::vector<int>(V, 0));

  for (int test = 0; test < number_of_tests; test++) {
    // graph initialization
    // std::cout << "\nStarted test no. " << test + 1 << std::endl;

    for (int i = 0; i < edge_number; i++) {
      int first_vertex = value_gen('v', V); // choosing random vertex
      int weight = value_gen('w');          // and weight
      int second_vertex = value_gen('v', V);
      while (first_vertex == second_vertex) {
        second_vertex = value_gen('v', V);
      }

      // for avoiding double edges to same vertices
      if (adj[first_vertex][second_vertex] != 0) {
        i--;
        continue;
      }
      insert_edge(first_vertex, second_vertex, weight);
    }

    // measurements
    time_for_all += dijkstra_to_others(value_gen('v', V));
    double t_t_t = dijkstra_to_chosen(value_gen('v', V), value_gen('v', V));
    while (t_t_t == -1) {
      t_t_t = dijkstra_to_chosen(value_gen('v', V), value_gen('v', V));
    }
    time_for_two += t_t_t;

    for (auto &row : adj) {
      std::fill(row.begin(), row.end(), 0);
    }
  }

  std::cout << "\n\tMatrix graph\t|V| = " << V << "\tD = " << density_percent
            << "\n";
  print_measures_mean();
}

void Matrix_graph::insert_edge(int first_vertex, int second_vertex,
                               int weight) {
  adj[first_vertex][second_vertex] = weight;
  adj[second_vertex][first_vertex] = weight;
}

int Matrix_graph::dijkstra_to_others(int source) {
  // std::cout << "  hello from dto " << V << " " << density_percent << "\n";
  steady_clock::time_point begin = steady_clock::now();
  std::priority_queue<int_pair, std::vector<int_pair>, std::greater<int_pair>>
      pq;
  std::vector<int> distances(V, 999);
  std::vector<int> parents(
      V, -1); // To store the parent of each vertex in the shortest path tree

  pq.emplace(0, source);
  distances[source] = 0;

  while (!pq.empty()) {
    int min_distance_vertex = pq.top().second;
    pq.pop();

    // Access the adjacency matrix to get neighboring vertices and weights
    for (int vertex = 0; vertex < V; vertex++) {
      int weight = adj[min_distance_vertex][vertex];

      // Skip if there is no edge or the vertex has already been processed
      if (weight == 0 || distances[vertex] != 999) {
        continue;
      }

      int next_check = distances[min_distance_vertex] + weight;
      if (distances[vertex] > next_check) {
        distances[vertex] = next_check;
        parents[vertex] =
            min_distance_vertex; // Update the parent of the vertex
        pq.emplace(distances[vertex], vertex);
      }
    }
  }

  if (V <= 10 && number_of_tests == 1) {
    printf(
        "\nPaths and distances from source vertex %d to all other vertices:\n",
        source);
    for (int i = 0; i < V; ++i) {
      if (i != source) {
        int distance = distances[i];
        std::vector<int> path;
        int curr = i;
        while (curr != -1) {
          path.push_back(curr);
          curr = parents[curr];
        }
        std::reverse(path.begin(), path.end());
        printf("Vertex %d: Distance = %d, Path: ", i, distance);
        for (int vertex : path) {
          printf("%d -> ", vertex);
        }
        printf("and back\n");
      }
    }
  }

  steady_clock::time_point end = steady_clock::now();
  return duration_cast<microseconds>(end - begin).count();
}

int Matrix_graph::dijkstra_to_chosen(int source, int destination) {
  // std::cout << "  hello from dtc " << V << " " << density_percent << "\n";
  // ensuring that different vertices were chosen
  while (destination == source) {
    source = value_gen('v', V);
  }

  steady_clock::time_point begin = steady_clock::now();
  std::priority_queue<int_pair, std::vector<int_pair>, std::greater<int_pair>>
      pq;
  std::vector<int> distances(V, 999);
  std::vector<int> parents(
      V, -1); // To store the parent of each vertex in the shortest path tree

  pq.emplace(0, source);
  distances[source] = 0;

  while (!pq.empty()) {
    int min_distance_vertex = pq.top().second;
    pq.pop();

    // Stop the algorithm if the destination vertex is found
    if (min_distance_vertex == destination) {
      break;
    }

    // Access the adjacency matrix to get neighboring vertices and weights
    for (int vertex = 0; vertex < V; vertex++) {
      int weight = adj[min_distance_vertex][vertex];

      // Skip if there is no edge or the vertex has already been processed
      if (weight == 0 || distances[vertex] != 999) {
        continue;
      }

      int next_check = distances[min_distance_vertex] + weight;
      if (distances[vertex] > next_check) {
        distances[vertex] = next_check;
        parents[vertex] =
            min_distance_vertex; // Update the parent of the vertex
        pq.emplace(distances[vertex], vertex);
      }
    }
  }

  // Check if a path from source to destination exists
  if (distances[destination] == 999) {
    // printf("\nNo path exists from source vertex %d to destination vertex
    // %d\n",
    //        source, destination);
    return -1;
  }

  int distance = distances[destination];
  std::vector<int> path;
  int curr = destination;
  while (curr != -1) {
    path.push_back(curr);
    curr = parents[curr];
  }
  if (V <= 10 && number_of_tests == 1) {
    std::reverse(path.begin(), path.end());

    printf(
        "\nPath and distance from source vertex %d to destination vertex %d:\n",
        source, destination);
    printf("Distance = %d, Path: ", distance);
    for (int vertex : path) {
      printf("%d -> ", vertex);
    }
    printf("and back\n");
  }

  steady_clock::time_point end = steady_clock::now();
  return duration_cast<microseconds>(end - begin).count();
}
