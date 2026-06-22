#include "command.hpp"
#include "hash_functions.hpp"
#include "cuckoo_hash_table.hpp"
#include <fstream>
#include <sstream>

void madieva::cmd_load(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable &) {
  std::string name;
  std::string filename;
  if (!(in >> name >> filename)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (templates.contains(name)) {
    out << "<TEMPLATE IN USE>\n";
    return;
  }

  try {
    Template tmpl;
    if (!tmpl.loadFromFile(filename)) {
      out << "<FAILED READING>\n";
      return;
    }
    templates.insert(name, tmpl);
  } catch (...) {
    out << "<FAILED READING>\n";
  }
}

void madieva::cmd_start(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games)
{
  std::string templateName;
  std::string gameName;
  if (!(in >> gameName >> templateName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!templates.contains(templateName)) {
    out << "TEMPLATE NOT FOUND\n";
    return;
  }

  if (games.contains(gameName)) {
    out << "<GAME NAME ALREADY IN USE>\n";
    return;
  }

  if (!templates.get(templateName).solvable()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Template & tmpl = templates.get(templateName);
    Game game(tmpl);
    games.insert(gameName, std::move(game));
    games.get(gameName).print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_reset(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  std::string gameNameNew;
  if (!(in >> gameNameNew >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (games.contains(gameNameNew) || !games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game game(games.get(gameName));
    games.insert(gameNameNew, std::move(game));
    games.get(gameNameNew).print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_save(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string name;
  std::string filename;
  if (!(in >> name >> filename)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!games.contains(name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    if (!games.get(name).saveToFile(filename)) {
      out << "FAILED SAVING\n";
      return;
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_continue(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string name;
  std::string filename;
  if (!(in >> name >> filename)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!games.contains(name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game game;
    if (!game.loadFromFile(filename)) {
      out << "<FAILED READING>\n";
      return;
    }
    games.insert(name, std::move(game));
    games.get(name).print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_exists(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable &)
{
  std::string name;
  if (!(in >> name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!templates.contains(name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (templates.get(name).solvable()) {
    out << "exists\n";
  } else {
    out << "doesn't exist\n";
  }
}

