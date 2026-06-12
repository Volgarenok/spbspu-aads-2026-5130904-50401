#include "taskManager.hpp"
#include <ostream>
#include <stdexcept>

namespace zhuravleva
{
  TaskManager::TaskManager():
    globalTasks_(),
    lists_()
  {}

  bool TaskManager::hasTask(const std::string& taskId) const
  {
    return globalTasks_.contains(taskId);
  }

  bool TaskManager::hasList(const std::string& listName) const
  {
    return lists_.contains(listName);
  }

  void TaskManager::createList(const std::string& listName, size_t maxLabor)
  {
    if (lists_.contains(listName))
    {
      throw std::logic_error("list already exists");
    }
    lists_.insert(listName, TaskList(listName, maxLabor));
  }

  void TaskManager::deleteList(const std::string& listName)
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    lists_.erase(listName);
  }

  void TaskManager::addTaskGlobal(const std::string& taskId, size_t labor)
  {
    if (globalTasks_.contains(taskId))
    {
      throw std::logic_error("task already exists");
    }
    globalTasks_.insert(taskId, Task(taskId, labor));
  }

  void TaskManager::removeTaskGlobal(const std::string& taskId)
  {
    if (!globalTasks_.contains(taskId))
    {
      throw std::logic_error("task not found");
    }
    globalTasks_.erase(taskId);
  }

  void TaskManager::addTaskToList(const std::string& listName,
      const std::string& taskId, size_t priority)
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    if (!globalTasks_.contains(taskId))
    {
      throw std::logic_error("task not found");
    }
    TaskList& list = lists_.get(listName);
    for (LIter< TaskInList > it = list.tasks.begin(); it != list.tasks.end(); ++it)
    {
      if (it->taskId == taskId)
      {
        throw std::logic_error("task already in list");
      }
    }
    list.tasks.pushBack(TaskInList(taskId, priority));
  }

  void TaskManager::removeTaskFromList(const std::string& listName,
      const std::string& taskId)
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    TaskList& list = lists_.get(listName);
    LIter< TaskInList > prev = list.tasks.beforeBegin();
    LIter< TaskInList > current = list.tasks.begin();
    while (current != list.tasks.end())
    {
      if (current->taskId == taskId)
      {
        list.tasks.eraseAfter(prev);
        return;
      }
      ++prev;
      ++current;
    }
    throw std::logic_error("task not found in list");
  }

  void TaskManager::showList(const std::string& listName, std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    for (LCIter< TaskInList > it = list.tasks.cbegin(); it != list.tasks.cend(); ++it)
    {
      const Task& task = globalTasks_.get(it->taskId);
      out << it->taskId << " "
          << task.labor << " "
          << it->priority << " "
          << it->done << " "
          << it->deadline << "\n";
    }
  }

  void TaskManager::highPriority(const std::string& listName, size_t count, std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    List< TaskInList > sorted(list.tasks);
    struct PriorityCompare
    {
      bool operator()(const TaskInList& lhs, const TaskInList& rhs) const
      {
        return lhs.priority < rhs.priority;
      }
    };
    sorted.sort(PriorityCompare());
    size_t printed = 0;
    for (LCIter< TaskInList > it = sorted.cbegin();
        it != sorted.cend() && printed < count; ++it)
    {
      const Task& task = globalTasks_.get(it->taskId);
      out << it->taskId << " "
          << task.labor << " "
          << it->priority << "\n";
      ++printed;
    }
  }
}
