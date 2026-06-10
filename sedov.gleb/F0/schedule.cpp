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

  int Schedule::getNextId() const noexcept
  {
    return nextId_;
  }

  void Schedule::setNextId(int id) noexcept
  {
    nextId_ = id;
  }

  void Schedule::addToIndex(const Task & task)
  {
    tasksByDatetime_.insert(DateTimeKey(task.makeDatetimeKey()), task);
  }

  void Schedule::removeFromIndex(const Task & task)
  {
    tasksByDatetime_.erase(DateTimeKey(task.makeDatetimeKey()));
  }
}
