#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "operation.hpp"
#include "saving.hpp"
#include "../common/vector.hpp"

#include <string>

namespace ulanova
{
  struct Profile
  {
    std::string name;
    long long balance;
    Vector< Saving > savings;
    Vector< Operation > operations;
  };
}

#endif
