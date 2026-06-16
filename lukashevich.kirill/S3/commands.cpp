#include "commands.hpp"
#include <istream>
#include <ostream>
#include <stdexcept>
#include "input.hpp"

namespace {
  const char * INVALID_COMMAND = "<INVALID COMMAND>";

  void printGraphNames(lukashevich::Vector< std::string > & names, std::ostream & out)
  {
    for (size_t i = 0; i < names.getSize(); ++i) {
      size_t min = i;

      for (size_t j = i + 1; j < names.getSize(); ++j) {
        if (names[j] < names[min]) {
          min = j;
        }
      }

      if (min != i) {
        std::swap(names[i], names[min]);
      }
    }

    if (names.isEmpty()) {
    out << '\n';
    return;
    }

    for (size_t i = 0; i < names.getSize(); ++i) {
      out << names[i] << '\n';
    }
  }

  void cmdGraphs(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream & out)
  {
    if (tokens.getSize() != 1) {
      throw std::logic_error("bad graphs command");
    }

    lukashevich::Vector< std::string > names;

    for (lukashevich::GraphTable::const_iterator it = graphs.cbegin(); it != graphs.cend(); ++it) {
      names.pushBack(it->key_);
    }

    printGraphNames(names, out);
  }

  void cmdVertexes(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream & out)
  {
    if ((tokens.getSize() != 2) || !graphs.has(tokens[1])) {
      throw std::logic_error("bad vertexes command");
    }

    graphs.at(tokens[1]).printVertexes(out);
  }

  void cmdOutbound(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream & out)
  {
    if ((tokens.getSize() != 3) || !graphs.has(tokens[1])) {
      throw std::logic_error("bad outbound command");
    }

    graphs.at(tokens[1]).printOutbound(tokens[2], out);
  }

  void cmdInbound(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream & out)
  {
    if ((tokens.getSize() != 3) || !graphs.has(tokens[1])) {
      throw std::logic_error("bad inbound command");
    }

    graphs.at(tokens[1]).printInbound(tokens[2], out);
  }

  void cmdBind(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream &)
  {
    size_t weight = 0;

    if ((tokens.getSize() != 5) || !graphs.has(tokens[1])
        || !lukashevich::parseUnsigned(tokens[4], weight)) {
      throw std::logic_error("bad bind command");
    }

    graphs.at(tokens[1]).bind(tokens[2], tokens[3], weight);
  }

  void cmdCut(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream &)
  {
    size_t weight = 0;

    if ((tokens.getSize() != 5) || !graphs.has(tokens[1])
        || !lukashevich::parseUnsigned(tokens[4], weight)) {
      throw std::logic_error("bad cut command");
    }

    graphs.at(tokens[1]).cut(tokens[2], tokens[3], weight);
  }

  void cmdCreate(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream &)
  {
    if ((tokens.getSize() < 2) || graphs.has(tokens[1])) {
      throw std::logic_error("bad create command");
    }

    lukashevich::Graph graph;

    if (tokens.getSize() == 2) {
      graphs.add(tokens[1], graph);
      return;
    }

    size_t count = 0;

    if (!lukashevich::parseUnsigned(tokens[2], count) || (tokens.getSize() != count + 3)) {
      throw std::logic_error("bad create command");
    }

    for (size_t i = 0; i < count; ++i) {
      graph.addVertex(tokens[i + 3]);
    }

    graphs.add(tokens[1], graph);
  }

  void copyVertexes(const lukashevich::Graph & source, lukashevich::Graph & result)
  {
    for (lukashevich::Graph::VertexTable::const_iterator it = source.getVertexes().cbegin();
        it != source.getVertexes().cend(); ++it) {
      result.addVertex(it->key_);
    }
  }

  void copyEdges(const lukashevich::Graph & source, lukashevich::Graph & result)
  {
    for (lukashevich::Graph::EdgeTable::const_iterator it = source.getEdges().cbegin();
        it != source.getEdges().cend(); ++it) {
      for (size_t i = 0; i < it->value_.getSize(); ++i) {
        result.bind(it->key_.first, it->key_.second, it->value_[i]);
      }
    }
  }

  void cmdMerge(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream &)
  {
    if ((tokens.getSize() != 4) || graphs.has(tokens[1])
        || !graphs.has(tokens[2]) || !graphs.has(tokens[3])) {
      throw std::logic_error("bad merge command");
    }

    lukashevich::Graph result;

    copyVertexes(graphs.at(tokens[2]), result);
    copyVertexes(graphs.at(tokens[3]), result);
    copyEdges(graphs.at(tokens[2]), result);
    copyEdges(graphs.at(tokens[3]), result);

    graphs.add(tokens[1], result);
  }

  void cmdExtract(const lukashevich::Vector< std::string > & tokens,
      lukashevich::GraphTable & graphs,
      std::ostream &)
  {
    if ((tokens.getSize() < 4) || graphs.has(tokens[1]) || !graphs.has(tokens[2])) {
      throw std::logic_error("bad extract command");
    }

    size_t count = 0;

    if (!lukashevich::parseUnsigned(tokens[3], count) || (tokens.getSize() != count + 4)) {
      throw std::logic_error("bad extract command");
    }

    const lukashevich::Graph & source = graphs.at(tokens[2]);
    lukashevich::Graph result;

    for (size_t i = 0; i < count; ++i) {
      if (!source.hasVertex(tokens[i + 4])) {
        throw std::logic_error("bad extract command");
      }

      result.addVertex(tokens[i + 4]);
    }

    for (lukashevich::Graph::EdgeTable::const_iterator it = source.getEdges().cbegin();
        it != source.getEdges().cend(); ++it) {
      if (result.hasVertex(it->key_.first) && result.hasVertex(it->key_.second)) {
        for (size_t i = 0; i < it->value_.getSize(); ++i) {
          result.bind(it->key_.first, it->key_.second, it->value_[i]);
        }
      }
    }

    graphs.add(tokens[1], result);
  }
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

void lukashevich::processCommandLine(const std::string & line,
    GraphTable & graphs,
    const CommandTable & commands,
    std::ostream & out)
{
  Vector< std::string > tokens = splitLine(line);

  if (tokens.isEmpty()) {
    return;
  }

  try {
    if (!commands.has(tokens[0])) {
      throw std::logic_error("unknown command");
    }

    commands.at(tokens[0])(tokens, graphs, out);
  } catch (...) {
    out << INVALID_COMMAND << '\n';
  }
}

void lukashevich::processCommands(std::istream & in,
    std::ostream & out,
    GraphTable & graphs,
    const CommandTable & commands)
{
  std::string line;

  while (std::getline(in, line)) {
    processCommandLine(line, graphs, commands, out);
  }
}
