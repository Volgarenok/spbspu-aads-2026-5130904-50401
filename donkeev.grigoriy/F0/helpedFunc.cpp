#include "helpedFunc.hpp"

std::string donkeev::nextWord(const std::string& str, size_t& pos)
{
  while (pos < str.length() && str[pos] == ' ')
  {
    ++pos;
  }

  if (pos >= str.length())
  {
    return "";
  }
  
  size_t start = pos;
  while (pos < str.length() && str[pos] != ' ')
  {
    ++pos;
  }
  
  return str.substr(start, pos - start);
}

int donkeev::inputInt(const std::string& prompt)
{
  int value;
  std::cout << prompt;
  std::cin >> value;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return value;
}

std::string donkeev::inputString(const std::string& prompt)
{
  std::string value;
  std::cout << prompt;
  std::getline(std::cin, value);
  return value;
}

size_t donkeev::inputSize_t(const std::string& prompt)
{
  size_t value;
  std::cout << prompt;
  std::cin >> value;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return value;
}
