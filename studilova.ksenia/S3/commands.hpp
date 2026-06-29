#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"

#include <iostream>
#include <string>
#include <functional>

namespace studilova
{
  using GraphsMap = HashTable< std::string, Graph, HMACHash, std::equal_to< std::string > >;

  using Command = void (*)(std::istream&, std::ostream&, GraphsMap&);

  using CommandsMap = HashTable< std::string, Command, HMACHash, std::equal_to< std::string > >;

  void graphs(std::istream&, std::ostream&, GraphsMap&);
  void vertexes(std::istream&, std::ostream&, GraphsMap&);
  void outbound(std::istream&, std::ostream&, GraphsMap&);
  void inbound(std::istream&, std::ostream&, GraphsMap&);
  void bind(std::istream&, std::ostream&, GraphsMap&);
  void cut(std::istream&, std::ostream&, GraphsMap&);
  void create(std::istream&, std::ostream&, GraphsMap&);
  void merge(std::istream&, std::ostream&, GraphsMap&);
  void extract(std::istream&, std::ostream&, GraphsMap&);
}

#endif
