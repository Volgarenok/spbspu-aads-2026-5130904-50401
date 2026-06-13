#ifndef PROFILE_HPP
#define PROFILE_HPP
#include "avltree.hpp"
#include "../common/list.hpp"
#include "task.hpp"
#include "schedule.hpp"
#include <string>

namespace sedov
{
  class Profile
  {
  public:
    Profile() noexcept = default;
    Profile(const Profile &) = default;
    Profile(Profile &&) = default;
    Profile & operator=(const Profile &) = default;
    Profile & operator=(Profile &&) = default;
    ~Profile() = default;
    explicit Profile(const std::string & name);

    const std::string & getName() const noexcept;
    size_t unplacedCount() const noexcept;

    bool addSchedule(const std::string & schedName);
    bool removeSchedule(const std::string & schedName);
    bool findSchedule(const std::string & schedName, Schedule & outSched) const;
    void getAllSchedules(List< Schedule > & outScheds) const;
    bool scheduleExists(const std::string & schedName) const;

    bool addTaskToSchedule(const std::string & schedName, const Task & task);
    bool removeTaskFromSchedule(const std::string & schedName, int id);
    bool findTaskInSchedule(const std::string & schedName, int id, Task & outTask) const;

    void addToUnplaced(const Task & task);
    bool removeFromUnplaced(int id);
    void getAllUnplaced(List< Task > & outTasks) const;
    Task * findUnplacedTask(int id);

    bool updateSchedule(const std::string & schedName, const Schedule & sch);

    int getNextTaskId() const noexcept;
    int generateTaskId();
    void setNextTaskId(int id) noexcept;
    void clear() noexcept;

  private:
    std::string name_;
    AVLTree< ScheduleKey, Schedule > schedules_;
    List< Task > unplacedTasks_;
    int nextTaskId_;
  };
}

#endif
