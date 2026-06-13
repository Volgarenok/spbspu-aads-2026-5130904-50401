#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP
#include "avltree.hpp"
#include "profile.hpp"
#include "../common/list.hpp"
#include "../common/vector.hpp"
#include "utils.hpp"
#include <string>

namespace sedov
{
  class Scheduler
  {
  public:
    Scheduler() noexcept = default;
    Scheduler(const Scheduler &) = default;
    Scheduler(Scheduler &&) = default;
    Scheduler & operator=(const Scheduler &) = default;
    Scheduler & operator=(Scheduler &&) = default;
    ~Scheduler() = default;

    bool createProfile(const std::string & name);
    bool deleteProfile(const std::string & name);
    void listProfiles() const;

    bool createSchedule(const std::string & profName, const std::string & schedName);
    bool deleteSchedule(const std::string & profName, const std::string & schedName);
    bool listSchedules(const std::string & profName);

    bool addTask(const std::string & profName, const std::string & schedName, const std::string & title,
      const std::string & date, const std::string & timeStart, const std::string & timeEnd, const std::string & imp);

    bool removeTask(const std::string & profName, const std::string & schedName, int id);
    bool viewTask(const std::string & profName, const std::string & schedName, int id);
    bool listTasks(const std::string & profName, const std::string & schedName, const std::string & date);
    bool showUnplaced(const std::string & profName);

    bool autoPlace(const std::string & profName, const std::string & dateFrom, const std::string & dateTo);
    bool findFreeWindow(const std::string & prof1, const std::string & sched1, const std::string & prof2,
      const std::string & sched2, const std::string & dateFrom, const std::string & dateTo, int minHours);

    bool mergeSchedules(const std::string & profName, const std::string & newName, const std::string & sched1Name,
      const std::string & sched2Name);

    bool compareMerge(const std::string & profName, const std::string & sched1Name, const std::string & sched2Name);
    bool optimizeRange(const std::string & profName, const std::string & dateFrom, const std::string & dateTo,
      const std::string & criterion, const std::vector< std::string > & targetScheds = {});

    bool findBestDay(const std::string & prof1, const std::string & sched1, const std::string & prof2,
      const std::string & sched2, int minHours);

    bool exportProfile(const std::string & name, const std::string & filename);
    bool importProfile(const std::string & filename);

  private:
    AVLTree< ProfileKey, Profile > profiles_;

    bool findProfile(const std::string & name, Profile & outProf) const;
    bool findSchedule(const std::string & profName, const std::string & schedName,
      Schedule & outSched) const;

    bool findTask(const std::string & profName, const std::string & schedName, int id, Task & outTask) const;
    Vector< TimeWindow > findFreeWindowsInSchedule(const Schedule & sched, const std::string & dateFrom,
      const std::string & dateTo, int minDurMin) const;

    bool canPlaceTask(const Schedule & sched, const Task & task, const std::string & newDate,
      const std::string & newTimeStart, std::string & outTimeEnd) const;

    void resolveConflict(Task & task1, Task & task2, const std::string & criterion, Task & keep, Task & move);
  };
}

#endif
