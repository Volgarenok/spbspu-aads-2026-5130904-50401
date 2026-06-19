#include "input.hpp"
#include <limits>
#include <stdexcept>

bool lukashevich::parseUnsigned(const std::string & text, size_t & value)
{
  if (text.empty()) {
    return false;
  }

  size_t result = 0;

  for (size_t i = 0; i < text.size(); ++i) {
    const char ch = text[i];

    if ((ch < '0') || (ch > '9')) {
      return false;
    }

    const size_t digit = static_cast< size_t >(ch - '0');

    if (result > (std::numeric_limits< size_t >::max() - digit) / 10) {
      return false;
    }

    result = result * 10 + digit;
  }

  value = result;
  return true;
}

lukashevich::Vector< std::string > lukashevich::splitLine(const std::string & line)
{
  Vector< std::string > tokens;
  size_t pos = 0;

  while (pos < line.size()) {
    while ((pos < line.size()) && ((line[pos] == ' ') || (line[pos] == '\t'))) {
      ++pos;
    }

    std::string token;

    while ((pos < line.size()) && (line[pos] != ' ') && (line[pos] != '\t')) {
      token.push_back(line[pos]);
      ++pos;
    }

    if (!token.empty()) {
      tokens.pushBack(token);
    }
  }

  return tokens;
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
