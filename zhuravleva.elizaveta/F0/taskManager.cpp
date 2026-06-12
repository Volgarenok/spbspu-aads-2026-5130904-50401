#include "taskManager.hpp"
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
}
