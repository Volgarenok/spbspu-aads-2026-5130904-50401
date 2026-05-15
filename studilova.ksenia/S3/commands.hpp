#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"

#include <iostream>
#include <string>
#include <functional>
#include <utility>

namespace studilova
{
  using GraphsMap = HashTable<
    std::string,
    Graph,
    HMACHash,
    std::equal_to< std::string >
  >;

  using Command = void (*)(std::istream&, std::ostream&, GraphsMap&);

  using CommandsMap = HashTable<
    std::string,
    Command,
    HMACHash,
    std::equal_to< std::string >
  >;

  template< class T, class Cmp >
  void sortVector(Vector< T >& values, Cmp cmp)
  {
    for (size_t i = 0; i < values.getSize(); ++i)
    {
      for (size_t j = i + 1; j < values.getSize(); ++j)
      {
        if (cmp(values[j], values[i]))
        {
          std::swap(values[i], values[j]);
        }
      }
    }
  }

  bool compareStrings(const std::string& lhs, const std::string& rhs)
  {
    return lhs < rhs;
  }

  bool compareWeights(size_t lhs, size_t rhs)
  {
    return lhs < rhs;
  }

  bool compareConnections(
    const studilova::Graph::Connection& lhs,
    const studilova::Graph::Connection& rhs
  )
  {
    return lhs.first < rhs.first;
  }

  void initCommands(CommandsMap& commands);

  void graphs(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void vertexes(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void outbound(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void inbound(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void bind(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void cut(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void create(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void merge(std::istream& in, std::ostream& out, GraphsMap& graphs);
  void extract(std::istream& in, std::ostream& out, GraphsMap& graphs);
}

#endif
