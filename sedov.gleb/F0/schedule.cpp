#include "schedule.hpp"
#include <algorithm>

sedov::Schedule::Schedule(const std::string & name):
  name_(name),
  nextId_(1)
{}

const std::string & sedov::Schedule::getName() const noexcept
{
  return name_;
}

size_t sedov::Schedule::taskCount() const noexcept
{
  return tasksById_.size();
}

int sedov::Schedule::getNextId() const noexcept
{
  return nextId_;
}

void sedov::Schedule::setNextId(int id) noexcept
{
  nextId_ = id;
}

int sedov::Schedule::addTask(const Task & task)
{
  Task newTask = task;
  if (newTask.getId() == 0)
  {
    Task updatedTask(nextId_++, newTask.getTitle(), newTask.getDate(), newTask.getTimeStart(), newTask.getTimeEnd(),
        newTask.getImportance(), newTask.getScheduleName(), newTask.isActive());
    tasksById_.insert(TaskIDKey{updatedTask.getId()}, updatedTask);
    addToIndex(updatedTask);
    return updatedTask.getId();
  }
  Task existing;
  if (tasksById_.find(TaskIDKey{newTask.getId()}, existing))
  {
    Task updatedTask(nextId_++, newTask.getTitle(), newTask.getDate(), newTask.getTimeStart(), newTask.getTimeEnd(),
      newTask.getImportance(), newTask.getScheduleName(), newTask.isActive());
    tasksById_.insert(TaskIDKey{updatedTask.getId()}, updatedTask);
    addToIndex(updatedTask);
    return updatedTask.getId();
  }
  tasksById_.insert(TaskIDKey{newTask.getId()}, newTask);
  addToIndex(newTask);
  return newTask.getId();
}

bool sedov::Schedule::removeTask(int id) noexcept
{
  Task task;
  if (!tasksById_.find(TaskIDKey{id}, task))
  {
    return false;
  }
  if (!tasksById_.erase(TaskIDKey{id}))
  {
    return false;
  }
  removeFromIndex(task);
  return true;
}

bool sedov::Schedule::findTask(int id, Task & outTask) const noexcept
{
  return tasksById_.find(TaskIDKey{id}, outTask);
}

bool sedov::Schedule::updateTask(const Task & task)
{
  if (!tasksById_.contains(TaskIDKey{task.getId()}))
  {
    return false;
  }
  Task oldTask;
  findTask(task.getId(), oldTask);
  tasksById_.insert(TaskIDKey{task.getId()}, task);
  removeFromIndex(oldTask);
  addToIndex(task);
  return true;
}

sedov::List< sedov::Task > sedov::Schedule::getTasksOnDate(const std::string & date) const
{
  List< Task > result;
  List< std::pair< const TaskIDKey, Task > > allTasks;
  tasksById_.getAll(allTasks);
  for (auto it = allTasks.begin(); it != allTasks.end(); ++it)
  {
    const Task & task = (*it).second;
    if (task.getDate() == date)
    {
      result.pushBack(task);
    }
  }
  return result;
}

sedov::List< sedov::Task > sedov::Schedule::getTasksInRange(const std::string & fromDate,
  const std::string & toDate) const
{
  List< Task > result;
  List< std::pair< const TaskIDKey, Task > > allTasks;
  tasksById_.getAll(allTasks);
  for (auto it = allTasks.begin(); it != allTasks.end(); ++it)
  {
    const Task & task = (*it).second;
    if (task.getDate() >= fromDate && task.getDate() <= toDate)
    {
      result.pushBack(task);
    }
  }
  return result;
}

bool sedov::Schedule::hasConflict(const Task & task, int excludeId) const
{
  List< Task > tasksOnDate = getTasksOnDate(task.getDate());
  for (auto it = tasksOnDate.begin(); it != tasksOnDate.end(); ++it)
  {
    if ((*it).getId() != excludeId && (*it).overlapsWith(task))
    {
      return true;
    }
  }
  return false;
}

void sedov::Schedule::clear() noexcept
{
  tasksById_.clear();
  tasksByDatetime_.clear();
  nextId_ = 1;
}

void sedov::Schedule::addToIndex(const Task & task)
{
  tasksByDatetime_.insert(DateTimeKey(task.makeDatetimeKey()), task);
}

void sedov::Schedule::removeFromIndex(const Task & task)
{
  tasksByDatetime_.erase(DateTimeKey(task.makeDatetimeKey()));
}
