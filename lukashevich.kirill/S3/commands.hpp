#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <functional>
#include <iosfwd>
#include <string>

#include "vector/vector.hpp"
#include "graph.hpp"
#include "hash-table.hpp"
#include "hasher.hpp"

namespace lukashevich {
  using CommandHandler = bool (*)(std::istream &, GraphTable &, std::ostream &);
  using CommandTable = HashTable< std::string, CommandHandler, Blake2StringHash, std::equal_to< std::string > >;

  CommandTable createCommandTable();

  void printGraphNames(Vector< std::string > & names, std::ostream & out);

  bool cmdGraphs(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdVertexes(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdOutbound(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdInbound(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdBind(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdCut(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdCreate(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdMerge(std::istream & in, GraphTable & graphs, std::ostream & out);
  bool cmdExtract(std::istream & in, GraphTable & graphs, std::ostream & out);
}

#endif
