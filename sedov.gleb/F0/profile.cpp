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

  bool Profile::addSchedule(const std::string & schedName)
  {
    if (scheduleExists(schedName))
    {
      return false;
    }
    schedules_.insert(ScheduleKey{schedName}, Schedule(schedName));
    return true;
  }

  bool Profile::removeSchedule(const std::string & schedName)
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

  bool Profile::findSchedule(const std::string & schedName, Schedule & outSched) const
  {
    return schedules_.find(ScheduleKey{schedName}, outSched);
  }

  void Profile::getAllSchedules(List< Schedule > & outScheds) const
  {
    List< std::pair< const ScheduleKey, Schedule > > raw;
    schedules_.getAll(raw);
    for (auto it = raw.begin(); it != raw.end(); ++it)
    {
      outScheds.pushBack((*it).second);
    }
  }

  bool Profile::scheduleExists(const std::string & schedName) const
  {
    return schedules_.contains(ScheduleKey{schedName});
  }

  bool Profile::addTaskToSchedule(const std::string & scheduleName, const Task & task)
  {
    ScheduleKey key{scheduleName};
    Schedule sch;
    if (!schedules_.find(key, sch))
    {
      return false;
    }
    sch.addTask(task);
    schedules_.insert(key, sch);
    return true;
  }

  bool Profile::removeTaskFromSchedule(const std::string & scheduleName, int id)
  {
    ScheduleKey key{scheduleName};
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

  bool Profile::findTaskInSchedule(const std::string & scheduleName, int id, Task & outTask) const
  {
    Schedule sch;
    if (!schedules_.find(ScheduleKey{scheduleName}, sch))
    {
      return false;
    }
    return sch.findTask(id, outTask);
  }
}
