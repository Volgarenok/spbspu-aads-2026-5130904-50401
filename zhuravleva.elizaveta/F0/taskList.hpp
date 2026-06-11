#ifndef TASK_LIST_HPP
#define TASK_LIST_HPP
#include <cstddef>
#include <string>
#include "../common/list.hpp"

namespace zhuravleva
{
  struct TaskInList
  {
    std::string taskId;
    size_t priority;
    bool done;
    std::string deadline;
    std::string doneDate;

    TaskInList();
    TaskInList(const std::string& id, size_t taskPriority);
  };

  struct TaskList
  {
    std::string name;
    size_t maxLabor;
    List< TaskInList > tasks;

    TaskList();
    TaskList(const std::string& listName, size_t limit);
  };
}

#endif
