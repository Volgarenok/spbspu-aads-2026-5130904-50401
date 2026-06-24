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
