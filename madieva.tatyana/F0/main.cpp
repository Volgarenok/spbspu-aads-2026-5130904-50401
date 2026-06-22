#include "command.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

int main()
{
  madieva::TemplateTable templates(16, 100);
  madieva::GameTable games(16, 100);
  madieva::CommandTable commands(16, 100);
  commands.insert("load", madieva::cmd_load);
  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string comm;
    if (!(iss >> comm)) {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    if (commands.contains(comm)) {
      commands.get(comm)(iss, std::cout, templates, games);
    } else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
}
