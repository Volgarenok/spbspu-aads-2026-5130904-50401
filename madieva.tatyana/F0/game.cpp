#include "game.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "template.hpp"
#include "solver.hpp"

madieva::Game::Game(const Template & tmpl):
  rows_(tmpl.getRows()),
  cols_(tmpl.getCols()),
  state_(),
  rowHints_(tmpl.getRowHints()),
  colHints_(tmpl.getColHints()),
  solution_(tmpl.getSolution()),
  filledCount_(0),
  totalFilled_(tmpl.getFill())
{
  state_.reserve(rows_);
  for (size_t i = 0; i < rows_; ++i) {
    state_.pushBack(Vector< int >());
    for (size_t j = 0; j < cols_; ++j) {
      state_[i].pushBack(0);
    }
  }
}

madieva::Game::Game(const Game & game):
  rows_(game.rows_),
  cols_(game.cols_),
  state_(),
  rowHints_(game.rowHints_),
  colHints_(game.colHints_),
  solution_(game.solution_),
  filledCount_(0),
  totalFilled_(game.totalFilled_)
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
  totalFilled_(0)
{
  loadFromFile(filename);
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
  if (solution_[row][col] == 1) {
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
  if (solution_[row][col] == -1) {
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

  file.close();
  return true;
}

bool madieva::Game::loadFromFile(const std::string & filename)
{
  std::fstream file;
  file.open(filename);
  if (!file.is_open()) {
    return false;
  }
  size_t rows, cols;
  Vector< Vector< size_t > > tempRowHints;
  Vector< Vector< size_t > > tempColHints;
  Vector< Vector< int > > tempState;
  size_t totalFilled;
  size_t filledCount;

  std::string line;

  std::getline(file, line);
  std::istringstream sizeStream(line);
  sizeStream >> rows >> cols;

  tempRowHints.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    tempRowHints.pushBack(Vector< size_t >());
    size_t num;
    while (iss >> num) {
      tempRowHints[i].pushBack(num);
    }
  }

  tempColHints.reserve(cols);
  for (size_t i = 0; i < cols; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    tempColHints.pushBack(Vector< size_t >());
    size_t num;
    while (iss >> num) {
      tempColHints[i].pushBack(num);
    }
  }

  tempState.reserve(rows);
  for (size_t i = 0; i < rows; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    tempState.pushBack(Vector< int >());
    int val;
    while (iss >> val) {
      if (val == 2) {
        tempState[i].pushBack(-1);
      } else {
        tempState[i].pushBack(val);
      }
    }
  }

  Vector< Vector< int > > result = solvePuzzle(tempRowHints, tempColHints, rows, cols);

  std::getline(file, line);
  std::istringstream totalStream(line);
  totalStream >> totalFilled;

  std::getline(file, line);
  std::istringstream filledStream(line);
  filledStream >> filledCount;

  file.close();

  rows_ = rows;
  cols_ = cols;
  state_ = tempState;
  rowHints_ = tempRowHints;
  colHints_ = tempColHints;
  solution_ = result;
  totalFilled_ = totalFilled;
  filledCount_ = filledCount;
  return true;
}
