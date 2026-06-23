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
    commands.insert("reset", madieva::cmd_reset);
    commands.insert("save", madieva::cmd_save);
    commands.insert("continue", madieva::cmd_continue);
    commands.insert("exists", madieva::cmd_exists);
    commands.insert("fill", madieva::cmd_fill);
    commands.insert("fill_row", madieva::cmd_fill_row);
    commands.insert("fill_col", madieva::cmd_fill_col);
    commands.insert("empty", madieva::cmd_empty);
    commands.insert("empty_row", madieva::cmd_empty_row);
    commands.insert("empty_col", madieva::cmd_empty_col);
    commands.insert("help", madieva::cmd_help);
    commands.insert("list_tmpl", madieva::cmd_list_tmpl);
    commands.insert("list_game", madieva::cmd_list_game);
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
