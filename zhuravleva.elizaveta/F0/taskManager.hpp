#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <string>
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

  private:
    CuckooHashTable< std::string, Task > globalTasks_;
    CuckooHashTable< std::string, TaskList > lists_;
  };
}

#endif
