#include <list>
#include <vector>

typedef std::pair<int, int> int_pair;

class Graph {
protected:
  int V; // no. of vertices
  float density_percent;

  int calculate_edges() { return (density_percent * V * (V - 1)) / 2; }
  int value_gen(char type, int vertices_number = 0);

  Graph(int vertices, float density_percent)
      : V(vertices), density_percent(density_percent){};
  virtual void add_edge(int first_vertex, int second_vertex, int weight) = 0;
  virtual void print_graph() = 0;

  virtual void dijkstra_to_others(int source) = 0;
  virtual void dijkstra_to_chosen(int source, int destination) = 0;
};

class List_graph : public Graph {
  std::list<int_pair> *adj; // vertex and weight of every edge
  void add_edge(int first_vertex, int second_vertex, int weight) override;

public:
  List_graph(int vertices, float density_percent);
  // ~List_graph() { delete adj; }

  void print_graph() override;

  void dijkstra_to_others(int source) override;
  void dijkstra_to_chosen(int source, int destination) override;
};

class Matrix_graph : public Graph {
  std::vector<std::vector<int>> adj;
  void add_edge(int first_vertex, int second_vertex, int weight) override;

public:
  Matrix_graph(int vertices, float density_percent);

  void print_graph() override;

  void dijkstra_to_others(int source) override;
  void dijkstra_to_chosen(int source, int destination) override;
};
