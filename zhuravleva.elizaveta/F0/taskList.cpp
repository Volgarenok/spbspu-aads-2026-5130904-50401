#include "taskList.hpp"

namespace zhuravleva
{
  TaskInList::TaskInList():
    taskId(),
    priority(0),
    done(false),
    deadline(),
    doneDate()
  {}

  TaskInList::TaskInList(const std::string& id, size_t taskPriority):
    taskId(id),
    priority(taskPriority),
    done(false),
    deadline(),
    doneDate()
  {}

  TaskList::TaskList():
    name(),
    maxLabor(0),
    tasks()
  {}

  TaskList::TaskList(const std::string& listName, size_t limit):
    name(listName),
    maxLabor(limit),
    tasks()
  {}
}
