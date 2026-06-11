#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_map>

namespace
{
  using command_t = void (*)(std::istream&, std::ostream&);

  void invalid_command(std::istream& in, std::ostream& out)
  {
    out << "<INVALID COMMAND>\n";

    const std::streamsize max_size = std::numeric_limits< std::streamsize >::max();
    in.ignore(max_size, '\n');
  }
}

int main()
{
  using commands_t = std::unordered_map< std::string, command_t >;

  commands_t commands;

  std::string command;
  while (std::cin >> command)
  {
    try
    {
      commands.at(command)(std::cin, std::cout);
    }
    catch (const std::out_of_range&)
    {
      invalid_command(std::cin, std::cout);
    }
    catch (const std::logic_error&)
    {
      invalid_command(std::cin, std::cout);
    }
  }

  if (!std::cin.eof())
  {
    std::cerr << "bad input\n";
    return 1;
  }

  return 0;
}
