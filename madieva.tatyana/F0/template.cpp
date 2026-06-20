#include "template.hpp"
#include <fstream>
#include <sstream>
#include "solver.hpp"

madieva::Template::Template(size_t rows, size_t cols):
  rows_(rows),
  cols_(cols),
  rowHints_(),
  colHints_(),
  solution_(),
  isSolvable_(false)
{
  rowHints_.reserve(rows_);
  colHints_.reserve(cols_);
  
  for (size_t i = 0; i < rows_; ++i) {
    rowHints_.pushBack(Vector< size_t >());
  }
  
  for (size_t i = 0; i < cols_; ++i) {
    colHints_.pushBack(Vector< size_t >());
  }
}

bool madieva::Template::loadFromFile(const std::string & filename)
{
  std::fstream file;
  file.open(filename);
  if (!file.is_open()) {
    return false;
  }
  size_t rows, cols;
  file >> rows >> cols;
  rows_ = rows;
  cols_ = cols;
  rowHints_ = Vector< Vector< size_t > >();
  colHints_ = Vector< Vector< size_t > >();
  rowHints_.reserve(rows_);
  colHints_.reserve(cols_);
  
  for (size_t i = 0; i < rows_; ++i) {
    rowHints_.pushBack(Vector< size_t >());
  }
  
  for (size_t i = 0; i < cols_; ++i) {
    colHints_.pushBack(Vector< size_t >());
  }

  std::string line;
  std::getline(file, line);
  for (size_t i = 0; i < rows_; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    size_t num;
    while (iss >> num) {
      rowHints_[i].pushBack(num);
    }
  }

  for (size_t i = 0; i < cols_; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    size_t num;
    while (iss >> num) {
      colHints_[i].pushBack(num);
    }
  }

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
      sumrow++;
    }
  }
  size_t sumcol = 0;
  for (size_t i = 0; i < colHints_.getSize(); ++i) {
    for (size_t j = 0; j < colHints_[i].getSize(); ++j) {
      sumcol++;
    }
  }
  if (sumcol != sumrow){
    solution_ = Vector< Vector< int > >();
  }
  fill_ = sumcol;
  Vector< Vector< int > > result = solvePuzzle(rowHints_, colHints_, rows_, cols_);
  if (result.getSize() != 0) {
    solution_ = result;
    isSolvable_ = true;
    isSolvable_ = false;
    return true;
  }

  solution_ = Vector< Vector< int > >();
  isSolvable_ = false;
  return false;
}
