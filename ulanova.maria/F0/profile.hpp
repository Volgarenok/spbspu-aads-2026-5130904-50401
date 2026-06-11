#ifndef PROFILE_HPP
#define PROFILE_HPP

#include "saving.hpp"

#include <vector>
#include <string>

namespace ulanova
{
  struct Profile
  {
    std::string name;
    long long balance;
    std::vector< Saving > savings;
  };
}

#endif
