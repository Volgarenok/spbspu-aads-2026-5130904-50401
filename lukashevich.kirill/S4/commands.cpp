#include "commands.hpp"

#include <functional>
#include <istream>
#include <ostream>
#include <stdexcept>
#include <string>

#include "compare.hpp"
#include "tree.hpp"

namespace lukashevich
{
  namespace detail
  {
    void requireEnd(const TokenReader & reader);
    std::string readArgument(TokenReader & reader);
  }
}

void lukashevich::detail::requireEnd(const TokenReader & reader)
{
  if (!reader.empty())
  {
    throw std::invalid_argument("unexpected command");
  }
}

std::string lukashevich::detail::readArgument(TokenReader & reader)
{
  std::string argument;
  if (!reader.read(argument))
  {
    throw std::invalid_argument("miss argument");
  }
  return argument;
}

void lukashevich::printCommand(
    TokenReader & reader, DatasetTable & dataSets, std::ostream & output)
{
  const std::string name = detail::readArgument(reader);
  detail::requireEnd(reader);
  dataSets.print(name, output);
}

void lukashevich::complementCommand(
    TokenReader & reader, DatasetTable & dataSets, std::ostream &)
{
  const std::string newName = detail::readArgument(reader);
  const std::string lhsName = detail::readArgument(reader);
  const std::string rhsName = detail::readArgument(reader);
  detail::requireEnd(reader);
  dataSets.complement(newName, lhsName, rhsName);
}

void lukashevich::intersectCommand(
    TokenReader & reader, DatasetTable & dataSets, std::ostream &)
{
  const std::string newName = detail::readArgument(reader);
  const std::string lhsName = detail::readArgument(reader);
  const std::string rhsName = detail::readArgument(reader);
  detail::requireEnd(reader);
  dataSets.intersect(newName, lhsName, rhsName);
}

void lukashevich::unionCommand(
    TokenReader & reader, DatasetTable & dataSets, std::ostream &)
{
  const std::string newName = detail::readArgument(reader);
  const std::string lhsName = detail::readArgument(reader);
  const std::string rhsName = detail::readArgument(reader);
  detail::requireEnd(reader);
  dataSets.unite(newName, lhsName, rhsName);
}

void lukashevich::executeCommands(
    std::istream & input, std::ostream & output, DatasetTable & dataSets)
{
  using CommandTable = BSTree< std::string, Command, Comparator< std::string > >;

  CommandTable commands;
  commands.push("print", printCommand);
  commands.push("complement", complementCommand);
  commands.push("intersect", intersectCommand);
  commands.push("union", unionCommand);

  std::string line;
  while (std::getline(input, line))
  {
    TokenReader reader(line);
    std::string commandName;
    if (!reader.read(commandName))
    {
      continue;
    }

    const CommandTable::const_iterator command = commands.find(commandName);
    if (command == commands.end())
    {
      output << "<INVALID COMMAND>\n";
      continue;
    }

    try
    {
      command->second(reader, dataSets, output);
    }
    catch (const std::invalid_argument &)
    {
      output << "<INVALID COMMAND>\n";
    }
    catch (const std::out_of_range &)
    {
      output << "<INVALID COMMAND>\n";
    }
  }

  if (!input.eof() && input.fail())
  {
    throw std::runtime_error("failed  read");
  }
}
