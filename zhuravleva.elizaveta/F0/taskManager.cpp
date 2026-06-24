#include "taskManager.hpp"
#include <ostream>
#include <stdexcept>
#include <string>
#include <istream>

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

  size_t TaskManager::getCurrentLabor(const std::string& listName) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    size_t labor = 0;
    for (LCIter< TaskInList > it = list.tasks.cbegin();
        it != list.tasks.cend(); ++it)
    {
      labor += globalTasks_.get(it->taskId).labor;
    }
    return labor;
  }

  bool TaskManager::canAddTask(const std::string& listName,
      const std::string& taskId) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    if (!globalTasks_.contains(taskId))
    {
      throw std::logic_error("task not found");
    }
    const TaskList& list = lists_.get(listName);
    size_t currentLabor = getCurrentLabor(listName);
    size_t taskLabor = globalTasks_.get(taskId).labor;
    return (currentLabor + taskLabor <= list.maxLabor);
  }

  void TaskManager::checkAdd(const std::string& listName,
      const std::string& taskId, std::ostream& out) const
  {
    if (canAddTask(listName, taskId))
    {
      out << "OK\n";
    }
    else
    {
      out << "NOT OK\n";
    }
  }

  void TaskManager::markDone(const std::string& listName,
      const std::string& taskId, const std::string& date)
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    TaskList& list = lists_.get(listName);
    for (LIter< TaskInList > it = list.tasks.begin(); it != list.tasks.end(); ++it)
    {
      if (it->taskId == taskId)
      {
        it->done = true;
        it->doneDate = date;
        return;
      }
    }
    throw std::logic_error("task not found in list");
  }

  size_t TaskManager::countCompleted(const std::string& listName,
      const std::string& from, const std::string& to) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    size_t count = 0;
    for (LCIter< TaskInList > it = list.tasks.cbegin(); it != list.tasks.cend(); ++it)
    {
      if (it->done && it->doneDate >= from && it->doneDate <= to)
      {
        ++count;
      }
    }
    return count;
  }

  void TaskManager::setDeadline(const std::string& listName,
      const std::string& taskId, const std::string& deadline)
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    TaskList& list = lists_.get(listName);
    for (LIter< TaskInList > it = list.tasks.begin(); it != list.tasks.end(); ++it)
    {
      if (it->taskId == taskId)
      {
        it->deadline = deadline;
        return;
      }
    }
    throw std::logic_error("task not found in list");
  }

  void TaskManager::showExpired(const std::string& listName,
      const std::string& date, std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    for (LCIter< TaskInList > it = list.tasks.cbegin(); it != list.tasks.cend(); ++it)
    {
      if (!it->done && !it->deadline.empty() && it->deadline < date)
      {
        const Task& task = globalTasks_.get(it->taskId);
        out << it->taskId << " "
            << task.labor << " "
            << it->priority << " "
            << it->deadline << "\n";
      }
    }
  }

  void TaskManager::suggestRemoveOne(const std::string& listName,
      const std::string& taskId, std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    if (!globalTasks_.contains(taskId))
    {
      throw std::logic_error("task not found");
    }
    const TaskList& list = lists_.get(listName);
    size_t currentLabor = getCurrentLabor(listName);
    size_t newTaskLabor = globalTasks_.get(taskId).labor;
    if (currentLabor + newTaskLabor <= list.maxLabor)
    {
      out << "OK\n";
      return;
    }
    size_t required = currentLabor + newTaskLabor - list.maxLabor;
    bool found = false;
    for (LCIter< TaskInList > it = list.tasks.cbegin(); it != list.tasks.cend(); ++it)
    {
      const Task& task = globalTasks_.get(it->taskId);
      if (task.labor >= required)
      {
        out << it->taskId << "\n";
        found = true;
      }
    }
    if (!found)
    {
      out << "IMPOSSIBLE\n";
    }
  }

  size_t TaskManager::getFreeLabor(const std::string& listName) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    return list.maxLabor - getCurrentLabor(listName);
  }

  void TaskManager::showCapacity(const std::string& listName,
      std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    out << getCurrentLabor(listName)
        << " / "
        << list.maxLabor
        << "\n";
  }

  bool TaskManager::hasTaskInList(const std::string& listName,
      const std::string& taskId) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    for (LCIter< TaskInList > it = list.tasks.cbegin(); it != list.tasks.cend(); ++it)
    {
      if (it->taskId == taskId)
      {
        return true;
      }
    }
    return false;
  }

  void TaskManager::suggestRemove(const std::string& listName, const std::string& taskId,
      size_t maxRemove, std::ostream& out) const
  {
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    if (!globalTasks_.contains(taskId))
    {
      throw std::logic_error("task not found");
    }
    const TaskList& list = lists_.get(listName);
    size_t currentLabor = getCurrentLabor(listName);
    size_t newTaskLabor = globalTasks_.get(taskId).labor;
    if (currentLabor + newTaskLabor <= list.maxLabor)
    {
      out << "OK\n";
      return;
    }
    size_t required = currentLabor + newTaskLabor - list.maxLabor;
    List< std::string > selected;
    bool found = false;
    suggestRemoveRecursive(list.tasks.cbegin(), list.tasks.cend(),
        maxRemove, required, 0, selected, found, out);
    if (!found)
    {
      out << "IMPOSSIBLE\n";
    }
  }

  void TaskManager::suggestRemoveRecursive(LCIter< TaskInList > current,
      LCIter< TaskInList > end, size_t maxRemove, size_t required,
    size_t currentSum, List< std::string >& selected, bool& found, std::ostream& out) const
  {
    if (currentSum >= required && !selected.empty())
    {
      for (LCIter< std::string > it = selected.cbegin(); it != selected.cend(); ++it)
      {
        out << *it << " ";
      }
      out << "\n";
      found = true;
      return;
    }
    if (current == end || selected.size() == maxRemove)
    {
      return;
    }
    const Task& task = globalTasks_.get(current->taskId);
    selected.pushBack(current->taskId);
    LCIter< TaskInList > next = current;
    ++next;
    suggestRemoveRecursive(next, end, maxRemove, required, currentSum + task.labor, selected, found, out);
    selected.popBack();
    suggestRemoveRecursive(next, end, maxRemove, required, currentSum, selected, found, out);
  }

  size_t TaskManager::getPriorityScore(const TaskInList& task) const
  {
    if (task.priority >= 1000000)
    {
      return 0;
    }
    return 1000000 - task.priority;
  }

  void TaskManager::optimize(const std::string& newListName, const std::string& listName, size_t maxLabor)
  {
    if (lists_.contains(newListName))
    {
      throw std::logic_error("list already exists");
    }
    if (!lists_.contains(listName))
    {
      throw std::logic_error("list not found");
    }
    const TaskList& list = lists_.get(listName);
    List< TaskInList > currentList;
    List< TaskInList > bestList;
    size_t bestScore = 0;
    optimizeRecursive(list.tasks.cbegin(), list.tasks.cend(), maxLabor,
        0, 0, currentList, bestScore, bestList);
    TaskList result(newListName, maxLabor);
    for (LCIter< TaskInList > it = bestList.cbegin(); it != bestList.cend(); ++it)
    {
      result.tasks.pushBack(*it);
    }
    lists_.insert(newListName, result);
  }

  void TaskManager::optimizeRecursive(LCIter< TaskInList > current, LCIter< TaskInList > end, size_t maxLabor,
      size_t currentLabor, size_t currentScore, List< TaskInList >& currentList,
      size_t& bestScore, List< TaskInList >& bestList) const
  {
    if (current == end)
    {
      if (currentScore > bestScore)
      {
        bestScore = currentScore;
        bestList = currentList;
      }
      return;
    }
    const Task& task = globalTasks_.get(current->taskId);
    LCIter< TaskInList > next = current;
    ++next;
    if (currentLabor + task.labor <= maxLabor)
    {
      currentList.pushBack(*current);
      optimizeRecursive(next, end, maxLabor, currentLabor + task.labor,
           currentScore + getPriorityScore(*current), currentList, bestScore, bestList);
      currentList.popBack();
    }
    optimizeRecursive(next, end, maxLabor, currentLabor, currentScore, currentList, bestScore, bestList);
  }

  void TaskManager::save(std::ostream& out) const
  {
    out << "TASKS " << globalTasks_.size() << "\n";
    for (CuckooHashTable< std::string, Task >::ConstIterator it = globalTasks_.cbegin();
        it != globalTasks_.cend(); ++it)
    {
      out << it->first << " " << it->second.labor << "\n";
    }
    out << "LISTS " << lists_.size() << "\n";
    for (CuckooHashTable< std::string, TaskList >::ConstIterator it = lists_.cbegin();
        it != lists_.cend(); ++it)
    {
      out << it->first << " "  << it->second.maxLabor << "\n";
    }
    size_t itemsCount = 0;
    for (CuckooHashTable< std::string, TaskList >::ConstIterator it = lists_.cbegin();
        it != lists_.cend(); ++it)
    {
      itemsCount += it->second.tasks.size();
    }
    out << "ITEMS " << itemsCount << "\n";
    for (CuckooHashTable< std::string, TaskList >::ConstIterator it = lists_.cbegin();
        it != lists_.cend(); ++it)
    {
      const TaskList& list = it->second;
      for (LCIter< TaskInList > taskIt = list.tasks.cbegin();
          taskIt != list.tasks.cend(); ++taskIt)
      {
        std::string deadline = taskIt->deadline;
        std::string doneDate = taskIt->doneDate;
        if (deadline.empty())
        {
          deadline = "-";
        }
        if (doneDate.empty())
        {
          doneDate = "-";
        }
        out << it->first << " " << taskIt->taskId << " "
            << taskIt->priority << " " << taskIt->done << " "
            << deadline << " " << doneDate << "\n";
      }
    }
  }

  void TaskManager::load(std::istream& in)
  {
    TaskManager temp;
    std::string marker;
    size_t count = 0;
    in >> marker >> count;
    if (!in || marker != "TASKS")
    {
      throw std::logic_error("invalid file");
    }
    for (size_t i = 0; i < count; ++i)
    {
      std::string taskId;
      size_t labor = 0;
      in >> taskId >> labor;
      if (!in)
      {
        throw std::logic_error("invalid file");
      }
      temp.addTaskGlobal(taskId, labor);
    }
    in >> marker >> count;
    if (!in || marker != "LISTS")
    {
      throw std::logic_error("invalid file");
    }
    for (size_t i = 0; i < count; ++i)
    {
      std::string listName;
      size_t maxLabor = 0;
      in >> listName >> maxLabor;
      if (!in)
      {
        throw std::logic_error("invalid file");
      }
      temp.createList(listName, maxLabor);
    }
    in >> marker >> count;
    if (!in || marker != "ITEMS")
    {
      throw std::logic_error("invalid file");
    }
    for (size_t i = 0; i < count; ++i)
    {
      std::string listName;
      std::string taskId;
      size_t priority = 0;
      bool done = false;
      std::string deadline;
      std::string doneDate;
      in >> listName >> taskId >> priority >> done >> deadline >> doneDate;
      if (!in)
      {
        throw std::logic_error("invalid file");
      }
      if (deadline == "-")
      {
        deadline.clear();
      }
      if (doneDate == "-")
      {
        doneDate.clear();
      }
      temp.addTaskToList(listName, taskId, priority);
      temp.setDeadline(listName, taskId, deadline);
      if (done)
      {
        temp.markDone(listName, taskId, doneDate);
      }
    }
    swap(temp);
  }

  void TaskManager::swap(TaskManager& other) noexcept
  {
    globalTasks_.swap(other.globalTasks_);
    lists_.swap(other.lists_);
  }
}
