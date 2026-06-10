#include "schedule.hpp"
#include <algorithm>

namespace sedov
{
  Schedule::Schedule() noexcept:
    nextId_(1)
  {}

  Schedule::Schedule(const std::string & name):
    name_(name),
    nextId_(1)
  {}

  const std::string & Schedule::getName() const noexcept
  {
    return name_;
  }

  size_t Schedule::taskCount() const noexcept
  {
    return tasksById_.size();
  }
}
