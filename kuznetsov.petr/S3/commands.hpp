#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <string>

#include "graph.hpp"

namespace kuznetsov {
  using table = HashTable< std::string, Graph, SipHasher< std::string >, KeyComparator >;

  void graphs(std::ostream&, std::istream&, const table&);
  void vertexes(std::ostream&, std::istream&, const table&);
  void outbound(std::ostream&, std::istream&, const table&);
  void inbound(std::ostream&, std::istream&, const table&);

  void bind(std::ostream&, std::istream&, table&);
  void cut(std::ostream&, std::istream&, table&);

  void create(std::ostream&, std::istream&, table&);
  void merge(std::ostream&, std::istream&, table&);
  void extract(std::ostream&, std::istream&, table&);

}

#endif
