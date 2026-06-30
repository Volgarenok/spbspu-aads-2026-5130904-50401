#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <istream>
#include <ostream>
#include <string>
#include "graph.hpp"
#include "hash_table.hpp"
#include "sip_hash.hpp"

namespace hvostov {
  using GraphTable = HashTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  using cmd_t = void (*)(std::istream&, std::ostream&, GraphTable&);
  using const_cmd_t = void (*)(std::istream&, std::ostream&, const GraphTable&);

  void graphs(std::istream& in, std::ostream& out, const GraphTable& graphs);
  void vertexes(std::istream& in, std::ostream& out, const GraphTable& graphs);
  void outbound(std::istream& in, std::ostream& out, const GraphTable& graphs);
  void inbound(std::istream& in, std::ostream& out, const GraphTable& graphs);
  void bind(std::istream& in, std::ostream& out, GraphTable& graphs);
  void cut(std::istream& in, std::ostream& out, GraphTable& graphs);
  void create(std::istream& in, std::ostream& out, GraphTable& graphs);
  void merge(std::istream& in, std::ostream& out, GraphTable& graphs);
  void extract(std::istream& in, std::ostream& out, GraphTable& graphs);
}

#endif
