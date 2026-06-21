#include "template.hpp"
#include <fstream>
#include <sstream>
#include "solver.hpp"

madieva::Template::Template(const std::string & filename):
  rows_(0),
  cols_(0),
  fill_(0),
  rowHints_(),
  colHints_(),
  solution_(),
  isSolvable_(false)
{
  loadFromFile(filename);
}


bool madieva::Template::loadFromFile(const std::string & filename)
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
  Vector< Vector< size_t > > tempRowHints;
  Vector< Vector< size_t > > tempColHints;

  tempRowHints.reserve(rows);
  tempColHints.reserve(cols);
  
  for (size_t i = 0; i < rows_; ++i) {
    tempRowHints.pushBack(Vector< size_t >());
  }
  
  for (size_t i = 0; i < cols_; ++i) {
    tempColHints.pushBack(Vector< size_t >());
  }

  std::string line;
  for (size_t i = 0; i < rows_; ++i) {
    if (!std::getline(file, line)) {
      file.close();
      return false;
    }
    std::istringstream iss(line);
    size_t num;
    while (iss >> num) {
      tempRowHints[i].pushBack(num);
    }
    if (iss.fail() && !iss.eof()) {
      file.close();
      return false;
    }
  }

  for (size_t i = 0; i < cols_; ++i) {
    if (!std::getline(file, line)) {
      file.close();
      return false;
    }
    std::istringstream iss(line);
    size_t num;
    while (iss >> num) {
      tempColHints[i].pushBack(num);
    }
    if (iss.fail() && !iss.eof()) {
      file.close();
      return false;
    }
  }

  if (file.fail() && !file.eof()) {
    file.close();
    return false;
  }
  file.close();

  size_t totalRowHints = 0;
  for (size_t i = 0; i < rows; ++i) {
    for (size_t j = 0; j < tempRowHints[i].getSize(); ++j) {
      totalRowHints += tempRowHints[i][j];
    }
  }

  size_t totalColHints = 0;
  for (size_t i = 0; i < cols; ++i) {
    for (size_t j = 0; j < tempColHints[i].getSize(); ++j) {
      totalColHints += tempColHints[i][j];
    }
  }
  if (totalRowHints != totalColHints) {
    return false;
  }
  rows_ = rows;
  cols_ = cols;
  fill_ = 0;
  rowHints_ = std::move(tempRowHints);
  colHints_ = std::move(tempColHints);
  isSolvable_ = solve();
  return true;
}

size_t madieva::Template::getRows() const noexcept
{
  return rows_;
}

size_t madieva::Template::getCols() const noexcept
{
  return cols_;
}

size_t madieva::Template::getFill() const noexcept
{
  return fill_;
}

const madieva::Vector< madieva::Vector< size_t > > & madieva::Template::getRowHints() const
{
  return rowHints_;
}

const madieva::Vector< madieva::Vector< size_t > > & madieva::Template::getColHints() const
{
  return colHints_;
}

const madieva::Vector< madieva::Vector< int > > & madieva::Template::getSolution() const
{
  return solution_;
}

bool madieva::Template::solve()
{
  size_t sumrow = 0;
  for (size_t i = 0; i < rowHints_.getSize(); ++i) {
    for (size_t j = 0; j < rowHints_[i].getSize(); ++j) {
      sumrow += rowHints_[i][j];
    }
  }
  size_t sumcol = 0;
  for (size_t i = 0; i < colHints_.getSize(); ++i) {
    for (size_t j = 0; j < colHints_[i].getSize(); ++j) {
      sumcol += colHints_[i][j];
    }
  }
  if (sumcol != sumrow) {
    solution_ = Vector< Vector< int > >();
    isSolvable_ = false;
    return false;
  }
  Vector< Vector< int > > result = solvePuzzle(rowHints_, colHints_, rows_, cols_);
  if (result.getSize() != 0) {
    solution_ = result;
    isSolvable_ = true;
    fill_ = sumcol;
    return true;
  }

  solution_ = Vector< Vector< int > >();
  isSolvable_ = false;
  return false;
}
