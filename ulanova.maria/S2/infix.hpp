#ifndef INFIX_HPP
#define INFIX_HPP

#include "stack.hpp"
#include "math_functions.hpp"
#include "queue.hpp"

#include <string>
#include <sstream>

namespace ulanova
{
  Queue< std::string > infix_to_postfix(const std::string& expr);
}
#endif
