#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "graph.hpp"
#include <string>
#include <iostream>

namespace vasyakin
{
  using GraphsMap = HashTable< std::string, Graph, vasyakin::HMACHash, std::equal_to< std::string > >;

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
