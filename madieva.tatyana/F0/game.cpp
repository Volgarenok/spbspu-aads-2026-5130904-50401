#include "game.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <limits>
#include "template.hpp"
#include "solver.hpp"

madieva::Game::Game() noexcept:
  rows_(0),
  cols_(0),
  state_(),
  rowHints_(),
  colHints_(),
  solution_(),
  filledCount_(0),
  totalFilled_(0),
  cachedMaxRowHintLen_(0),
  cachedMaxColHintLen_(0),
  colsString_(),
  rowsString_()
{}

madieva::Game::Game(const Template & tmpl):
  rows_(tmpl.getRows()),
  cols_(tmpl.getCols()),
  state_(),
  rowHints_(tmpl.getRowHints()),
  colHints_(tmpl.getColHints()),
  solution_(tmpl.getSolution()),
  filledCount_(0),
  totalFilled_(tmpl.getFill()),
  cachedMaxRowHintLen_(0),
  cachedMaxColHintLen_(0),
  colsString_(),
  rowsString_()
{
  state_.reserve(rows_);
  for (size_t i = 0; i < rows_; ++i) {
    state_.pushBack(Vector< int >());
    for (size_t j = 0; j < cols_; ++j) {
      state_[i].pushBack(0);
    }
  }
  getMaxRowHintLength();
  getMaxColHintLength();
  prepareColumnStrings();
  prepareRowStrings();
}

madieva::Game::Game(const Game & game):
  rows_(game.rows_),
  cols_(game.cols_),
  state_(),
  rowHints_(game.rowHints_),
  colHints_(game.colHints_),
  solution_(game.solution_),
  filledCount_(0),
  totalFilled_(game.totalFilled_),
  cachedMaxRowHintLen_(game.cachedMaxRowHintLen_),
  cachedMaxColHintLen_(game.cachedMaxColHintLen_),
  colsString_(game.colsString_),
  rowsString_(game.rowsString_)
{
  state_.reserve(rows_);
  for (size_t i = 0; i < rows_; ++i) {
    state_.pushBack(Vector< int >());
    for (size_t j = 0; j < cols_; ++j) {
      state_[i].pushBack(0);
    }
  }
}

madieva::Game::Game(const std::string & filename):
  rows_(0),
  cols_(0),
  state_(),
  rowHints_(),
  colHints_(),
  solution_(),
  filledCount_(0),
  totalFilled_(0),
  cachedMaxRowHintLen_(0),
  cachedMaxColHintLen_(0),
  colsString_(),
  rowsString_()
{
  if (!loadFromFile(filename)) {
    throw std::runtime_error("Failed to load game from file: " + filename);
  }
  getMaxRowHintLength();
  getMaxColHintLength();
  prepareColumnStrings();
  prepareRowStrings();
}

madieva::Game & madieva::Game::operator=(const Game & other)
{
  if (this == &other) {
    return *this;
  }

  rows_ = other.rows_;
  cols_ = other.cols_;
  state_ = other.state_;
  rowHints_ = other.rowHints_;
  colHints_ = other.colHints_;
  solution_ = other.solution_;
  filledCount_ = other.filledCount_;
  totalFilled_ = other.totalFilled_;
  cachedMaxRowHintLen_ = other.cachedMaxRowHintLen_;
  cachedMaxColHintLen_ = other.cachedMaxColHintLen_;
  colsString_ = other.colsString_;
  rowsString_ = other.rowsString_;

  return *this;
}

madieva::Game & madieva::Game::operator=(Game && other) noexcept
{
  if (this == &other) {
    return *this;
  }

  rows_ = other.rows_;
  cols_ = other.cols_;
  state_ = std::move(other.state_);
  rowHints_ = std::move(other.rowHints_);
  colHints_ = std::move(other.colHints_);
  solution_ = std::move(other.solution_);
  filledCount_ = other.filledCount_;
  totalFilled_ = other.totalFilled_;
  cachedMaxRowHintLen_ = other.cachedMaxRowHintLen_;
  cachedMaxColHintLen_ = other.cachedMaxColHintLen_;
  colsString_ = std::move(other.colsString_);
  rowsString_ = std::move(other.rowsString_);

  return *this;
}

bool madieva::Game::isInBounds(size_t row, size_t col) const
{
  return row < rows_ && col < cols_;
}

bool madieva::Game::fill(size_t row, size_t col)
{
  if (!isInBounds(row, col)) {
    return false;
  }
  if (solution_[row][col] == 1 && state_[row][col] != 1) {
    state_[row][col] = 1;
    ++filledCount_;
    return true;
  }
  return false;
}

bool madieva::Game::emptyCell(size_t row, size_t col)
{
  if (!isInBounds(row, col)) {
    return false;
  }
  if (solution_[row][col] == -1 && state_[row][col] != -1) {
    state_[row][col] = -1;
    return true;
  }
  return false;
}

bool madieva::Game::win() const
{
  return filledCount_ == totalFilled_;
}

size_t madieva::Game::getRows() const noexcept
{
  return rows_;
}

size_t madieva::Game::getCols() const noexcept
{
  return cols_;
}

const madieva::Vector< madieva::Vector< size_t > > & madieva::Game::getRowHints() const
{
  return rowHints_;
}

const madieva::Vector< madieva::Vector< size_t > > & madieva::Game::getColHints() const
{
  return colHints_;
}

const madieva::Vector< madieva::Vector< int > > & madieva::Game::getSolution() const
{
  return solution_;
}

size_t madieva::Game::getFilledCount() const noexcept
{
  return filledCount_;
}

size_t madieva::Game::getTotalFilled() const noexcept
{
  return totalFilled_;
}

const madieva::Vector< madieva::Vector< int > > & madieva::Game::getState() const
{
  return state_;
}

void madieva::Game::getMaxRowHintLength()
{
  size_t maxrow = 0;
  for (size_t i = 0; i < rowHints_.getSize(); ++i) {
    size_t count = 0;
    for (size_t j = 0; j < rowHints_[i].getSize(); ++j) {
      if (rowHints_[i][j] > 9) {
        count += 2;
      } else {
        count++;
      }
    }
    count += rowHints_[i].getSize() - 1;
    if (count > maxrow) {
      maxrow = count;
    }
  }
  cachedMaxRowHintLen_ = maxrow;
}

void madieva::Game::getMaxColHintLength()
{
  size_t maxcol = 0;
  for (size_t i = 0; i < colHints_.getSize(); ++i) {
    size_t count = 0;
    for (size_t j = 0; j < colHints_[i].getSize(); ++j) {
      if (colHints_[i][j] > 9) {
        count += 2;
      } else {
        count++;
      }
    }
    count += colHints_[i].getSize() - 1;
    if (count > maxcol) {
      maxcol = count;
    }
  }
  cachedMaxColHintLen_ = maxcol;
}

bool madieva::Game::saveToFile(const std::string & filename) const
{
  std::ofstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  file << rows_ << ' ' << cols_ << '\n';
  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < rowHints_[i].getSize(); ++j) {
      file << rowHints_[i][j];
      if (j < rowHints_[i].getSize() - 1) {
        file << ' ';
      }
    }
    file << '\n';
  }

  for (size_t i = 0; i < cols_; ++i) {
    for (size_t j = 0; j < colHints_[i].getSize(); ++j) {
      file << colHints_[i][j];
      if (j < colHints_[i].getSize() - 1) {
        file << ' ';
      }
    }
    file << '\n';
  }

  for (size_t i = 0; i < rows_; ++i) {
    for (size_t j = 0; j < cols_; ++j) {
      int val = state_[i][j];
      if (val == -1) {
        file << 2;
      } else {
        file << val;
      }
      if (j < cols_ - 1) {
        file << ' ';
      }
    }
    file << '\n';
  }

  file << totalFilled_ << '\n';
  file << filledCount_ << '\n';

  bool success = file.good();

  file.close();
  return success;
}

bool madieva::Game::loadFromFile(const std::string & filename)
{
  std::fstream file;
  file.open(filename);
  if (!file.is_open()) {
    return false;
  }
  size_t rows = 0;
  size_t cols = 0;
  if (!(file >> rows >> cols)) {
    file.close();
    return false;
  }
  if (rows == 0 || cols == 0 || rows > 1000 || cols > 1000) {
    file.close();
    return false;
  }
  file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  Vector< Vector< size_t > > tempRowHints;
  Vector< Vector< size_t > > tempColHints;
  Vector< Vector< int > > tempState;
  size_t totalFilled;
  size_t filledCount;

  std::string line;

  tempRowHints.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    if (!std::getline(file, line)) {
      file.close();
      return false;
    }
    std::istringstream iss(line);
    tempRowHints.pushBack(Vector< size_t >());
    size_t num;
    while (iss >> num) {
      tempRowHints[i].pushBack(num);
    }
    if (iss.fail() && !iss.eof()) {
      file.close();
      return false;
    }
  }

  tempColHints.reserve(cols);
  for (size_t i = 0; i < cols; ++i) {
    if (!std::getline(file, line)) {
      file.close();
      return false;
    }
    std::istringstream iss(line);
    tempColHints.pushBack(Vector< size_t >());
    size_t num;
    while (iss >> num) {
      tempColHints[i].pushBack(num);
    }
    if (iss.fail() && !iss.eof()) {
      file.close();
      return false;
    }
  }

  tempState.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    if (!std::getline(file, line)) {
      file.close();
      return false;
    }
    std::istringstream iss(line);
    tempState.pushBack(Vector< int >());
    int val;
    while (iss >> val) {
      if (val == 2) {
        tempState[i].pushBack(-1);
      } else {
        tempState[i].pushBack(val);
      }
      if (iss.fail() && !iss.eof()) {
        file.close();
        return false;
      }
    }
  }

  if (file.fail() && !file.eof()) {
    file.close();
    return false;
  }
  if (tempState.getSize() != rows) {
    file.close();
    return false;
  }
  for (size_t i = 0; i < tempState.getSize(); ++i) {
    if (tempState[i].getSize() != cols) {
      file.close();
      return false;
    }
  }

  Vector< Vector< int > > result = solvePuzzle(tempRowHints, tempColHints, rows, cols);

  std::getline(file, line);
  std::istringstream totalStream(line);
  if (!(totalStream >> totalFilled)) {
      file.close();
      return false;
  }

  std::getline(file, line);
  std::istringstream filledStream(line);
  if (!(filledStream >> filledCount)) {
      file.close();
      return false;
  }

  file.close();

  rows_ = rows;
  cols_ = cols;
  state_ =  std::move(tempState);
  rowHints_ =  std::move(tempRowHints);
  colHints_ =  std::move(tempColHints);
  solution_ =  std::move(result);
  totalFilled_ = totalFilled;
  filledCount_ = filledCount;
  return true;
}

void madieva::Game::prepareColumnStrings()
{
  Vector< std::string > tempColsString;
  tempColsString.reserve(colHints_.getSize());
  for (size_t i = 0; i < colHints_.getSize(); ++i) {
    std::ostringstream oss;
    if (colHints_[i].getSize() > 0) {
      oss << colHints_[i][0];
      for (size_t j = 1; j < colHints_[i].getSize(); ++j) {
        oss << ' ' << colHints_[i][j];
      }
    }
    std::string line = oss.str();
    std::fill_n(std::back_inserter(line),
      cachedMaxColHintLen_ - line.length(), ' ');
    tempColsString.pushBack(std::move(line));
  }
  colsString_ = std::move(tempColsString);
}

void madieva::Game::prepareRowStrings()
{
  Vector< std::string > tempRowsString;
  tempRowsString.reserve(rowHints_.getSize());
  for (size_t i = 0; i < rowHints_.getSize(); ++i) {
    std::ostringstream oss;
    if (rowHints_[i].getSize() > 0) {
      oss << rowHints_[i][0];;
      for (size_t j = 1; j < rowHints_[i].getSize(); ++j) {
        oss << ' ' << rowHints_[i][j];
      }
    }
    std::string line = oss.str();
    std::fill_n(std::back_inserter(line),
      cachedMaxRowHintLen_ - line.length(), ' ');
    tempRowsString.pushBack(std::move(line));
  }
  rowsString_ = std::move(tempRowsString);
}

void madieva::Game::print(std::ostream & out) const
{
  for (size_t i = 0; i < cachedMaxColHintLen_; ++i) {
    std::fill_n(std::ostream_iterator< char >(out),
      cachedMaxRowHintLen_ + 2, ' ');
    for (size_t j = 0; j < cols_; ++j) {
      if (i < colsString_[j].length()) {
        out << colsString_[j][i];
      } else {
        out << ' ';
      }
      if (j < cols_ - 1) {
        out << ' ';
      }
    }
    out << '\n';
  }

  std::fill_n(std::ostream_iterator< char >(out),
    cachedMaxRowHintLen_ + 1, ' ');
  std::fill_n(std::ostream_iterator< char >(out), cols_ * 2, '-');

  out << '\n';

  for (size_t i = 0; i < rows_; ++i) {
    out << rowsString_[i];
    out << "| ";
    for (size_t j = 0; j < cols_; ++j) {
      if (state_[i][j] == 1) {
        out << '0';
      } else if (state_[i][j] == -1) {
        out << '-';
      } else {
        out << '.';
      }
      if (j < cols_ - 1) {
        out << ' ';
      }
    }
    out << '\n';
  }
}