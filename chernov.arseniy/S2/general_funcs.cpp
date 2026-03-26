#include "general_funcs.hpp"

#include <cctype>
#include <stdexcept>
#include <queue.hpp>

#include "math_funcs.hpp"

chernov::Queue< std::string > chernov::processLine(const std::string & line)
{
  Queue< std::string > math_expression;
  size_t i = 0;
  while (line[i] != '\0') {
    size_t j = i;
    while (line[j] != '\0' && !std::isspace(line[j])) {
      ++j;
    }
    if (i != j) {
      math_expression.push(line.substr(i, j - i));
    }
    i = j + 1;
  }
  return math_expression;
}
