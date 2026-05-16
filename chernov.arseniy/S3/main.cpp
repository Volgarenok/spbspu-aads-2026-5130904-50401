#include <fstream>
#include <iostream>
#include <string>

#include "graph.hpp"
#include <hashtable.hpp>
#include <vector.hpp>

int main(int args, char ** argv)
{
  using namespace chernov;

  if (args != 2) {
    std::cerr << "filename is required, count of args must be 1\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "could not open file\n";
    return 1;
  }

  std::istream & input = std::cin;
  std::ostream & output = std::cout;

  Graphs graphs;

  std::string graph_name;
  while (file >> graph_name) {
    size_t edges_count;
    file >> edges_count;
    for (size_t i = 0; i < edges_count; ++i) {
      std::string start, end;
      size_t weight;
      file >> start >> end >> weight;
      graphs.addEdge(graph_name, start, end, weight);
    }
  }
}
