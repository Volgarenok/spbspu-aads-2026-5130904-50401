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
  try {
    commands.insert("load", madieva::cmd_load);
    commands.insert("start", madieva::cmd_start);
  } catch (...) {
    std::cerr << "<INTERNAL ERROR>\n";
    return 2;
  }
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
  return 0;
}
