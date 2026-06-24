#include "commands.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>
#include "input.hpp"

namespace {
  void checkNoExtraArguments(std::istream & in)
  {
    std::string extra;

    if (in >> extra) {
      throw std::logic_error("extra command arguments");
    }
  }

  void copyVertexes(const lukashevich::Graph & source,
      lukashevich::Graph & result)
  {
    for (lukashevich::Graph::VertexTable::const_iterator it =
        source.getVertexes().cbegin();
        it != source.getVertexes().cend();
        ++it) {
      result.addVertex(it->first);
    }
  }

  void copyEdges(const lukashevich::Graph & source,
      lukashevich::Graph & result)
  {
    for (lukashevich::Graph::EdgeTable::const_iterator it =
        source.getEdges().cbegin();
        it != source.getEdges().cend();
        ++it) {
      for (std::size_t i = 0; i < it->second.getSize(); ++i) {
        result.bind(it->first.first, it->first.second, it->second[i]);
      }
    }
  }
}

void lukashevich::printGraphNames(lukashevich::Vector< std::string > & names,
  std::ostream & out)
{
  std::less< std::string > less;

  for (size_t i = 0; i < names.getSize(); ++i) {
    size_t min = i;

    for (size_t j = i + 1; j < names.getSize(); ++j) {
      if (less(names[j], names[min])) {
        min = j;
      }
    }

    if (min != i) {
      std::swap(names[i], names[min]);
    }
  }

  for (size_t i = 0; i < names.getSize(); ++i) {
     if (i != 0) {
      out << '\n';
    }
    out << names[i];
  }
}

bool lukashevich::cmdGraphs(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream & out)
{
  checkNoExtraArguments(in);

  Vector< std::string > names;

  for (GraphTable::const_iterator it = graphs.cbegin(); it != graphs.cend(); ++it) {
    names.pushBack(it->first);
  }

  printGraphNames(names, out);
  return true;
}

bool lukashevich::cmdVertexes(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream & out)
{
  std::string graphName;

  if (!(in >> graphName) || !graphs.has(graphName)) {
    throw std::logic_error("bad vertexes command");
  }

  checkNoExtraArguments(in);
  graphs.at(graphName).printVertexes(out);
  return true;
}

bool lukashevich::cmdOutbound(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream & out)
{
  std::string graphName;
  std::string vertex;

  if (!(in >> graphName >> vertex) || !graphs.has(graphName)) {
    throw std::logic_error("bad outbound command");
  }

  checkNoExtraArguments(in);
  graphs.at(graphName).printOutbound(vertex, out);
  return true;
}

bool lukashevich::cmdInbound(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream & out)
{
  std::string graphName;
  std::string vertex;

  if (!(in >> graphName >> vertex) || !graphs.has(graphName)) {
    throw std::logic_error("bad inbound command");
  }

  checkNoExtraArguments(in);
  graphs.at(graphName).printInbound(vertex, out);
  return true;
}

bool lukashevich::cmdBind(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream &)
{
  std::string graphName;
  std::string from;
  std::string to;
  Weight weight = 0;

  if (!(in >> graphName >> from >> to >> weight) || !graphs.has(graphName)) {
    throw std::logic_error("bad bind command");
  }

  checkNoExtraArguments(in);
  graphs.at(graphName).bind(from, to, weight);
  return true;
}

bool lukashevich::cmdCut(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream &)
{
  std::string graphName;
  std::string from;
  std::string to;
  Weight weight = 0;

  if (!(in >> graphName >> from >> to >> weight) || !graphs.has(graphName)) {
    throw std::logic_error("bad cut command");
  }

  checkNoExtraArguments(in);
  graphs.at(graphName).cut(from, to, weight);
  return true;
}

bool lukashevich::cmdCreate(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream &)
{
  std::string graphName;

  if (!(in >> graphName) || graphs.has(graphName)) {
    throw std::logic_error("bad create command");
  }

  Graph graph;
  size_t count = 0;

  if (!(in >> count)) {
    if (in.eof()) {
      graphs.add(graphName, graph);
      return;
    }

    throw std::logic_error("bad create command");
  }

  for (std::size_t i = 0; i < count; ++i) {
    std::string vertex;

    if (!(in >> vertex)) {
      throw std::logic_error("bad create command");
    }

    graph.addVertex(vertex);
  }

  checkNoExtraArguments(in);
  graphs.add(graphName, graph);
  return false;
}

bool lukashevich::cmdMerge(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream &)
{
  std::string newName;
  std::string firstName;
  std::string secondName;

  if (!(in >> newName >> firstName >> secondName) || graphs.has(newName)
      || !graphs.has(firstName) || !graphs.has(secondName)) {
    throw std::logic_error("bad merge command");
  }

  checkNoExtraArguments(in);

  Graph result;

  copyVertexes(graphs.at(firstName), result);
  copyVertexes(graphs.at(secondName), result);
  copyEdges(graphs.at(firstName), result);
  copyEdges(graphs.at(secondName), result);

  graphs.add(newName, result);
  return false;
}

bool lukashevich::cmdExtract(std::istream & in,
  lukashevich::GraphTable & graphs, std::ostream &)
{
  std::string newName;
  std::string sourceName;
  size_t count = 0;

  if (!(in >> newName >> sourceName >> count) || graphs.has(newName) || !graphs.has(sourceName)) {
    throw std::logic_error("bad extract command");
  }

  const Graph & source = graphs.at(sourceName);
  Graph result;

  for (size_t i = 0; i < count; ++i) {
    std::string vertex;

    if (!(in >> vertex) || !source.hasVertex(vertex)) {
      throw std::logic_error("bad extract command");
    }

    result.addVertex(vertex);
  }

  checkNoExtraArguments(in);

  for (Graph::EdgeTable::const_iterator it = source.getEdges().cbegin();
      it != source.getEdges().cend(); ++it) {
    if (result.hasVertex(it->first.first) && result.hasVertex(it->first.second)) {
      for (size_t i = 0; i < it->second.getSize(); ++i) {
        result.bind(it->first.first, it->first.second, it->second[i]);
      }
    }
  }

  graphs.add(newName, result);
  return false;
}


lukashevich::CommandTable lukashevich::createCommandTable()
{
  CommandTable commands(17, 2);

  commands.add("graphs", cmdGraphs);
  commands.add("vertexes", cmdVertexes);
  commands.add("outbound", cmdOutbound);
  commands.add("inbound", cmdInbound);
  commands.add("bind", cmdBind);
  commands.add("cut", cmdCut);
  commands.add("create", cmdCreate);
  commands.add("merge", cmdMerge);
  commands.add("extract", cmdExtract);

  return commands;
}
