#ifndef SAVING_HPP
#define SAVING_HPP

#include <string>

namespace ulanova
{
  struct Saving
  {
    std::string name;
    long long current_sum;
    long long target_sum;
    int priority;
  };
}
#endif
