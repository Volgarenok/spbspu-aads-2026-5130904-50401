#include <iostream>
#include <limits>
#include <string>
#include <vector.hpp>
#include "scheduler.hpp"
#include "commands.hpp"

sedov::Vector< std::string > parseCommandLine(const std::string & line)
{
  sedov::Vector< std::string > args;
  std::string current;
  bool inQuotes = false;
  for (size_t i = 0; i < line.size(); ++i)
  {
    char c = line[i];
    if (c == '"')
    {
      inQuotes = !inQuotes;
    }
    else if (c == ' ' && !inQuotes)
    {
      if (!current.empty())
      {
        args.pushBack(current);
        current.clear();
      }
    }
    else
    {
      current += c;
    }
  }
  if (!current.empty())
  {
    args.pushBack(current);
  }
  return args;
}

int main()
{
  sedov::Scheduler scheduler;
  sedov::CommandMap commands = sedov::makeCommandMap();
  std::string line;
  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }
    sedov::Vector< std::string > args = parseCommandLine(line);
    if (args.isEmpty())
    {
      continue;
    }
    const std::string & cmd = args[0];
    try
    {
      sedov::CommandHandler handler = nullptr;
      if (commands.find(cmd, handler))
      {
        sedov::Vector< std::string > cmdArgs;
        for (size_t i = 1; i < args.getSize(); ++i)
        {
          cmdArgs.pushBack(args[i]);
        }
        handler(cmdArgs, std::cout, scheduler);
      }
      else
      {
        std::cout << "[ERROR] Unknown command \"" << cmd << "\". Type 'help' for commands.\n";
      }
    }
    catch (const std::exception & e)
    {
      std::cout << "[ERROR] " << e.what() << "\n";
    }
    catch (...)
    {
      std::cout << "[ERROR] Unexpected error occurred\n";
    }
  }
  std::cout << "Goodbye!\n";
  return 0;
}
