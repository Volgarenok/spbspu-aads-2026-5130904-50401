#ifndef SCHEDULE_HPP
#define SCHEDULE_HPP
#include "avltree.hpp"
#include "list.hpp"
#include "task.hpp"
#include <algorithm>

namespace sedov
{
  class Schedule
  {
  public:
    Schedule() noexcept;
    explicit Schedule(const std::string & name);

    const std::string & getName() const noexcept;
    size_t taskCount() const noexcept;

    int getNextId() const noexcept;
    void setNextId(int id) noexcept;

    int addTask(const Task & task);
    bool removeTask(int id) noexcept;
    bool findTask(int id, Task & outTask) const noexcept;
    bool updateTask(const Task & task);
    List< Task > getTasksOnDate(const std::string & date) const;
    List< Task > getTasksInRange(const std::string & fromDate, const std::string & toDate) const;

    bool hasConflict(const Task & task, int excludeId = -1) const;
    void clear() noexcept;
  private:
      std::string name_;
      AVLTree< TaskIDKey, Task > tasksById_;
      AVLTree< DateTimeKey, Task > tasksByDatetime_;
      int nextId_;

      void addToIndex(const Task & task);
      void removeFromIndex(const Task & task);
  };
}

#endif
