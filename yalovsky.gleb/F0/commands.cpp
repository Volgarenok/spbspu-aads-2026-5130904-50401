#include "commands.hpp"
#include <istream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include "avl-map.hpp"
#include "graph-storage.hpp"
#include "input.hpp"
#include "algorithms.hpp"
#include <fstream>
#include "file-io.hpp"

void yalovsky::runCommands(std::istream& input, std::ostream& output, GraphStorage& storage)
{
  using Command = void (*)(std::istream&, std::ostream&, GraphStorage&);
  AvlMap< std::string, Command > commands;

  commands.insert("make-graph", detail::makeGraphCommand);
  commands.insert("drop-graph", detail::dropGraphCommand);
  commands.insert("add-city", detail::addCityCommand);
  commands.insert("remove-city", detail::removeCityCommand);
  commands.insert("add-road", detail::addRoadCommand);
  commands.insert("remove-road", detail::removeRoadCommand);
  commands.insert("road-weight", detail::roadWeightCommand);
  commands.insert("show-graphs", detail::showGraphsCommand);
  commands.insert("show-cities", detail::showCitiesCommand);
  commands.insert("show-graph", detail::showGraphCommand);
  commands.insert("find-path", detail::findPathCommand);
  commands.insert("suggest-road", detail::suggestRoadCommand);
  commands.insert("save", detail::saveCommand);
  commands.insert("load", detail::loadCommand);

  std::string line;
  while (std::getline(input, line))
  {
    std::istringstream arguments(line);
    std::string command;
    if (!(arguments >> command))
    {
      continue;
    }

    try
    {
      if (command == "exit")
      {
        requireEnd(arguments);
        return;
      }
      if (command == "help")
      {
        requireEnd(arguments);
        commands.forEach([&output](const std::string& name, const Command&)
        {
          output << name << '\n';
        });
        output << "help\nexit\n";
      }
      else
      {
        commands.at(command)(arguments, output, storage);
      }
    }
    catch (const std::logic_error&)
    {
      output << "<INVALID COMMAND>\n";
    }
    catch (const std::runtime_error&)
    {
      output << "<INVALID COMMAND>\n";
    }

    if (!output)
    {
      throw std::runtime_error("Output failure");
    }
  }

  if (input.bad() || (input.fail() && !input.eof()))
  {
    throw std::runtime_error("Input failure");
  }
}

void yalovsky::detail::makeGraphCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  requireEnd(input);
  storage.makeGraph(graph);
  output << "Graph created\n";
}

void yalovsky::detail::dropGraphCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  requireEnd(input);
  storage.dropGraph(graph);
  output << "Graph removed\n";
}

void yalovsky::detail::addCityCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string city = readToken(input);
  requireEnd(input);
  storage.getGraph(graph).addCity(city);
  output << "City added\n";
}

void yalovsky::detail::removeCityCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string city = readToken(input);
  requireEnd(input);
  storage.getGraph(graph).removeCity(city);
  output << "City removed\n";
}

void yalovsky::detail::addRoadCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string from = readToken(input);
  const std::string to = readToken(input);
  const Distance distance = readUnsigned(input);
  requireEnd(input);
  storage.getGraph(graph).addRoad(from, to, distance);
  output << "Road added\n";
}

void yalovsky::detail::removeRoadCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string from = readToken(input);
  const std::string to = readToken(input);
  requireEnd(input);
  storage.getGraph(graph).removeRoad(from, to);
  output << "Road removed\n";
}

void yalovsky::detail::roadWeightCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string from = readToken(input);
  const std::string to = readToken(input);
  requireEnd(input);

  const Distance distance = storage.getGraph(graph).getRoadWeight(from, to);
  output << "Distance: " << distance << '\n';
}

void yalovsky::detail::showGraphsCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  requireEnd(input);
  storage.getGraphs().forEach([&output](const std::string& name, const RoadGraph&)
  {
    output << name << '\n';
  });
}

void yalovsky::detail::showCitiesCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string name = readToken(input);
  requireEnd(input);

  const RoadGraph& graph = storage.getGraph(name);
  graph.getCities().forEach([&output](const std::string& city, const RoadGraph::Roads&)
  {
    output << city << '\n';
  });
}

void yalovsky::detail::showGraphCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string name = readToken(input);
  requireEnd(input);

  const RoadGraph& graph = storage.getGraph(name);
  graph.getCities().forEach([&output](const std::string& from, const RoadGraph::Roads& roads)
  {
    roads.forEach([&output, &from](const std::string& to, const Distance& distance)
    {
      if (from < to)
      {
        output << from << " -> " << to << " (" << distance << ")\n";
      }
    });
  });
}

void yalovsky::detail::findPathCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const std::string from = readToken(input);
  const std::string to = readToken(input);
  requireEnd(input);

  const PathResult path = findShortestPath(storage.getGraph(graph), from, to);

  output << "Path: ";
  for (std::size_t index = 0; index < path.size; ++index)
  {
    if (index != 0)
    {
      output << " -> ";
    }
    output << path.cities[index];
  }
  output << "\nDistance: " << path.distance << '\n';
}

void yalovsky::detail::suggestRoadCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string graph = readToken(input);
  const Distance weight = readUnsigned(input);
  requireEnd(input);

  const RoadSuggestion suggestion = suggestRoad(storage.getGraph(graph), weight);
  output << "<SUGGESTED ROAD: " << suggestion.from << ' ' << suggestion.to
      << ", WEIGHT: " << suggestion.weight
      << ", IMPROVEMENT: " << suggestion.improvement << ">\n";
}

void yalovsky::detail::saveCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string filename = readToken(input);
  requireEnd(input);

  std::ofstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Cannot open output file");
  }

  writeStorage(file, storage);
  file.close();
  if (!file)
  {
    throw std::runtime_error("Cannot finish writing file");
  }

  output << "Saved\n";
}

void yalovsky::detail::loadCommand(std::istream& input, std::ostream& output,
    GraphStorage& storage)
{
  const std::string filename = readToken(input);
  requireEnd(input);

  std::ifstream file(filename);
  if (!file)
  {
    throw std::runtime_error("Cannot open input file");
  }

  readStorage(file, storage);
  output << "Loaded\n";
}
