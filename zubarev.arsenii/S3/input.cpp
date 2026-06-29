
#include "input.hpp"
#include <iostream>

void zubarev::inputGraphs(std::istream& in, std::ostream& out, GraphTable& graphs)
{
  std::string graphName;

  while (in >> graphName) {

    size_t edges = 0;
    in >> edges;
    graphs.create(out, graphName, 0, {});
    for (size_t i = 0; i < edges; ++i) {
      std::string from;
      std::string to;
      size_t weight = 0;

      in >> from >> to >> weight;

      const std::pair< std::string, std::string > edge(from, to);

      graphs.bind(out, graphName, edge, weight);
    }
  }
}
