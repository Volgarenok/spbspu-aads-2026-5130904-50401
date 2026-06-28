#include "input.hpp"
#include <iostream>
namespace zubarev
{
  void inputGraphs(std::istream& in, std::ostream& out, GraphTable& graphs)
  {
    std::string graphName;

    while (in >> graphName) {

      size_t edges = 0;
      in >> edges;
      graphs.create(graphName, 0, {}, out);
      for (size_t i = 0; i < edges; ++i) {
        std::string from;
        std::string to;
        size_t weight = 0;

        in >> from >> to >> weight;

        const std::pair< std::string, std::string > edge(from, to);

        graphs.bind(graphName, edge, weight, out);
      }
    }
  }
}
