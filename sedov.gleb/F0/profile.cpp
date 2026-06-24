#include "profile.hpp"
#include <string>

sedov::Profile::Profile(const std::string & name):
  name_(name),
  nextTaskId_(1)
{}

const std::string & sedov::Profile::getName() const noexcept
{
  return name_;
}

size_t sedov::Profile::unplacedCount() const noexcept
{
  return unplacedTasks_.size();
}

bool sedov::Profile::addSchedule(const std::string & schedName)
{
  if (scheduleExists(schedName))
  {
    return false;
  }
  schedules_.insert(ScheduleKey{schedName}, Schedule(schedName));
  return true;
}

bool sedov::Profile::removeSchedule(const std::string & schedName)
{
  ScheduleKey key{schedName};
  if (!schedules_.contains(key))
  {
    return false;
  }
  Schedule sch;
  if (schedules_.find(key, sch))
  {
    List< Task > tasks = sch.getTasksInRange("0000-01-01", "9999-12-31");
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
      Task t = *it;
      t.setActive(false);
      unplacedTasks_.pushBack(t);
    }
  }
  schedules_.erase(key);
  return true;
}

bool sedov::Profile::findSchedule(const std::string & schedName, Schedule & outSched) const
{
  return schedules_.find(ScheduleKey{schedName}, outSched);
}

void sedov::Profile::getAllSchedules(List< Schedule > & outScheds) const
{
  List< std::pair< const ScheduleKey, Schedule > > raw;
  schedules_.getAll(raw);
  for (auto it = raw.begin(); it != raw.end(); ++it)
  {
    outScheds.pushBack((*it).second);
  }
}

bool sedov::Profile::scheduleExists(const std::string & schedName) const
{
  return schedules_.contains(ScheduleKey{schedName});
}

bool sedov::Profile::addTaskToSchedule(const std::string & schedName, const Task & task)
{
  ScheduleKey key{schedName};
  Schedule sch;
  if (!schedules_.find(key, sch))
  {
    return false;
  }
  sch.addTask(task);
  schedules_.insert(key, sch);
  return true;
}

bool sedov::Profile::removeTaskFromSchedule(const std::string & schedName, int id)
{
  ScheduleKey key{schedName};
  Schedule sch;
  if (!schedules_.find(key, sch))
  {
    return false;
  }
  if (!sch.removeTask(id))
  {
    return false;
  }
  schedules_.insert(key, sch);
  return true;
}

bool sedov::Profile::findTaskInSchedule(const std::string & schedName, int id, Task & outTask) const
{
  Schedule sch;
  if (!schedules_.find(ScheduleKey{schedName}, sch))
  {
    return false;
  }
  return sch.findTask(id, outTask);
}

void sedov::Profile::addToUnplaced(const Task & task)
{
  unplacedTasks_.pushBack(task);
}

bool sedov::Profile::removeFromUnplaced(int id)
{
  for (auto it = unplacedTasks_.begin(); it != unplacedTasks_.end(); ++it)
  {
    if ((*it).getId() == id)
    {
      unplacedTasks_.erase(it);
      return true;
    }
  }
  return false;
}

void sedov::Profile::getAllUnplaced(List< Task > & outTasks) const
{
  outTasks = unplacedTasks_;
}

sedov::Task * sedov::Profile::findUnplacedTask(int id)
{
  for (auto it = unplacedTasks_.begin(); it != unplacedTasks_.end(); ++it)
  {
    if ((*it).getId() == id)
    {
      return &(*it);
    }
  }
  return nullptr;
}

bool sedov::Profile::updateSchedule(const std::string & schedName, const Schedule & sch)
{
  if (!scheduleExists(schedName))
  {
    return false;
  }
  schedules_.insert(ScheduleKey{schedName}, sch);
  return true;
}

int sedov::Profile::getNextTaskId() const noexcept
{
  return nextTaskId_;
}

int sedov::Profile::generateTaskId()
{
  return nextTaskId_++;
}

void sedov::Profile::setNextTaskId(int id) noexcept
{
  nextTaskId_ = id;
}

void sedov::Profile::clear() noexcept
{
  schedules_.clear();
  unplacedTasks_.clear();
  nextTaskId_ = 1;
}
