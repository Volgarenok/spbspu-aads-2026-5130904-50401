#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "dataset.hpp"
#include "input.hpp"

#include <iosfwd>

namespace lukashevich
{
  using Command = void (*)(TokenReader &, DatasetTable &, std::ostream &);

  void printCommand(
      TokenReader & reader, DatasetTable & dataSets, std::ostream & output);
  void complementCommand(
      TokenReader & reader, DatasetTable & dataSets, std::ostream & output);
  void intersectCommand(
      TokenReader & reader, DatasetTable & dataSets, std::ostream & output);
  void unionCommand(
      TokenReader & reader, DatasetTable & dataSets, std::ostream & output);

  void executeCommands(
      std::istream & input, std::ostream & output, DatasetTable & dataSets);
}

#endif
