#include "taskManager.hpp"

zhuravleva::TaskManager::TaskManager():
  globalTasks_(),
  lists_()
{}

bool zhuravleva::TaskManager::hasTask(const std::string& taskId) const
{
  return globalTasks_.contains(taskId);
}

bool zhuravleva::TaskManager::hasList(const std::string& listName) const
{
  return lists_.contains(listName);
}
