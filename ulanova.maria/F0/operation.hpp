#ifndef OPERATION_HPP
#define OPERATION_HPP

#include "date.hpp"

namespace ulanova
{
  struct Operation
  {
    long long amount;
    Date date;
    bool is_income;
  };
}

#endif
