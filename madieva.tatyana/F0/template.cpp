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
  int rows, cols;
  file >> rows >> cols;
  if (rows <= 0 || cols <= 0) {
    return false;
  }
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
      if (num > 0) {
        rowHints_[i].pushBack(num);
      }
    }
  }

  for (size_t i = 0; i < cols_; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    size_t num;
    while (iss >> num) {
      if (num > 0) {
        colHints_[i].pushBack(num);
      }
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
  Vector< Vector< int > > picture;
  picture.reserve(getRows());
  for (size_t i = 0; i < getRows(); ++i) {
    picture.pushBack(Vector< int >());
    for (size_t j = 0; j < getCols(); ++j) {
      picture[i].pushBack(0);
    }
  }
  bool changed = true;
  size_t iterations = 0;
  const size_t MAX_ITERATIONS = 1000;

  while (changed && iterations < MAX_ITERATIONS) {
    changed = false;
    ++iterations;
    for (size_t i = 0; i < rows_; ++i) {
      Vector< int > newLine;
      bool lineChanged = analyzeLine(picture[i], rowHints_[i], newLine);
      
      if (lineChanged) {
        picture[i] = newLine;
        changed = true;
      }
    }
    for (size_t j = 0; j < cols_; ++j) {
      Vector< int > column;
      column.reserve(rows_);
      for (size_t i = 0; i < rows_; ++i) {
        column.pushBack(picture[i][j]);
      }

      Vector< int > newColumn;
      bool colChanged = analyzeLine(column, colHints_[j], newColumn);

      if (colChanged) {
        for (size_t i = 0; i < rows_; ++i) {
          picture[i][j] = newColumn[i];
        }
        changed = true;
      }
    }
  }

  bool isFullySolved = true;
  for (size_t i = 0; i < rows_ && isFullySolved; ++i) {
    for (size_t j = 0; j < cols_ && isFullySolved; ++j) {
      if (picture[i][j] == 0) {
        isFullySolved = false;
      }
    }
  }

  if (isFullySolved) {
    solution_ = picture;
    isSolvable_ = true;
  } else {
    solution_ = Vector< Vector< int > >();
    isSolvable_ = false;
  }
  return isSolvable_;
}
