#include "scheduler.hpp"

namespace sedov
{
  Scheduler::Scheduler() noexcept
  {
    currentProfile_.clear();
  }

  bool Scheduler::createProfile(const std::string & name)
  {
    if (name.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    for (size_t i = 0; i < name.size(); ++i)
    {
      char c = name[i];
      if (!std::isalnum(c) && c != '_')
      {
        std::cout << "[ERROR] Profile name contains invalid characters. Use letters, digits, and underscores only\n";
        return false;
      }
    }
    if (profiles_.contains(ProfileKey{name}))
    {
      std::cout << "[ERROR] Profile \"" << name << "\" already exists\n";
      return false;
    }
    profiles_.insert(ProfileKey{name}, Profile(name));
    std::cout << "[OK] Profile \"" << name << "\" created\n";
    return true;
  }

  bool Scheduler::deleteProfile(const std::string& name)
  {
    if (name.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (!profiles_.erase(ProfileKey{name}))
    {
      std::cout << "[ERROR] Profile \"" << name << "\" not found\n";
      return false;
    }
    std::cout << "[OK] Profile \"" << name << "\" deleted\n";
    return true;
  }

  void Scheduler::listProfiles() const
  {
    List< std::pair< const ProfileKey, Profile > > raw;
    profiles_.getAll(raw);
    std::cout << "Profile list:\n";
    int i = 1;
    for (auto it = raw.begin(); it != raw.end(); ++it, ++i)
    {
      std::cout << "  " << i << ". " << (*it).second.getName() << "\n";
    }
  }

  bool Scheduler::createSchedule(const std::string & profName, const std::string & schedName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (profile.scheduleExists(schedName))
    {
      std::cout << "[ERROR] Schedule \"" << schedName << "\" already exists\n";
      return false;
    }
    profile.addSchedule(schedName);
    profiles_.insert(ProfileKey{profName}, profile);
    std::cout << "[OK] Schedule \"" << schedName << "\" created in profile \"" << profName << "\"\n";
    return true;
  }

  bool Scheduler::deleteSchedule(const std::string & profName, const std::string & schedName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (!profile.removeSchedule(schedName))
    {
      std::cout << "[ERROR] Schedule \"" << schedName << "\" not found\n";
      return false;
    }
    profiles_.insert(ProfileKey{profName}, profile);
    std::cout << "[OK] Schedule \"" << schedName << "\" deleted from profile \"" << profName << "\"\n";
    return true;
  }

  bool Scheduler::listSchedules(const std::string & profName)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    List< Schedule > scheds;
    profile.getAllSchedules(scheds);
    std::cout << "Schedules in profile \"" << profName << "\":\n";
    if (scheds.size() == 0)
    {
        std::cout << "  No schedules\n";
    }
    else
    {
      for (auto it = scheds.begin(); it != scheds.end(); ++it)
      {
        std::cout << "  - " << (*it).getName() << " (" << (*it).taskCount() << " tasks)\n";
      }
    }
    return true;
  }

  bool Scheduler::addTask(const std::string & profName, const std::string & schedName, const std::string & title,
    const std::string & date, const std::string & timeStart, const std::string & timeEnd, const std::string & imp)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    Schedule schedule;
    if (!profile.findSchedule(schedName, schedule))
    {
      std::cout << "[ERROR] Schedule \"" << schedName << "\" not found in profile \"" << profName << "\"\n";
      return false;
    }
    std::string cleanTitle = title;
    if (cleanTitle.size() >= 2 && cleanTitle.front() == '"' && cleanTitle.back() == '"')
    {
      cleanTitle = cleanTitle.substr(1, cleanTitle.size() - 2);
    }
    if (cleanTitle.empty())
    {
      std::cout << "[ERROR] Task title cannot be empty\n";
      return false;
    }
    int y, m, d;
    if (!parseDate(date, y, m, d))
    {
      std::cout << "[ERROR] Invalid date format: " << date << ". Use YYYY-MM-DD\n";
      return false;
    }
    int sh, sm;
    if (!parseTime(timeStart, sh, sm))
    {
      std::cout << "[ERROR] Invalid start time format: " << timeStart << ". Use HH:MM\n";
      return false;
    }
    int eh, em;
    if (!parseTime(timeEnd, eh, em))
    {
      std::cout << "[ERROR] Invalid end time format: " << timeEnd << ". Use HH:MM\n";
      return false;
    }
    int startMin = sh * 60 + sm;
    int endMin = eh * 60 + em;
    if (startMin >= endMin)
    {
      std::cout << "[ERROR] Start time must be less than end time\n";
      return false;
    }
    if (imp != "low" && imp != "mid" && imp != "high")
    {
      std::cout << "[ERROR] Invalid importance: " << imp << ". Use low, mid, or high\n";
      return false;
    }
    Task newTask(profile.generateTaskId(), cleanTitle, date, timeStart, timeEnd, imp, schedName, true);
    if (schedule.hasConflict(newTask))
    {
      std::cout << "[WARN] Conflict with existing task\n";
      newTask.setActive(false);
      profile.addToUnplaced(newTask);
      std::cout << "[OK] Task moved to unplaced due to conflict. ID: " << newTask.getId() << "\n";
    }
    else
    {
      schedule.addTask(newTask);
      profile.addTaskToSchedule(schedName, newTask);
      std::cout << "[OK] Task added to schedule \"" << schedName << "\". ID: " << newTask.getId() << "\n";
    }
    profiles_.insert(ProfileKey{profName}, profile);
    return true;
  }

  bool Scheduler::removeTask(const std::string & profName, const std::string & schedName, int id)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    if (id <= 0)
    {
      std::cout << "[ERROR] Invalid task ID: " << id << "\n";
      return false;
    }
    if (!profile.removeTaskFromSchedule(schedName, id))
    {
      std::cout << "[ERROR] Task ID " << id << " not found\n";
      return false;
    }
    profiles_.insert(ProfileKey{profName}, profile);
    std::cout << "[OK] Task removed\n";
    return true;
  }

  bool Scheduler::viewTask(const std::string & profName, const std::string & schedName, int id)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    Profile profile;
    if (!findProfile(profName, profile))
    {
      std::cout << "[ERROR] Profile \"" << profName << "\" not found\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    if (id <= 0)
    {
      std::cout << "[ERROR] Invalid task ID: " << id << "\n";
      return false;
    }
    Task task;
    if (!findTask(profName, schedName, id, task))
    {
      std::cout << "[ERROR] Task ID " << id << " not found\n";
      return false;
    }
    std::cout << "========================================\nID: " << task.getId() << "\nTitle: " << task.getTitle()
      << "\nSchedule: " << task.getScheduleName() << "\nDate: " << task.getDate() << "\nTime: " << task.getTimeStart()
      << " - " << task.getTimeEnd() << "\nImportance: " << task.getImportance() << "\n"
      << "========================================\n";
    return true;
  }

  bool Scheduler::listTasks(const std::string & profName, const std::string & schedName,
    const std::string & date)
  {
    if (profName.empty())
    {
      std::cout << "[ERROR] Profile name cannot be empty\n";
      return false;
    }
    if (schedName.empty())
    {
      std::cout << "[ERROR] Schedule name cannot be empty\n";
      return false;
    }
    int y, m, d;
    if (!parseDate(date, y, m, d))
    {
      std::cout << "[ERROR] Invalid date format: " << date << ". Use YYYY-MM-DD\n";
      return false;
    }
    Schedule schedule;
    if (!findSchedule(profName, schedName, schedule))
    {
      std::cout << "[ERROR] Schedule not found\n";
      return false;
    }
    List< Task > raw = schedule.getTasksOnDate(date);
    Vector< Task > tasks;
    for (auto it = raw.begin(); it != raw.end(); ++it)
    {
      tasks.pushBack(*it);
    }
    for (size_t i = 0; i < tasks.getSize(); ++i)
    {
      for (size_t j = i + 1; j < tasks.getSize(); ++j)
      {
        if (tasks[i].getTimeStart() > tasks[j].getTimeStart())
        {
          std::swap(tasks[i], tasks[j]);
        }
      }
    }
    std::cout << "Tasks for " << date << " (" << schedName << "):\n";
    if (tasks.getSize() == 0)
    {
        std::cout << "  No tasks\n";
    }
    else
    {
      for (size_t i = 0; i < tasks.getSize(); ++i)
      {
        std::cout << "  ID " << tasks[i].getId() << ": " << tasks[i].getTimeStart() << "-" << tasks[i].getTimeEnd() 
          << " [" << tasks[i].getImportance() << "] " << tasks[i].getTitle() << "\n";
      }
    }
    return true;
  }
}
