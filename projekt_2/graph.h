#include <list>
#include <vector>

typedef std::pair<int, int> int_pair;

class Graph {
protected:
  int V; // no. of vertices
  int number_of_tests = 1;
  float density_percent;
  double time_for_all = 0, time_for_two = 0;

  int calculate_edges() { return (density_percent * V * (V - 1)) / 2; }
  int value_gen(char type, int vertices_number = 0);
  double arithmetic_mean(double value) { return value / number_of_tests; }
  void print_measures_mean();

  Graph(int vertices, float density_percent)
      : V(vertices), density_percent(density_percent){};
  virtual void insert_edge(int first_vertex, int second_vertex, int weight) = 0;
  virtual void print_graph() = 0;

  virtual int dijkstra_to_others(int source) = 0;
  virtual int dijkstra_to_chosen(int source, int destination) = 0;
};

class List_graph : public Graph {
  std::list<int_pair> *adj; // vertex and weight of every edge
  void insert_edge(int first_vertex, int second_vertex, int weight) override;

public:
  List_graph(int vertices, float density_percent);
  // ~List_graph() { delete adj; }

  void print_graph() override;

  int dijkstra_to_others(int source) override;
  int dijkstra_to_chosen(int source, int destination) override;
};

class Matrix_graph : public Graph {
  std::vector<std::vector<int>> adj;
  void insert_edge(int first_vertex, int second_vertex, int weight) override;

public:
  Matrix_graph(int vertices, float density_percent);

  void print_graph() override;

  int dijkstra_to_others(int source) override;
  int dijkstra_to_chosen(int source, int destination) override;
};
