#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iosfwd>
#include <string>
#include "../common/vector.hpp"
#include "graph.hpp"
#include "hash-table.hpp"
#include "hasher.hpp"

namespace lukashevich {
  using CommandHandler = void (*)(const Vector< std::string > &, GraphTable &, std::ostream &);

  using CommandTable = HashTable<
      std::string,
      CommandHandler,
      Blake2StringHash,
      StringEqual >;

  CommandTable createCommandTable();

  void processCommandLine(const std::string & line,
      GraphTable & graphs,
      const CommandTable & commands,
      std::ostream & out);

  void processCommands(std::istream & in,
      std::ostream & out,
      GraphTable & graphs,
      const CommandTable & commands);
}

#endif
