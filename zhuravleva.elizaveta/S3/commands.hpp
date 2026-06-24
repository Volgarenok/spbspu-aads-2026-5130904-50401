#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <functional>
#include <iosfwd>
#include <string>
#include "graph.hpp"
#include "myHash.hpp"
#include "hasher.hpp"

namespace zhuravleva
{
  using GraphTable = HashTable< std::string, Graph,
      Blake2Hasher< std::string >, std::equal_to< std::string > >;
  void graphs(std::ostream& out, std::istream& in, const GraphTable& graphs);
  void vertexes(std::ostream& out, std::istream& in, const GraphTable& graphs);
  void outbound(std::ostream& out, std::istream& in, const GraphTable& graphs);
  void inbound(std::ostream& out, std::istream& in, const GraphTable& graphs);

  void bind(std::ostream& out, std::istream& in, GraphTable& graphs);
  void cut(std::ostream& out, std::istream& in, GraphTable& graphs);
  void create(std::ostream& out, std::istream& in, GraphTable& graphs);
  void merge(std::ostream& out, std::istream& in, GraphTable& graphs);
  void extract(std::ostream& out, std::istream& in, GraphTable& graphs);
}

#endif
