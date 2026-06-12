#include <iostream>
#include <limits>
#include <string>
#include "scheduler.hpp"
#include "commands.hpp"

int main()
{
  sedov::Scheduler scheduler;
  sedov::CommandMap commands = sedov::makeCommandMap();
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      sedov::CommandHandler handler = nullptr;
      if (commands.find(cmd, handler))
      {
        handler(std::cin, std::cout, scheduler);
      }
      else
      {
        std::cout << "[ERROR] Unknown command \"" << cmd << "\". Type 'help' for commands.\n";
        std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      }
    }
    catch (const std::exception & e)
    {
      std::cout << "[ERROR] " << e.what() << "\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (...)
    {
      std::cout << "[ERROR] Unexpected error occurred\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  std::cout << "Goodbye!\n";
  return 0;
}
