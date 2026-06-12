#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <string>
#include <iosfwd>
#include "../common/cuckooHashTable.hpp"
#include "task.hpp"
#include "taskList.hpp"

namespace zhuravleva
{
  class TaskManager
  {
  public:
    TaskManager();

    bool hasTask(const std::string& taskId) const;
    bool hasList(const std::string& listName) const;
    void createList(const std::string& listName, size_t maxLabor);
    void deleteList(const std::string& listName);
    void addTaskGlobal(const std::string& taskId, size_t labor);
    void removeTaskGlobal(const std::string& taskId);
    void addTaskToList(const std::string& listName,
        const std::string& taskId, size_t priority);
    void removeTaskFromList(const std::string& listName, const std::string& taskId);
    void showList(const std::string& listName, std::ostream& out) const;
    void highPriority(const std::string& listName, size_t count, std::ostream& out) const;
    size_t getCurrentLabor(const std::string& listName) const;
    bool canAddTask(const std::string& listName, const std::string& taskId) const;
    void checkAdd(const std::string& listName,
        const std::string& taskId, std::ostream& out) const;

    void markDone(const std::string& listName,
      const std::string& taskId, const std::string& date);

    size_t countCompleted(const std::string& listName,
        const std::string& from, const std::string& to) const;

  private:
    CuckooHashTable< std::string, Task > globalTasks_;
    CuckooHashTable< std::string, TaskList > lists_;
  };
}

#endif
