#include "file-io.hpp"
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>
#include "graph-storage.hpp"
#include "input.hpp"

namespace yalovsky
{
  namespace detail
  {
    void expectToken(std::istream& input, const std::string& expected);
  }
}

void yalovsky::detail::expectToken(std::istream& input, const std::string& expected)
{
  if (readToken(input) != expected)
  {
    throw std::invalid_argument("Invalid file structure");
  }
}

void yalovsky::writeStorage(std::ostream& output, const GraphStorage& storage)
{
  output << "NAVIGATOR 1\n";
  output << storage.getGraphs().size() << '\n';

  storage.getGraphs().forEach([&output](const std::string& name, const RoadGraph& graph)
  {
    output << "GRAPH " << name << ' ' << graph.getCities().size()
        << ' ' << graph.getRoadCount() << '\n';

    graph.getCities().forEach([&output](const std::string& city, const RoadGraph::Roads&)
    {
      output << "CITY " << city << '\n';
    });

    graph.getCities().forEach([&output](const std::string& from, const RoadGraph::Roads& roads)
    {
      roads.forEach([&output, &from](const std::string& to, const Distance& distance)
      {
        if (from < to)
        {
          output << "ROAD " << from << ' ' << to << ' ' << distance << '\n';
        }
      });
    });

    output << "ENDGRAPH\n";
  });

  output << "END\n";
  if (!output)
  {
    throw std::runtime_error("Cannot write graphs");
  }
}

void yalovsky::readStorage(std::istream& input, GraphStorage& storage)
{
  detail::expectToken(input, "NAVIGATOR");
  constexpr unsigned long long supportedVersion = 1;

  if (readUnsigned(input) != supportedVersion)
  {
    throw std::invalid_argument("Unsupported file version");
  }

  GraphStorage loaded;
  const unsigned long long graphCount = readUnsigned(input);

  for (unsigned long long graphIndex = 0; graphIndex < graphCount; ++graphIndex)
  {
    detail::expectToken(input, "GRAPH");
    const std::string name = readToken(input);
    const unsigned long long cityCount = readUnsigned(input);
    const unsigned long long roadCount = readUnsigned(input);

    loaded.makeGraph(name);
    RoadGraph& graph = loaded.getGraph(name);

    for (unsigned long long cityIndex = 0; cityIndex < cityCount; ++cityIndex)
    {
      detail::expectToken(input, "CITY");
      graph.addCity(readToken(input));
    }

    for (unsigned long long roadIndex = 0; roadIndex < roadCount; ++roadIndex)
    {
      detail::expectToken(input, "ROAD");
      const std::string from = readToken(input);
      const std::string to = readToken(input);
      const Distance distance = readUnsigned(input);
      graph.addRoad(from, to, distance);
    }

    detail::expectToken(input, "ENDGRAPH");
  }

  detail::expectToken(input, "END");
  requireEnd(input);
  storage.swap(loaded);
}
