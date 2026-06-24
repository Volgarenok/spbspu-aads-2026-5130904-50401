#include "commands.hpp"

#include <iomanip>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <string>

#include "vector/vector.hpp"

namespace {
  bool isSpace(char symbol) noexcept
  {
    return (symbol == ' ') || (symbol == '\t') || (symbol == '\r');
  }

  lukashevich::Vector< std::string > splitLine(const std::string & line)
  {
    lukashevich::Vector< std::string > tokens;
    size_t begin = 0;

    while (begin < line.size()) {
      while ((begin < line.size()) && isSpace(line[begin])) {
        ++begin;
      }

      size_t end = begin;
      while ((end < line.size()) && !isSpace(line[end])) {
        ++end;
      }

      if (begin < end) {
        tokens.pushBack(line.substr(begin, end - begin));
      }

      begin = end;
    }

    return tokens;
  }

  bool hasSign(const std::string & value)
  {
    return !value.empty() && ((value[0] == '-') || (value[0] == '+'));
  }

  size_t parseSize(const std::string & value)
  {
    if (value.empty() || hasSign(value)) {
      throw std::invalid_argument("bad size");
    }

    size_t position = 0;
    unsigned long long result = std::stoull(value, &position);
    if (position != value.size()) {
      throw std::invalid_argument("bad size");
    }
    if (result > std::numeric_limits< size_t >::max()) {
      throw std::invalid_argument("bad size");
    }

    return static_cast< size_t >(result);
  }

  size_t parsePositiveSize(const std::string & value)
  {
    const size_t result = parseSize(value);
    if (result == 0) {
      throw std::invalid_argument("bad positive size");
    }

    return result;
  }

  double parseDouble(const std::string & value)
  {
    if (value.empty()) {
      throw std::invalid_argument("bad double");
    }

    size_t position = 0;
    const double result = std::stod(value, &position);
    if (position != value.size()) {
      throw std::invalid_argument("bad double");
    }

    return result;
  }

  void checkProduct(size_t first, size_t second)
  {
    if ((second != 0) && (first > std::numeric_limits< size_t >::max() / second)) {
      throw std::invalid_argument("size overflow");
    }
  }

  void executeList(const lukashevich::Vector< std::string > & tokens,
      const lukashevich::MatrixTable & table,
      std::ostream & out)
  {
    if (tokens.getSize() != 1) {
      throw std::invalid_argument("bad command");
    }

    table.printList(out);
  }

  void executeCreate(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 4) {
      throw std::invalid_argument("bad command");
    }

    const std::string & name = tokens[1];
    const size_t rows = parsePositiveSize(tokens[2]);
    const size_t cols = parsePositiveSize(tokens[3]);
    checkProduct(rows, cols);

    const size_t expected = rows * cols;
    const size_t actual = tokens.getSize() - 4;
    if (actual > expected) {
      throw std::invalid_argument("bad values count");
    }

    lukashevich::Vector< double > values;
    values.reserve(expected);
    for (size_t i = 4; i < tokens.getSize(); ++i) {
      values.pushBack(parseDouble(tokens[i]));
    }
    for (size_t i = actual; i < expected; ++i) {
      values.pushBack(0.0);
    }

    table.add(name, lukashevich::Matrix(rows, cols, values));
  }

  void executeShow(const lukashevich::Vector< std::string > & tokens,
      const lukashevich::MatrixTable & table,
      std::ostream & out)
  {
    if (tokens.getSize() != 2) {
      throw std::invalid_argument("bad command");
    }

    table.printOne(out, tokens[1]);
  }

  void executeDelete(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 2) {
      throw std::invalid_argument("bad command");
    }

    table.remove(tokens[1]);
  }

  void executeGet(const lukashevich::Vector< std::string > & tokens,
      const lukashevich::MatrixTable & table,
      std::ostream & out)
  {
    if (tokens.getSize() != 4) {
      throw std::invalid_argument("bad command");
    }

    const std::string & name = tokens[1];
    const size_t row = parseSize(tokens[2]);
    const size_t col = parseSize(tokens[3]);
    const lukashevich::Matrix & matrix = table.get(name);

    const double value = matrix(row, col);

    out << name << '[' << row << "][" << col << "] = ";
    lukashevich::printDouble(out, value);
    out << '\n';
  }

  void executeSet(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 5) {
      throw std::invalid_argument("bad command");
    }

    const size_t row = parseSize(tokens[2]);
    const size_t col = parseSize(tokens[3]);
    const double value = parseDouble(tokens[4]);
    table.get(tokens[1])(row, col) = value;
  }

  void executeDet(const lukashevich::Vector< std::string > & tokens,
      const lukashevich::MatrixTable & table,
      std::ostream & out)
  {
    if (tokens.getSize() != 2) {
      throw std::invalid_argument("bad command");
    }

    const double value = table.get(tokens[1]).getDeterminant();

    out << "Determinant of " << tokens[1] << ": ";
    lukashevich::printDouble(out, value);
    out << '\n';
  }

  void executeRank(const lukashevich::Vector< std::string > & tokens,
      const lukashevich::MatrixTable & table,
      std::ostream & out)
  {
    if (tokens.getSize() != 2) {
      throw std::invalid_argument("bad command");
    }

    out << "Rank of " << tokens[1] << ": " << table.get(tokens[1]).getRank() << '\n';
  }

  void executeScalarMultiply(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 4) {
      throw std::invalid_argument("bad command");
    }

    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const double value = parseDouble(tokens[3]);
    table.add(tokens[1], table.get(tokens[2]) * value);
  }

  void executeTranspose(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 3) {
      throw std::invalid_argument("bad command");
    }

    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    table.add(tokens[1], table.get(tokens[2]).getTransposed());
  }

  void executeInverse(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 3) {
      throw std::invalid_argument("bad command");
    }

    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    table.add(tokens[1], table.get(tokens[2]).getInversed());
  }

  void fillMatrixNames(const lukashevich::Vector< std::string > & tokens,
      size_t count,
      lukashevich::Vector< std::string > & names)
  {
    if (tokens.getSize() != count + 3) {
      throw std::invalid_argument("bad command");
    }

    for (size_t i = 0; i < count; ++i) {
      names.pushBack(tokens[i + 3]);
    }
  }

  void executeAdd(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t count = parsePositiveSize(tokens[2]);
    if (count < 2) {
      throw std::invalid_argument("bad count");
    }

    lukashevich::Vector< std::string > names;
    fillMatrixNames(tokens, count, names);

    lukashevich::Matrix result = table.get(names[0]);
    for (size_t i = 1; i < names.getSize(); ++i) {
      result = result + table.get(names[i]);
    }

    table.add(tokens[1], result);
  }

  void executeSub(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t count = parsePositiveSize(tokens[2]);
    if (count < 2) {
      throw std::invalid_argument("bad count");
    }

    lukashevich::Vector< std::string > names;
    fillMatrixNames(tokens, count, names);

    lukashevich::Matrix result = table.get(names[0]);
    for (size_t i = 1; i < names.getSize(); ++i) {
      result = result - table.get(names[i]);
    }

    table.add(tokens[1], result);
  }

  void executeMul(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t count = parsePositiveSize(tokens[2]);
    if (count < 2) {
      throw std::invalid_argument("bad count");
    }

    lukashevich::Vector< std::string > names;
    fillMatrixNames(tokens, count, names);

    lukashevich::Matrix result = table.get(names[0]);
    for (size_t i = 1; i < names.getSize(); ++i) {
      result = result * table.get(names[i]);
    }

    table.add(tokens[1], result);
  }

  void executeMergeVertical(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const double fillValue = parseDouble(tokens[4]);
    table.add(tokens[1], mergeVertical(table.get(tokens[2]), table.get(tokens[3]), fillValue));
  }

  void executeMergeHorizontal(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const double fillValue = parseDouble(tokens[4]);
    table.add(tokens[1], mergeHorizontal(table.get(tokens[2]), table.get(tokens[3]), fillValue));
  }

  void executeAddRows(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 3) {
      throw std::invalid_argument("bad command");
    }

    lukashevich::Matrix result = table.get(tokens[1]);
    for (size_t i = 2; i < tokens.getSize(); ++i) {
      result.appendRows(table.get(tokens[i]));
    }

    table.get(tokens[1]) = result;
  }

  void executeAddCols(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() < 3) {
      throw std::invalid_argument("bad command");
    }

    lukashevich::Matrix result = table.get(tokens[1]);
    for (size_t i = 2; i < tokens.getSize(); ++i) {
      result.appendCols(table.get(tokens[i]));
    }

    table.get(tokens[1]) = result;
  }

  void executeTileHorizontal(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 4) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t count = parsePositiveSize(tokens[3]);
    table.add(tokens[1], tileHorizontal(table.get(tokens[2]), count));
  }

  void executeTileVertical(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 4) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t count = parsePositiveSize(tokens[3]);
    table.add(tokens[1], tileVertical(table.get(tokens[2]), count));
  }

  void executeFill(const lukashevich::Vector< std::string > & tokens,
      lukashevich::MatrixTable & table)
  {
    if (tokens.getSize() != 5) {
      throw std::invalid_argument("bad command");
    }
    if (table.contains(tokens[1])) {
      throw std::invalid_argument("matrix exists");
    }

    const size_t rows = parsePositiveSize(tokens[2]);
    const size_t cols = parsePositiveSize(tokens[3]);
    checkProduct(rows, cols);

    table.add(tokens[1], lukashevich::makeFilledMatrix(rows, cols, parseDouble(tokens[4])));
  }
}

void lukashevich::executeCommand(const std::string & line,
    MatrixTable & table,
    std::ostream & out)
{
  const Vector< std::string > tokens = splitLine(line);
  if (tokens.isEmpty()) {
    return;
  }

  try {
    const std::string & command = tokens[0];
    if (command == "list") {
      executeList(tokens, table, out);
    } else if (command == "create") {
      executeCreate(tokens, table);
    } else if (command == "show") {
      executeShow(tokens, table, out);
    } else if (command == "delete") {
      executeDelete(tokens, table);
    } else if (command == "get") {
      executeGet(tokens, table, out);
    } else if (command == "set") {
      executeSet(tokens, table);
    } else if (command == "det") {
      executeDet(tokens, table, out);
    } else if (command == "rank") {
      executeRank(tokens, table, out);
    } else if (command == "smul") {
      executeScalarMultiply(tokens, table);
    } else if (command == "transpose") {
      executeTranspose(tokens, table);
    } else if (command == "inverse") {
      executeInverse(tokens, table);
    } else if (command == "add") {
      executeAdd(tokens, table);
    } else if (command == "sub") {
      executeSub(tokens, table);
    } else if (command == "mul") {
      executeMul(tokens, table);
    } else if (command == "merge-v") {
      executeMergeVertical(tokens, table);
    } else if (command == "merge-h") {
      executeMergeHorizontal(tokens, table);
    } else if (command == "add-rows") {
      executeAddRows(tokens, table);
    } else if (command == "add-cols") {
      executeAddCols(tokens, table);
    } else if (command == "tile-h") {
      executeTileHorizontal(tokens, table);
    } else if (command == "tile-v") {
      executeTileVertical(tokens, table);
    } else if (command == "fill") {
      executeFill(tokens, table);
    } else {
      throw std::invalid_argument("unknown command");
    }
  } catch (const std::exception &) {
    out << "<INVALID COMMAND>\n";
  }
}
