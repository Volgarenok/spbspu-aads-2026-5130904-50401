#include "input.hpp"

#include <istream>
#include <limits>
#include <stdexcept>

bool lukashevich::hasExtraData(std::istream & in)
{
  int symbol = in.peek();

  while ((symbol == ' ') || (symbol == '\t')) {
    in.get();
    symbol = in.peek();
  }

  return (symbol != std::char_traits< char >::eof());
}

void lukashevich::readGraphs(std::istream & in, GraphTable & graphs)
{
  std::string graphName;
  size_t edgeCount = 0;

  while (in >> graphName >> edgeCount) {
    if (graphs.has(graphName)) {
      throw std::logic_error("duplicate graph name");
    }

    Graph graph;

    for (size_t i = 0; i < edgeCount; ++i) {
      std::string from;
      std::string to;
      Weight weight = 0;

      if (!(in >> from >> to >> weight)) {
        throw std::logic_error("bad graph input");
      }

      graph.bind(from, to, weight);
    }

    graphs.add(graphName, graph);
  }

  if (!in.eof()) {
    throw std::logic_error("bad graph input");
  }
}
