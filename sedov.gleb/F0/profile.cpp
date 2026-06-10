#include "profile.hpp"
#include <string>

namespace sedov
{
  Profile::Profile() noexcept:
    nextTaskId_(1)
  {}

  Profile::Profile(const std::string & name):
    name_(name),
    nextTaskId_(1)
  {}

  const std::string & Profile::getName() const noexcept
  {
    return name_;
  }

  size_t Profile::unplacedCount() const noexcept
  {
    return unplacedTasks_.size();
  }
}
