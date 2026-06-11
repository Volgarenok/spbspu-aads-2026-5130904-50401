#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>

namespace ulanova
{
  struct Operation
  {
    long long amount;
    std::string date;
    bool is_income;
  };
}

#endif
