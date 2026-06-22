#include "command.hpp"
#include "hash_functions.hpp"
#include "cuckoo_hash_table.hpp"
#include <fstream>
#include <sstream>

void madieva::cmd_load(std::istream & in, std::ostream & out, TemplateTable & templates, GameTable &) {
  std::string name;
  std::string filename;
  if (!(in >> name >> filename)) {
    out << "INVALID COMMAND\n";
    return;
  }

  if (templates.contains(name)) {
    out << "TEMPLATE IN USE\n";
    return;
  }

  try {
    Template tmpl;
    if (!tmpl.loadFromFile(filename)) {
        out << "FAILED READING\n";
        return;
    }
    templates.insert(name, tmpl);
  } catch (...) {
    out << "FAILED READING\n";
  }
}

void madieva::cmd_start(std::istream & in, std::ostream & out,
  TemplateTable & templates, GameTable & games)
{
  std::string templateName;
  std::string gameName;
  if (!(in >> templateName >> gameName)) {
    out << "INVALID COMMAND\n";
    return;
  }
  if (!templates.contains(templateName)) {
    out << "TEMPLATE NOT FOUND: " << templateName << "\n";
    return;
  }

  if (games.contains(gameName)) {
    out << "GAME NAME ALREADY IN USE: " << gameName << "\n";
    return;
  }

    if (!templates.get(templateName).solvable()) {
    out << "TEMPLATE NOT SOLVE\n";
    return;
  }

  try {
    Template tmpl = templates.get(templateName);
    Game game(tmpl);
    games.insert(std::move(gameName), std::move(game));
  } catch (...) {
      out << "INVALID COMMAND\n";
  }
}
