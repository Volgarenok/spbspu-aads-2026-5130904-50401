#include "command.hpp"
#include <fstream>
#include <sstream>
#include "hash_functions.hpp"
#include "cuckoo_hash_table.hpp"
#include "solver.hpp"

namespace {
  bool readCell(std::istream & in, size_t & row, size_t & col)
  {
    size_t r = 0;
    size_t c = 0;
    if (!(in >> r >> c)) {
      return false;
    }
    row = r - 1;
    col = c - 1;
    return true;
  }
}

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
    out << "<TEMPLATE NOT FOUND>\n";
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
      out << "<FAILED SAVING>\n";
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

  if (games.contains(name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game game(filename);
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
    out << "<EXISTS>\n";
  } else {
    out << "<DOESN'T EXIST>\n";
  }
}

void madieva::cmd_fill(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game & game = games.get(gameName);
    if (game.getRows() <= row || game.getCols() <= col) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    bool success = game.fill(row, col);
    if (!success) {
      game.emptyCell(row, col);
      game.print(out);
      out << '\n';
      out << "<MISS>\n";
      return;
    }
    const bool isWin = game.win();
    out << '\n';
    game.print(out);
    out << '\n';
    if (isWin) {
      out << "<YOU WIN>\n";
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_fill_row(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t count = 0;
  if (!(in >> count) || count == 0) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    Game & game = games.get(gameName);
    if (game.getRows() <= row || game.getCols() < col + count) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    for (size_t i = 0; i < count; ++i) {
      size_t currentCol = col + i;
      bool success = game.fill(row, currentCol);
      if (!success) {
        game.emptyCell(row, currentCol);
        game.print(out);
        out << '\n';
        out << "<MISS>\n";
        return;
      }
    }
    const bool isWin = game.win();
    out << '\n';
    game.print(out);
    out << '\n';
    if (isWin) {
      out << "<YOU WIN>\n";
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_fill_col(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t count = 0;
  if (!(in >> count) || count == 0) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game & game = games.get(gameName);
    if (game.getRows() < row + count || game.getCols() <= col) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    for (size_t i = 0; i < count; ++i) {
      size_t currentRow = row + i;
      bool success = game.fill(currentRow, col);
      if (!success) {
        game.emptyCell(currentRow, col);
        game.print(out);
        out << '\n';
        out << "<MISS>\n";
        return;
      }
    }
    const bool isWin = game.win();
    out << '\n';
    game.print(out);
    out << '\n';
    if (isWin) {
      out << "<YOU WIN>\n";
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_empty(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game & game = games.get(gameName);
    if (game.getRows() <= row || game.getCols() <= col) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    bool success = game.emptyCell(row, col);
    if (!success) {
      game.fill(row, col);
      game.print(out);
      out << '\n';
      out << "<MISS>\n";
      return;
    }
    out << '\n';
    game.print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_empty_row(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t count = 0;
  if (!(in >> count) || count == 0) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  try {
    Game & game = games.get(gameName);
    if (game.getRows() <= row || game.getCols() < col + count) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    for (size_t i = 0; i < count; ++i) {
      size_t currentCol = col + i;
      bool success = game.emptyCell(row, currentCol);
      if (!success) {
        game.fill(row, currentCol);
        game.print(out);
        out << '\n';
        out << "<MISS>\n";
        return;
      }
    }
    out << '\n';
    game.print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_empty_col(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  size_t row = 0;
  size_t col = 0;
  if (!readCell(in, row, col)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  size_t count = 0;
  if (!(in >> count) || count == 0) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    Game & game = games.get(gameName);
    if (game.getRows() < row + count || game.getCols() <= col) {
      out << "<OUT OF RANGE>\n";
      return;
    }
    for (size_t i = 0; i < count; ++i) {
      size_t currentRow = row + i;
      bool success = game.emptyCell(currentRow, col);
      if (!success) {
        game.fill(currentRow, col);
        game.print(out);
        out << '\n';
        out << "<MISS>\n";
        return;
      }
    }
    out << '\n';
    game.print(out);
    out << '\n';
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_help(std::istream & in, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  std::string gameName;
  if (!(in >> gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!games.contains(gameName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try {
    const Game & game = games.get(gameName);
    const size_t rows = game.getRows();
    const size_t cols = game.getCols();
    const Vector< Vector< int > > & state = game.getState();
    const Vector< Vector< size_t > > & rowHints = game.getRowHints();
    const Vector< Vector< size_t > > & colHints = game.getColHints();

    Vector< std::pair< size_t, size_t > > toFill;
    Vector< std::pair< size_t, size_t > > toEmpty;

    bool find = false;
    for (size_t i = 0; i < rows && !find; ++i) {
      Vector< int > newLine;
      const bool lineChanged = analyzeLine(state[i], rowHints[i], newLine);

      if (lineChanged) {
        for (size_t j = 0; j < cols; ++j) {
          if (state[i][j] == 0 && newLine[j] == 1) {
            toFill.pushBack(std::make_pair(i + 1, j + 1));
          } else if (state[i][j] == 0 && newLine[j] == -1) {
            toEmpty.pushBack(std::make_pair(i + 1, j + 1));
          }
        }
        find = true;
      }
    }

    for (size_t j = 0; j < cols && !find; ++j) {
      Vector< int > column;
      column.reserve(rows);
      for (size_t i = 0; i < rows; ++i) {
        column.pushBack(state[i][j]);
      }

      Vector< int > newColumn;
      const bool colChanged = analyzeLine(column, colHints[j], newColumn);

      if (colChanged) {
        for (size_t i = 0; i < rows; ++i) {
          if (state[i][j] == 0 && newColumn[i] == 1) {
            toFill.pushBack(std::make_pair(i + 1, j + 1));
          } else if (state[i][j] == 0 && newColumn[i] == -1) {
            toEmpty.pushBack(std::make_pair(i + 1, j + 1));
          }
        }
        find = true;
      }
    }

    out << "fill";
    out << '\n';
    if (toFill.getSize() > 0) {
      out << "(" << toFill[0].first << ", " << toFill[0].second << ")";
    }
    for (size_t i = 1; i < toFill.getSize(); ++i) {
      out << " (" << toFill[i].first << ", " << toFill[i].second << ")";
    }
    out <<'\n';
    out << "empty";
    out << '\n';
    if (toEmpty.getSize() > 0) {
      out << "(" << toEmpty[0].first << ", " << toEmpty[0].second << ")";
    }
    for (size_t i = 0; i < toEmpty.getSize(); ++i) {
      out << " (" << toEmpty[i].first << ", " << toEmpty[i].second << ")";
    }
    out << "\n";
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_list_tmpl(std::istream &, std::ostream & out,
  TemplateTable & templates, GameTable &)
{
  try {
    Vector< std::string > templateKeys = templates.getKeys();
    if (templateKeys.getSize() == 0) {
      out << '\n';
    } else {
      for (size_t i = 0; i < templateKeys.getSize(); ++i) {
        out << templateKeys[i];
        out << "\n";
      }
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}

void madieva::cmd_list_game(std::istream &, std::ostream & out,
  TemplateTable &, GameTable & games)
{
  try {
    Vector< std::string > gameKeys = games.getKeys();
    if (gameKeys.getSize() == 0) {
      out << '\n';
    } else {
      for (size_t i = 0; i < gameKeys.getSize(); ++i) {
        out << gameKeys[i];
        out << '\n';
      }
    }
  } catch (...) {
    out << "<INVALID COMMAND>\n";
  }
}
