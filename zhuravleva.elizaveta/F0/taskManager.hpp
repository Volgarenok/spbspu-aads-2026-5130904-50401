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

  private:
    CuckooHashTable< std::string, Task > globalTasks_;
    CuckooHashTable< std::string, TaskList > lists_;
  };
}

#endif
