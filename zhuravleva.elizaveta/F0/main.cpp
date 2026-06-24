#include <iostream>
#include <string>
#include "taskManager.hpp"
#include "commands.hpp"

int main()
{
  zhuravleva::TaskManager manager;
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      zhuravleva::executeCommand(command, std::cin, std::cout, manager);
    }
    catch (const std::exception& error)
    {
      std::cout << error.what() << "\n";
    }
  }
}
