#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include "queue.hpp"

namespace tarasenko
{
  long long calculate(std::string line);
  tarasenko::Queue< char > stringToQueue(std::string line);
  long long getNumber(std::string line, size_t& pos);
}

#endif
