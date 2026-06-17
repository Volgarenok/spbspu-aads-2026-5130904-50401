#include "template.hpp"
#include <fstream>
#include <sstream>

madieva::Template::Template(int rows, int cols):
  rows_(rows),
  cols_(cols),
  rowHints_(),
  colHints_(),
  solution_(),
  isSolvable_(false)
{
  rowHints_.reserve(rows_);
  colHints_.reserve(cols_);
  
  for (int i = 0; i < rows_; ++i) {
    rowHints_.pushBack(Vector< int >());
  }
  
  for (int i = 0; i < cols_; ++i) {
    colHints_.pushBack(Vector< int >());
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
  rowHints_ = Vector< Vector< int > >();
  colHints_ = Vector< Vector< int > >();
  rowHints_.reserve(rows_);
  colHints_.reserve(cols_);
  
  for (int i = 0; i < rows_; ++i) {
    rowHints_.pushBack(Vector< int >());
  }
  
  for (int i = 0; i < cols_; ++i) {
    colHints_.pushBack(Vector< int >());
  }

  std::string line;
  std::getline(file, line);
  for (int i = 0; i < rows_; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    int num;
    while (iss >> num) {
      if (num > 0) {
        rowHints_[i].pushBack(num);
      }
    }
  }

  for (int i = 0; i < cols_; ++i) {
    std::getline(file, line);
    std::istringstream iss(line);
    int num;
    while (iss >> num) {
      if (num > 0) {
        colHints_[i].pushBack(num);
      }
    }
  }

  isSolvable_ = solve();

  return true;
}
