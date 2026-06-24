#include "scheduler.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <ctime>

namespace
{
  std::string nextDay(const std::string & date)
  {
    int y, m, d;
    if (!sedov::parseDate(date, y, m, d))
    {
      return date;
    }
    const int dim[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    bool leap = (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
    int maxd = (m == 2 && leap) ? 29 : dim[m-1];
    d++;
    if (d > maxd)
    {
      d = 1;
      m++;
    }
    if (m > 12)
    {
      m = 1;
      y++;
    }
    return sedov::pad(y) + "-" + sedov::pad(m) + "-" + sedov::pad(d);
  }

  int compareDates(const std::string & a, const std::string & b)
  {
    if (a < b)
    {
      return -1;
    }
    if (a > b)
    {
      return 1;
    }
    return 0;
  }
}

bool sedov::Scheduler::createProfile(const std::string & name)
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

bool sedov::Scheduler::deleteProfile(const std::string& name)
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

void sedov::Scheduler::listProfiles() const
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

bool sedov::Scheduler::createSchedule(const std::string & profName, const std::string & schedName)
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

bool sedov::Scheduler::deleteSchedule(const std::string & profName, const std::string & schedName)
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

bool sedov::Scheduler::listSchedules(const std::string & profName)
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

bool sedov::Scheduler::addTask(const std::string & profName, const std::string & schedName, const std::string & title,
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

bool sedov::Scheduler::removeTask(const std::string & profName, const std::string & schedName, int id)
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

bool sedov::Scheduler::viewTask(const std::string & profName, const std::string & schedName, int id)
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

bool sedov::Scheduler::listTasks(const std::string & profName, const std::string & schedName, const std::string & date)
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

bool sedov::Scheduler::showUnplaced(const std::string & profName)
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
  List< Task > raw;
  profile.getAllUnplaced(raw);
  Vector< Task > tasks;
  for (auto it = raw.begin(); it != raw.end(); ++it)
  {
    tasks.pushBack(*it);
  }
  for (size_t i = 0; i < tasks.getSize(); ++i)
  {
    for (size_t j = i + 1; j < tasks.getSize(); ++j)
    {
      bool swap = tasks[i].getDate() > tasks[j].getDate();
      if (tasks[i].getDate() == tasks[j].getDate())
      {
        swap = tasks[i].getTimeStart() > tasks[j].getTimeStart();
      }
      if (swap)
      {
        std::swap(tasks[i], tasks[j]);
      }
    }
  }
  std::cout << "Unplaced tasks in profile \"" << profName << "\":\n";
  if (tasks.getSize() == 0)
  {
    std::cout << "  No tasks\n";
  }
  else
  {
    for (size_t i = 0; i < tasks.getSize(); ++i)
    {
      std::cout << "  ID " << tasks[i].getId() << ": " << tasks[i].getTitle() << " | " << tasks[i].getDate() << " "
        << tasks[i].getTimeStart() << "-" << tasks[i].getTimeEnd() << " | " << tasks[i].getImportance() << "\n";
    }
    std::cout << "Total: " << tasks.getSize() << " tasks\n";
  }
  return true;
}

bool sedov::Scheduler::autoPlace(const std::string & profName, const std::string & dateFrom,
  const std::string & dateTo)
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
  int y1, m1, d1;
  if (!parseDate(dateFrom, y1, m1, d1))
  {
    std::cout << "[ERROR] Invalid date_from format: " << dateFrom << "\n";
    return false;
  }
  int y2, m2, d2;
  if (!parseDate(dateTo, y2, m2, d2))
  {
    std::cout << "[ERROR] Invalid date_to format: " << dateTo << "\n";
    return false;
  }
  if (dateFrom > dateTo)
  {
    std::cout << "[ERROR] date_from (" << dateFrom << ") must be <= date_to (" << dateTo << ")\n";
    return false;
  }
  List< Task > unplaced;
  profile.getAllUnplaced(unplaced);
  int placed = 0, failed = 0;
  std::cout << "Searching free windows for " << unplaced.size() << " unplaced tasks:\n\n";
  for (auto task_it = unplaced.begin(); task_it != unplaced.end(); ++task_it)
  {
    Task orig = *task_it;
    bool found = false;
    Schedule targetSched;
    if (profile.findSchedule(orig.getScheduleName(), targetSched))
    {
      Schedule sched = targetSched;
      if (orig.getDate() >= dateFrom && orig.getDate() <= dateTo && !sched.hasConflict(orig))
      {
        Task placedTask = orig;
        placedTask.setActive(true);
        sched.addTask(placedTask);
        profile.updateSchedule(sched.getName(), sched);
        profile.removeFromUnplaced(orig.getId());
        std::cout << "[PLACED] Task \"" << orig.getTitle() << "\" (" << orig.getDurationMinutes() << "min, "
          << orig.getImportance() << ") -> " << sched.getName() << " " << orig.getDate() << " "
          << orig.getTimeStart() << "-" << orig.getTimeEnd() << "\n";
        placed++;
        found = true;
      }
      else
      {
        Vector< TimeWindow > windows = findFreeWindowsInSchedule(sched, dateFrom, dateTo, orig.getDurationMinutes());
        for (size_t i = 0; i < windows.getSize() && !found; ++i)
        {
          std::string newEnd;
          std::string newStart = formatTime(windows[i].getStartMinutes());
          if (canPlaceTask(sched, orig, windows[i].getDate(), newStart, newEnd))
          {
            Task placedTask = orig;
            placedTask.setDate(windows[i].getDate());
            placedTask.setTimeStart(newStart);
            placedTask.setTimeEnd(newEnd);
            placedTask.setActive(true);
            sched.addTask(placedTask);
            profile.updateSchedule(sched.getName(), sched);
            profile.removeFromUnplaced(orig.getId());
            std::cout << "[PLACED] Task \"" << orig.getTitle() << "\" (" << orig.getDurationMinutes() << "min, "
              << orig.getImportance() << ") -> " << sched.getName() << " " << placedTask.getDate() << " "
              << placedTask.getTimeStart() << "-" << placedTask.getTimeEnd() << "\n";
            placed++;
            found = true;
          }
        }
      }
    }
    else
    {
      std::cout << "[WARN] Schedule \"" << orig.getScheduleName() << "\" for task \"" << orig.getTitle()
        << "\" no longer exists. Remove task? (y/n): ";
      char answer;
      std::cin >> answer;
      if (answer == 'y' || answer == 'Y')
      {
        profile.removeFromUnplaced(orig.getId());
        std::cout << "[OK] Task removed\n";
      }
      else
      {
        std::cout << "[INFO] Task kept in unplaced\n";
      }
      found = true;
      failed++;
    }
    if (!found)
    {
      std::cout << "[FAILED] Task \"" << orig.getTitle() << "\" (" << orig.getDurationMinutes() << "min, "
        << orig.getImportance() << ") - no suitable window in schedule \"" << orig.getScheduleName() << "\"\n";
      failed++;
    }
  }
  profiles_.insert(ProfileKey{profName}, profile);
  std::cout << "\nResult: " << placed << " placed, " << failed << " failed\n";
  return true;
}

bool sedov::Scheduler::findFreeWindow(const std::string & prof1, const std::string & sched1, const std::string & prof2,
  const std::string & sched2, const std::string & dateFrom, const std::string & dateTo, int minHours)
{
  if (prof1.empty() || prof2.empty())
  {
    std::cout << "[ERROR] Profile names cannot be empty\n";
    return false;
  }
  if (sched1.empty() || sched2.empty())
  {
    std::cout << "[ERROR] Schedule names cannot be empty\n";
    return false;
  }
  if (prof1 == prof2 && sched1 == sched2)
  {
    std::cout << "[ERROR] Cannot find free window in the same schedule\n";
    return false;
  }
  int y1, m1, d1, y2, m2, d2;
  if (!parseDate(dateFrom, y1, m1, d1))
  {
    std::cout << "[ERROR] Invalid date_from format: " << dateFrom << "\n";
    return false;
  }
  if (!parseDate(dateTo, y2, m2, d2))
  {
    std::cout << "[ERROR] Invalid date_to format: " << dateTo << "\n";
    return false;
  }
  if (minHours < 1 || minHours > 23)
  {
    std::cout << "[ERROR] min_hours must be in range [1; 23]\n";
    return false;
  }
  if (dateFrom > dateTo)
  {
    std::cout << "[ERROR] date_from must be <= date_to\n";
    return false;
  }
  Schedule sch1, sch2;
  if (!findSchedule(prof1, sched1, sch1))
  {
    std::cout << "[ERROR] Schedule \"" << sched1 << "\" not found in profile \"" << prof1 << "\"\n";
    return false;
  }
  if (!findSchedule(prof2, sched2, sch2))
  {
    std::cout << "[ERROR] Schedule \"" << sched2 << "\" not found in profile \"" << prof2 << "\"\n";
    return false;
  }
  int min_minutes = minHours * 60;
  std::cout << "Searching shared free windows\nProfiles and schedules: " << prof1 << "." << sched1
    << ", " << prof2 << "." << sched2 << "\nMin duration: " << minHours << " hours\nDate range: " << dateFrom
    << " - " << dateTo << "\n\n";
  Vector< TimeWindow > windows1 = findFreeWindowsInSchedule(sch1, dateFrom, dateTo, min_minutes);
  Vector< TimeWindow > windows2 = findFreeWindowsInSchedule(sch2, dateFrom, dateTo, min_minutes);
  Vector< TimeWindow > common;
  for (size_t i = 0; i < windows1.getSize(); ++i)
  {
    for (size_t j = 0; j < windows2.getSize(); ++j)
    {
      if (windows1[i].getDate() == windows2[j].getDate())
      {
        int start = std::max(windows1[i].getStartMinutes(), windows2[j].getStartMinutes());
        int end = std::min(windows1[i].getEndMinutes(), windows2[j].getEndMinutes());
        if (end - start >= min_minutes)
        {
          TimeWindow w(windows1[i].getDate(), start, end);
          common.pushBack(w);
        }
      }
    }
  }
  if (common.getSize() == 0)
  {
    std::cout << "No free windows found\n";
    return true;
  }
  int best = 0;
  for (size_t i = 1; i < common.getSize(); ++i)
  {
    if (common[i].getDurationMinutes() > common[best].getDurationMinutes())
    {
      best = i;
    }
  }
  for (size_t i = 0; i < common.getSize(); ++i)
  {
    std::cout << "  " << common[i].format() << (i == static_cast< size_t >(best) ? " [BEST]" : "") << "\n";
  }
  std::cout << "\n[OK] Best: " << common[best].format() << "\n";
  return true;
}

bool sedov::Scheduler::mergeSchedules(const std::string & profName, const std::string & newName,
  const std::string & sched1Name, const std::string & sched2Name)
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
  if (newName.empty())
  {
    std::cout << "[ERROR] New schedule name cannot be empty\n";
    return false;
  }
  if (sched1Name.empty() || sched2Name.empty())
  {
    std::cout << "[ERROR] Schedule names cannot be empty\n";
    return false;
  }
  Schedule sch1, sch2;
  if (!profile.findSchedule(sched1Name, sch1) || !profile.findSchedule(sched2Name, sch2))
  {
    std::cout << "[ERROR] Source schedule not found\n";
    return false;
  }
  if (sched1Name == sched2Name)
  {
    std::cout << "[ERROR] Cannot merge a schedule with itself\n";
    return false;
  }
  if (profile.scheduleExists(newName))
  {
    std::cout << "[ERROR] Schedule \"" << newName << "\" already exists\n";
    return false;
  }
  Schedule merged(newName);
  int conflicts = 0;
  List< Task > t1 = sch1.getTasksInRange("0000-01-01", "9999-12-31");
  for (auto it = t1.begin(); it != t1.end(); ++it)
  {
    Task task = *it;
    task.setScheduleName(newName);
    merged.addTask(task);
  }
  List< Task > t2 = sch2.getTasksInRange("0000-01-01", "9999-12-31");
  for (auto it = t2.begin(); it != t2.end(); ++it)
  {
    Task task = *it;
    task.setScheduleName(newName);
    if (merged.hasConflict(task))
    {
      List< Task > conflictTasks = merged.getTasksOnDate(task.getDate());
      bool resolved = false;
      for (auto cit = conflictTasks.begin(); cit != conflictTasks.end(); ++cit)
      {
        if (task.overlapsWith(*cit))
        {
          if (task.getImportanceValue() > (*cit).getImportanceValue())
          {
            Task old = *cit;
            merged.removeTask(old.getId());
            merged.addTask(task);
            profile.addToUnplaced(old);
            std::cout << "  Conflict: keeping \"" << task.getTitle() << "\", moving \"" << old.getTitle() << "\"\n";
          }
          else
          {
            profile.addToUnplaced(task);
            std::cout << "  Conflict: keeping \"" << (*cit).getTitle() << "\", moving \"" << task.getTitle()
              << "\"\n";
          }
          resolved = true;
          conflicts++;
          break;
        }
      }
      if (!resolved)
      {
        merged.addTask(task);
      }
    }
    else
    {
      merged.addTask(task);
    }
  }
  profile.addSchedule(newName);
  profile.updateSchedule(newName, merged);
  profiles_.insert(ProfileKey{profName}, profile);
  std::cout << "[OK] Schedule \"" << newName << "\" created\nTotal conflicts: " << conflicts << "\n";
  return true;
}

bool sedov::Scheduler::compareMerge(const std::string & profName, const std::string & sched1Name,
  const std::string & sched2Name)
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
  if (sched1Name.empty() || sched2Name.empty())
  {
    std::cout << "[ERROR] Schedule names cannot be empty\n";
    return false;
  }
  Schedule sch1, sch2;
  if (!profile.findSchedule(sched1Name, sch1) || !profile.findSchedule(sched2Name, sch2))
  {
    std::cout << "[ERROR] Schedule not found\n";
    return false;
  }
  if (sched1Name == sched2Name)
  {
    std::cout << "[ERROR] Cannot merge a schedule with itself\n";
    return false;
  }
  std::cout << "Tasks that will NOT be included in merged schedule:\n";
  List< Task > t2 = sch2.getTasksInRange("0000-01-01", "9999-12-31");
  int count = 0;
  for (auto it = t2.begin(); it != t2.end(); ++it)
  {
    if (sch1.hasConflict(*it))
    {
      std::cout << "  From " << sched2Name << ": ID " << (*it).getId() << ": " << (*it).getTitle() << " | "
        << (*it).getDate() << " " << (*it).getTimeStart() << "-" << (*it).getTimeEnd() << " | "
        << (*it).getImportance() << "\n";
        count++;
    }
  }
  std::cout << "Total: " << count << " task(s) will be moved to unplaced\n";
  return true;
}

bool sedov::Scheduler::optimizeRange(const std::string & profName, const std::string & dateFrom,
  const std::string & dateTo, const std::string & criterion, const Vector< std::string > & targetScheds)
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
  if (criterion != "importance" && criterion != "short" && criterion != "long")
  {
    std::cout << "[ERROR] Invalid criterion. Use: importance, short, long\n";
    return false;
  }
  int y1, m1, d1, y2, m2, d2;
  if (!parseDate(dateFrom, y1, m1, d1))
  {
    std::cout << "[ERROR] Invalid date_from format: " << dateFrom << "\n";
    return false;
  }
  if (!parseDate(dateTo, y2, m2, d2))
  {
    std::cout << "[ERROR] Invalid date_to format: " << dateTo << "\n";
    return false;
  }
  if (dateFrom > dateTo)
  {
    std::cout << "[ERROR] date_from must be <= date_to\n";
    return false;
  }
  List< Schedule > targetScheds1;
  if (targetScheds.isEmpty())
  {
    profile.getAllSchedules(targetScheds1);
  }
  else
  {
    for (size_t idx = 0; idx < targetScheds.getSize(); ++idx)
    {
      const std::string & name = targetScheds[idx];
      if (name.empty())
      {
        std::cout << "[ERROR] Schedule name cannot be empty\n";
        return false;
      }
      Schedule s;
      if (!profile.findSchedule(name, s))
      {
        std::cout << "[ERROR] Schedule \"" << name << "\" not found\n";
        return false;
      }
      targetScheds1.pushBack(s);
    }
  }
  int moved = 0;
  for (auto sch_it = targetScheds1.begin(); sch_it != targetScheds1.end(); ++sch_it)
  {
    Schedule schedule = *sch_it;
    List< Task > schedTasks = schedule.getTasksInRange(dateFrom, dateTo);
    Vector< Task > pool;
    for (auto it = schedTasks.begin(); it != schedTasks.end(); ++it)
    {
      pool.pushBack(*it);
    }
    List< Task > unplaced;
    profile.getAllUnplaced(unplaced);
    for (auto it = unplaced.begin(); it != unplaced.end(); ++it)
    {
      if ((*it).getDate() >= dateFrom && (*it).getDate() <= dateTo && (*it).getScheduleName() == schedule.getName())
      {
        pool.pushBack(*it);
      }
    }
    for (size_t i = 0; i < pool.getSize(); ++i)
    {
      for (size_t j = i + 1; j < pool.getSize(); ++j)
      {
        bool need_swap = false;
        if (criterion == "importance")
        {
          need_swap = pool[i].getImportanceValue() < pool[j].getImportanceValue();
        }
        else if (criterion == "short")
        {
          need_swap = pool[i].getDurationMinutes() > pool[j].getDurationMinutes();
        }
        else if (criterion == "long")
        {
          need_swap = pool[i].getDurationMinutes() < pool[j].getDurationMinutes();
        }
        if (need_swap)
        {
          std::swap(pool[i], pool[j]);
        }
      }
    }
    List< Task > toRemove = schedule.getTasksInRange(dateFrom, dateTo);
    for (auto it = toRemove.begin(); it != toRemove.end(); ++it)
    {
      schedule.removeTask((*it).getId());
    }
    for (size_t i = 0; i < pool.getSize(); ++i)
    {
      Task t = pool[i];
      t.setScheduleName(schedule.getName());
      if (!schedule.hasConflict(t))
      {
        t.setActive(true);
        schedule.addTask(t);
      }
      else
      {
        if (t.isActive())
        {
          t.setActive(false);
          profile.addToUnplaced(t);
          moved++;
          std::cout << "  Moved to unplaced: \"" << t.getTitle() << "\" on " << t.getDate() << "\n";
        }
        else
        {
          std::cout << "  Staying in unplaced: \"" << t.getTitle() << "\" on " << t.getDate() << "\n";
        }
      }
    }
    profile.updateSchedule(schedule.getName(), schedule);
  }
  profiles_.insert(ProfileKey{profName}, profile);
  std::cout << "[OK] Optimization complete. " << moved << " tasks moved to unplaced\n";
  return true;
}

bool sedov::Scheduler::findBestDay(const std::string & prof1, const std::string & sched1, const std::string & prof2,
  const std::string & sched2, int minHours)
{
  if (prof1.empty() || prof2.empty())
  {
    std::cout << "[ERROR] Profile names cannot be empty\n";
    return false;
  }
  if (sched1.empty() || sched2.empty())
  {
    std::cout << "[ERROR] Schedule names cannot be empty\n";
    return false;
  }
  if (prof1 == prof2 && sched1 == sched2)
  {
    std::cout << "[ERROR] Cannot find best day in the same schedule\n";
    return false;
  }
  if (minHours < 1 || minHours > 23)
  {
    std::cout << "[ERROR] min_hours must be in range [1; 23]\n";
    return false;
  }
  Schedule sch1, sch2;
  if (!findSchedule(prof1, sched1, sch1) || !findSchedule(prof2, sched2, sch2))
  {
    std::cout << "[ERROR] Schedule not found\n";
    return false;
  }
  int min_minutes = minHours * 60;
  std::cout << "Searching best day for profiles and schedules: " << prof1 << "." << sched1 << ", " << prof2 << "."
    << sched2 << "\nMinimum hours required: " << minHours << "\n\n";
  std::time_t now = std::time(nullptr);
  std::tm * tmNow = std::localtime(&now);
  std::string today = pad(tmNow->tm_year + 1900) + "-" + pad(tmNow->tm_mon + 1) + "-" + pad(tmNow->tm_mday);
  std::string weekEnd = today;
  std::string prevDate;
  bool dateStuck = false;
  for (int i = 0; i < 6; ++i)
  {
    prevDate = weekEnd;
    weekEnd = nextDay(weekEnd);
    if (weekEnd == prevDate)
    {
      std::cerr << "[WARN] findBestDay: date stuck at " << weekEnd << "\n";
      dateStuck = true;
      break;
    }
  }
  if (dateStuck)
  {
    std::cout << "[ERROR] Cannot calculate week range due to date error\n";
    return false;
  }
  Vector< TimeWindow > w1 = findFreeWindowsInSchedule(sch1, today, weekEnd, min_minutes);
  Vector< TimeWindow > w2 = findFreeWindowsInSchedule(sch2, today, weekEnd, min_minutes);
  Vector< std::string > bestDates;
  Vector< int > bestStarts;
  Vector< int > bestEnds;
  Vector< int > bestDurations;
  for (size_t i = 0; i < w1.getSize(); ++i)
  {
    for (size_t j = 0; j < w2.getSize(); ++j)
    {
      if (w1[i].getDate() == w2[j].getDate())
      {
        int s = std::max(w1[i].getStartMinutes(), w2[j].getStartMinutes());
        int e = std::min(w1[i].getEndMinutes(), w2[j].getEndMinutes());
        int dur = e - s;
        if (dur >= min_minutes)
        {
          bool found = false;
          for (size_t k = 0; k < bestDates.getSize(); ++k)
          {
            if (bestDates[k] == w1[i].getDate())
            {
              if (dur > bestDurations[k])
              {
                bestDurations[k] = dur;
                bestStarts[k] = s;
                bestEnds[k] = e;
              }
              found = true;
              break;
            }
          }
          if (!found)
          {
            bestDates.pushBack(w1[i].getDate());
            bestStarts.pushBack(s);
            bestEnds.pushBack(e);
            bestDurations.pushBack(dur);
          }
        }
      }
    }
  }
  if (bestDates.getSize() == 0)
  {
    std::cout << "No suitable day found\n";
    return true;
  }
  int bestIndex = 0;
  for (size_t i = 1; i < bestDurations.getSize(); ++i)
  {
    if (bestDurations[i] > bestDurations[bestIndex])
    {
      bestIndex = i;
    }
  }
  for (size_t i = 0; i < bestDates.getSize(); ++i)
  {
    std::cout << "  " << bestDates[i] << " " << formatTime(bestStarts[i]) << "-" << formatTime(bestEnds[i])
      << " (" << bestDurations[i] / 60 << "h " << bestDurations[i] % 60 << "m)";
    if (i == static_cast< size_t >(bestIndex))
    {
      std::cout << " [BEST]";
    }
    std::cout << "\n";
  }
  std::cout << "\n[OK] Best day: " << bestDates[bestIndex] << "\n     Longest free window: "
    << formatTime(bestStarts[bestIndex]) << "-" << formatTime(bestEnds[bestIndex])
    << " (" << bestDurations[bestIndex] / 60 << "h " << bestDurations[bestIndex] % 60 << "m)\n";
  return true;
}

bool sedov::Scheduler::exportProfile(const std::string & name, const std::string & filename)
{
  if (name.empty())
  {
    std::cout << "[ERROR] Profile name cannot be empty\n";
    return false;
  }
  Profile profile;
  if (!findProfile(name, profile))
  {
    std::cout << "[ERROR] Profile \"" << name << "\" not found\n";
    return false;
  }
  if (filename.empty())
  {
    std::cout << "[ERROR] Filename cannot be empty\n";
    return false;
  }
  std::ofstream file(filename);
  if (!file.is_open())
  {
    std::cout << "[ERROR] Cannot create file: " << filename << "\n";
    return false;
  }
  file << "Profile: " << name << "\n";
  file << "Next task ID: " << profile.getNextTaskId() << "\n\n";
  file << "Schedules:\n";
  List< Schedule > scheds;
  profile.getAllSchedules(scheds);
  for (auto it = scheds.begin(); it != scheds.end(); ++it)
  {
    file << "  " << (*it).getName() << ":\n";
    List< Task > raw = (*it).getTasksInRange("0000-01-01", "9999-12-31");
    Vector< Task > tasks;
    for (auto tit = raw.begin(); tit != raw.end(); ++tit)
    {
      tasks.pushBack(*tit);
    }
    for (size_t i = 0; i < tasks.getSize(); ++i)
    {
      for (size_t j = i + 1; j < tasks.getSize(); ++j)
      {
        if (tasks[i].getDate() > tasks[j].getDate() || (tasks[i].getDate() == tasks[j].getDate() &&
          tasks[i].getTimeStart() > tasks[j].getTimeStart()))
        {
          std::swap(tasks[i], tasks[j]);
        }
      }
    }
    for (size_t i = 0; i < tasks.getSize(); ++i)
    {
      file << "    ID " << tasks[i].getId() << ": " << tasks[i].getTitle() << " | " << tasks[i].getDate() << " "
        << tasks[i].getTimeStart() << "-" << tasks[i].getTimeEnd() << " | " << tasks[i].getImportance() << "\n";
    }
  }
  file << "\nUnplaced tasks:\n";
  List< Task > rawUnp;
  profile.getAllUnplaced(rawUnp);
  Vector< Task > unplaced;
  for (auto it = rawUnp.begin(); it != rawUnp.end(); ++it)
  {
    unplaced.pushBack(*it);
  }
  for (size_t i = 0; i < unplaced.getSize(); ++i)
  {
    for (size_t j = i + 1; j < unplaced.getSize(); ++j)
    {
      if (unplaced[i].getDate() > unplaced[j].getDate() || (unplaced[i].getDate() == unplaced[j].getDate() &&
        unplaced[i].getTimeStart() > unplaced[j].getTimeStart()))
      {
        std::swap(unplaced[i], unplaced[j]);
      }
    }
  }
  for (size_t i = 0; i < unplaced.getSize(); ++i)
  {
    file << "  ID " << unplaced[i].getId() << ": " << unplaced[i].getTitle() << " | " << unplaced[i].getDate() << " "
      << unplaced[i].getTimeStart() << "-" << unplaced[i].getTimeEnd() << " | " << unplaced[i].getImportance()
      << "\n";
  }
  file.close();
  std::cout << "[OK] Profile exported to " << filename << "\n";
  return true;
}

bool sedov::Scheduler::importProfile(const std::string & filename)
{
  if (filename.empty())
  {
    std::cout << "[ERROR] Filename cannot be empty\n";
    return false;
  }
  std::ifstream file(filename);
  if (!file.is_open())
  {
    std::cout << "[ERROR] File not found: " << filename << "\n";
    return false;
  }
  std::string line;
  if (!std::getline(file, line) || line.find("Profile: ") != 0)
  {
    std::cout << "[ERROR] Invalid format: missing 'Profile:' header\n";
    return false;
  }
  std::string profileName = line.substr(9);
  if (profileName.empty())
  {
    std::cout << "[ERROR] Invalid format: empty profile name\n";
    return false;
  }
  std::string importName = profileName;
  int counter = 1;
  while (profiles_.contains(ProfileKey{importName}))
  {
    importName = profileName + "_imported";
    if (counter > 1)
    {
      importName += "_" + std::to_string(counter);
    }
    counter++;
  }
  Profile newProfile(importName);
  if (std::getline(file, line) && line.find("Next task ID: ") == 0)
  {
    try
    {
      int id = std::stoi(line.substr(14));
      if (id >= 1)
      {
        newProfile.setNextTaskId(id);
      }
    }
    catch (...)
    {}
  }
  std::getline(file, line);
  std::string curSched;
  while (std::getline(file, line))
  {
    if (line.size() >= 2 && line[0] == ' ' && line[1] == ' ' && line[2] != ' ')
    {
      size_t colon = line.find(':');
      if (colon != std::string::npos)
      {
        curSched = line.substr(2, colon - 2);
        if (!curSched.empty())
        {
          newProfile.addSchedule(curSched);
        }
      }
    }
    else if (line.size() >= 6 && line.substr(0, 6) == "    ID " && !curSched.empty())
    {
      size_t pos = 6;
      size_t idEnd = line.find(':', pos);
      if (idEnd == std::string::npos)
      {
        continue;
      }
      int id = std::stoi(line.substr(pos, idEnd - pos));
      pos = idEnd + 2;
      size_t titleEnd = line.find(" | ", pos);
      if (titleEnd == std::string::npos)
      {
        continue;
      }
      std::string title = line.substr(pos, titleEnd - pos);
      pos = titleEnd + 3;
      size_t dateEnd = line.find(' ', pos);
      if (dateEnd == std::string::npos)
      {
        continue;
      }
      std::string date = line.substr(pos, dateEnd - pos);
      pos = dateEnd + 1;
      size_t timesEnd = line.find(" | ", pos);
      if (timesEnd == std::string::npos)
      {
        continue;
      }
      std::string times = line.substr(pos, timesEnd - pos);
      size_t dash = times.find('-');
      if (dash == std::string::npos)
      {
        continue;
      }
      std::string timeStart = times.substr(0, dash);
      std::string timeEnd = times.substr(dash + 1);
      pos = timesEnd + 3;
      std::string importance = line.substr(pos);
      if (importance != "low" && importance != "mid" && importance != "high")
      {
        importance = "mid";
      }
      newProfile.addTaskToSchedule(curSched, Task(id, title, date, timeStart, timeEnd, importance, curSched, true));
    }
    else if (line.size() >= 8 && line.substr(0, 8) == "Unplaced")
    {
      break;
    }
  }
  if (line.size() >= 8 && line.substr(0, 8) == "Unplaced")
  {
    while (std::getline(file, line))
    {
      if (line.size() >= 4 && line.substr(0, 4) == "  ID")
      {
        size_t pos = 4;
        size_t idEnd = line.find(':', pos);
        if (idEnd == std::string::npos)
        {
          continue;
        }
        int id = std::stoi(line.substr(pos, idEnd - pos));
        pos = idEnd + 2;
        size_t titleEnd = line.find(" | ", pos);
        if (titleEnd == std::string::npos)
        {
          continue;
        }
        std::string title = line.substr(pos, titleEnd - pos);
        pos = titleEnd + 3;
        size_t dateEnd = line.find(' ', pos);
        if (dateEnd == std::string::npos)
        {
          continue;
        }
        std::string date = line.substr(pos, dateEnd - pos);
        pos = dateEnd + 1;
        size_t timesEnd = line.find(" | ", pos);
        if (timesEnd == std::string::npos)
        {
          continue;
        }
        std::string times = line.substr(pos, timesEnd - pos);
        size_t dash = times.find('-');
        if (dash == std::string::npos)
        {
          continue;
        }
        std::string timeStart = times.substr(0, dash);
        std::string timeEnd = times.substr(dash + 1);
        pos = timesEnd + 3;
        std::string importance = line.substr(pos);
        if (importance != "low" && importance != "mid" && importance != "high")
        {
          importance = "mid";
        }
        newProfile.addToUnplaced(Task(id, title, date, timeStart, timeEnd, importance, "", false));
      }
    }
  }
  file.close();
  profiles_.insert(ProfileKey{importName}, newProfile);
  std::cout << "[OK] Profile loaded from " << filename << "\n";
  std::cout << "[OK] Imported as \"" << importName << "\"\n";
  return true;
}

bool sedov::Scheduler::findProfile(const std::string & name, Profile & outProf) const
{
  if (name.empty())
  {
    return false;
  }
  return profiles_.find(ProfileKey{name}, outProf);
}

bool sedov::Scheduler::findSchedule(const std::string & profName, const std::string & schedName,
  Schedule & outSched) const
{
  if (profName.empty() || schedName.empty())
  {
    return false;
  }
  Profile profile;
  if (!findProfile(profName, profile))
  {
    return false;
  }
  return profile.findSchedule(schedName, outSched);
}

bool sedov::Scheduler::findTask(const std::string & profName, const std::string & schedName, int id,
  Task & outTask) const
{
  if (profName.empty() || schedName.empty() || id <= 0)
  {
    return false;
  }
  Profile profile;
  if (!findProfile(profName, profile))
  {
    return false;
  }
  return profile.findTaskInSchedule(schedName, id, outTask);
}

sedov::Vector< sedov::TimeWindow > sedov::Scheduler::findFreeWindowsInSchedule(const Schedule & sched,
  const std::string & dateFrom, const std::string & dateTo, int min_duration_minutes) const
{
  Vector< TimeWindow > result;
  int y1, m1, d1, y2, m2, d2;
  if (!parseDate(dateFrom, y1, m1, d1) || !parseDate(dateTo, y2, m2, d2))
  {
    return result;
  }
  if (dateFrom > dateTo)
  {
    return result;
  }
  std::string curDate = dateFrom;
  int maxIter = 365;
  int iter = 0;
  while (compareDates(curDate, dateTo) <= 0 && iter < maxIter)
  {
    iter++;
    List< Task > tasks = sched.getTasksOnDate(curDate);
    Vector< std::pair< int,int > > busy;
    for (auto it = tasks.begin(); it != tasks.end(); ++it)
    {
      int sh, sm, eh, em;
      parseTime((*it).getTimeStart(), sh, sm);
      parseTime((*it).getTimeEnd(), eh, em);
      busy.pushBack({sh*60+sm, eh*60+em});
    }
    for (size_t i = 0; i < busy.getSize(); ++i)
    {
      for (size_t j = i+1; j < busy.getSize(); ++j)
      {
        if (busy[i].first > busy[j].first)
        {
          std::swap(busy[i], busy[j]);
        }
      }
    }
    int curStart = 0;
    for (size_t i = 0; i < busy.getSize(); ++i)
    {
      if (curStart + min_duration_minutes <= busy[i].first)
      {
        TimeWindow w(curDate, curStart, busy[i].first);
        if (w.getDurationMinutes() >= min_duration_minutes)
        {
          result.pushBack(w);
        }
      }
      if (curStart < busy[i].second)
      {
        curStart = busy[i].second;
      }
    }
    if (curStart + min_duration_minutes <= 1440)
    {
      TimeWindow w(curDate, curStart, 1440);
      if (w.getDurationMinutes() >= min_duration_minutes)
      {
        result.pushBack(w);
      }
    }
    std::string nextDate = nextDay(curDate);
    if (nextDate == curDate)
    {
      break;
    }
    curDate = nextDate;
  }
  return result;
}

bool sedov::Scheduler::canPlaceTask(const Schedule & sched, const Task & task, const std::string & newDate,
  const std::string & newTimeStart, std::string & outTimeEnd) const
{
  int y, m, d;
  if (!parseDate(newDate, y, m, d))
  {
    return false;
  }
  int dur = task.getDurationMinutes();
  int sh, sm;
  if (!parseTime(newTimeStart, sh, sm))
  {
    return false;
  }
  int start_min = sh*60 + sm, end_min = start_min + dur;
  if (end_min > 1440)
  {
    return false;
  }
  outTimeEnd = formatTime(end_min);
  Task tmp = task;
  tmp.setDate(newDate);
  tmp.setTimeStart(newTimeStart);
  tmp.setTimeEnd(outTimeEnd);
  return !sched.hasConflict(tmp);
}

void sedov::Scheduler::resolveConflict(Task & task1, Task & task2, const std::string & criterion, Task & keep,
  Task & move)
{
  if (criterion == "importance")
  {
    if (task1.getImportanceValue() >= task2.getImportanceValue())
    {
      keep = task1;
      move = task2;
    }
    else
    {
      keep = task2;
      move = task1;
    }
  }
  else if (criterion == "short")
  {
    if (task1.getDurationMinutes() <= task2.getDurationMinutes())
    {
      keep = task1;
      move = task2;
    }
    else
    {
      keep = task2;
      move = task1;
    }
  }
  else if (criterion == "long")
  {
    if (task1.getDurationMinutes() >= task2.getDurationMinutes())
    {
      keep = task1;
      move = task2;
    }
    else
    {
      keep = task2;
      move = task1;
    }
  }
  else
  {
    throw std::invalid_argument("Unknown criterion: " + criterion);
  }
}
