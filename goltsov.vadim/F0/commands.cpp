#include "commands.hpp"
#include <stdexcept>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "structs.hpp"

std::istream& goltsov::detail::skipSpaces(std::istream& is)
{
  while (is.peek() == ' ')
  {
    is.get();
  }
  return is;
}
bool goltsov::detail::isEndOfLine(std::istream& is)
{
  if (is.peek() == '\n' || is.eof())
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool goltsov::detail::pushTask(State& current_state, Task& task, const TimeInterval& duration)
{
  if (current_state.current_schedule->tasks.size() == 0)
  {
    task.start_time = task.left_boundary_time;
    task.end_time = task.start_time + duration;
    current_state.current_schedule->tasks.insert({task.start_time, task});
    current_state.current_schedule->id_start_time[task.id] = task.start_time;
    return true;
  }
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule->tasks.end())
  {
    current = current_state.current_schedule->tasks.begin();
  }
  else
  {
    ++current;
  }
  DateTime start = task.left_boundary_time;
  while (current != current_state.current_schedule->tasks.end()
    && start + duration > current->second.start_time && start + duration <= task.right_boundary_time)
  {
    if (task.priority > current->second.priority && !current->second.is_protected)
    {
      current_state.current_schedule->unplanned_tasks.insert({current->second.id, current->second});
      goltsov::MapIterator< DateTime, Task > temp = current;
      ++current;
      current_state.current_schedule->id_start_time.erase(temp->second.id);
      current_state.current_schedule->tasks.erase(temp->first);
    }
    else
    {
      start = current->second.end_time;
      ++current;
    }
  }
  if (((current == current_state.current_schedule->tasks.end()) &&
    (start + duration <= task.right_boundary_time))
      || ((current != current_state.current_schedule->tasks.end())
      && (start + duration <= current->second.start_time && start + duration <= task.right_boundary_time)))
  {
    task.start_time = start;
    task.end_time = start + duration;
    current_state.current_schedule->tasks.insert({task.start_time, task});
    current_state.current_schedule->id_start_time[task.id] = task.start_time;
    return true;
  }
  else
  {
    return false;
  }
}
bool goltsov::detail::pushSoftTask(State& current_state, Task& task, const TimeInterval& duration)
{;
  if (current_state.current_schedule->tasks.empty())
  {
    task.start_time = task.left_boundary_time;
    task.end_time = task.start_time + duration;
    current_state.current_schedule->tasks.insert({task.start_time, task});
    current_state.current_schedule->id_start_time[task.id] = task.start_time;
    return true;
  }
  MapIterator<DateTime, Task> current = rfindByPredicate(*current_state.current_schedule, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule->tasks.end())
  {
    current = current_state.current_schedule->tasks.begin();
  }
  else
  {
    ++current;
  }
  DateTime potential_start = task.left_boundary_time;
  while (current != current_state.current_schedule->tasks.end()
    && potential_start + duration <= task.right_boundary_time)
  {
    if (potential_start + duration <= current->second.start_time)
    {
      break;
    }
    potential_start = current->second.end_time;
    ++current;
  }
  if (potential_start + duration <= task.right_boundary_time)
  {
    task.start_time = potential_start;
    task.end_time = potential_start + duration;
    current_state.current_schedule->tasks.insert({task.start_time, task});
    current_state.current_schedule->id_start_time[task.id] = task.start_time;
    return true;
  }
  return pushTask(current_state, task, duration);
}
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::pushProtectedTask(
  State& current_state, Task& task)
{
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule->tasks.end())
  {
    current = current_state.current_schedule->tasks.begin();
  }
  else
  {
    ++current;
  }
  while (current != current_state.current_schedule->tasks.end()
    && task.right_boundary_time > current->second.start_time)
  {
    if (current->second.is_protected)
    {
      current_state.current_schedule->unplanned_tasks.insert({task.id, task});
      return current;
    }
    else
    {
      current_state.current_schedule->unplanned_tasks.insert({current->second.id, current->second});
      goltsov::MapIterator< DateTime, Task > temp = current;
      ++current;
      current_state.current_schedule->id_start_time.erase(temp->second.id);
      current_state.current_schedule->tasks.erase(temp->first);
    }
  }
  task.start_time = task.left_boundary_time;
  task.end_time = task.right_boundary_time;
  current_state.current_schedule->tasks.insert({task.start_time, task});
  current_state.current_schedule->id_start_time[task.id] = task.start_time;
  return current_state.current_schedule->tasks.end();
}
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::pushProtectedTaskForce(
  State& current_state, Task& task)
{
  goltsov::MapIterator< DateTime, Task > current =
    rfindByPredicate(*current_state.current_schedule, FindTaskHasETLessLB{task});
  if (current == current_state.current_schedule->tasks.end())
  {
    current = current_state.current_schedule->tasks.begin();
  }
  else
  {
    ++current;
  }
  while (current != current_state.current_schedule->tasks.end()
    && task.right_boundary_time > current->second.start_time)
  {
    current_state.current_schedule->unplanned_tasks.insert({current->second.id, current->second});
    goltsov::MapIterator< DateTime, Task > temp = current;
    ++current;
    current_state.current_schedule->id_start_time.erase(temp->second.id);
    current_state.current_schedule->tasks.erase(temp->first);
  }
  task.start_time = task.left_boundary_time;
  task.end_time = task.right_boundary_time;
  current_state.current_schedule->tasks.insert({task.start_time, task});
  current_state.current_schedule->id_start_time[task.id] = task.start_time;
  return current_state.current_schedule->tasks.end();
}
void goltsov::detail::pushUnplanned(State& current_state)
{
  size_t count_pushed = 1;
  while (count_pushed)
  {
    count_pushed = 0;
    for (goltsov::MapIterator< std::string, Task > it =
      current_state.current_schedule->unplanned_tasks.begin();
      it != current_state.current_schedule->unplanned_tasks.end(); ++it)
    {
      if (pushSoftTask(current_state, it->second, it->second.end_time - it->second.start_time))
      {
        current_state.current_schedule->unplanned_tasks.erase(it->second.id);
        ++count_pushed;
      }
    }
  }
}
std::pair< size_t, size_t > goltsov::detail::mergeInterval(State& current_state,
  goltsov::MapIterator< DateTime, Task > s,
  goltsov::MapIterator< DateTime, Task > e)
{
  size_t count_planned0 = current_state.current_schedule->tasks.size();
  for (; s != e; ++s)
  {
    if (!s->second.is_protected)
    {
      pushSoftTask(current_state, s->second, s->second.end_time - s->second.start_time);
    }
    else
    {
      pushProtectedTask(current_state, s->second) == current_state.current_schedule->tasks.end();
    }
  }
  detail::pushUnplanned(current_state);
  size_t count_unplanned = current_state.current_schedule->tasks.size();
  size_t count_planned1 = current_state.current_schedule->tasks.size();
  return {count_planned1 - count_planned0, count_unplanned};
}
std::pair< goltsov::DateTime, goltsov::DateTime > goltsov::detail::findCommonGapInVector(
    State& current_state, const DateTime& start_time, const DateTime& end_time,
    const TimeInterval& duration, const goltsov::Vector< std::string >& names_schedules)
{
  goltsov::Vector< Task > all_tasks;
  for (size_t i = 0; i < names_schedules.getSize(); ++i)
  {
    goltsov::Schedule& current_schedule =
      current_state.current_context->schedules.at(names_schedules[i]);
    for (MapIterator< DateTime, Task > it = current_schedule.tasks.begin(); it != current_schedule.tasks.end(); ++it)
    {
      if (it->second.end_time > start_time && it->second.start_time < end_time)
      {
        all_tasks.pushBack(it->second);
      }
    }
  }
  for (size_t i = 0; i < all_tasks.getSize(); ++i)
  {
    for (size_t j = i + 1; j < all_tasks.getSize(); ++j)
    {
      if (all_tasks[j].start_time < all_tasks[i].start_time)
      {
        std::swap(all_tasks[i], all_tasks[j]);
      }
    }
  }
  DateTime current = start_time;
  for (size_t i = 0; i < all_tasks.getSize(); ++i)
  {
    Task& task = all_tasks[i];
    DateTime task_start = (task.start_time < start_time) ? start_time : task.start_time;
    DateTime task_end = (task.end_time > end_time) ? end_time : task.end_time;
    if (task_start > current)
    {
      if (task_start - current >= duration)
      {
        return {current, current + duration};
      }
    }
    if (task_end > current)
    {
      current = task_end;
    }
  }
  if (end_time > current)
  {
    if (end_time - current >= duration)
    {
      return {current, current + duration};
    }
  }
  throw std::runtime_error("<GAP DID NOT FOUND>");
}
template< class Predicate >
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::findByPredicate(Schedule& schedule,
  Predicate pred)
{
  for (MapIterator< DateTime, Task > it = schedule.tasks.begin(); it != schedule.tasks.end(); ++it)
  {
    if (pred(*it))
    {
      return it;
    }
  }
  return schedule.tasks.end();
}
template< class Predicate >
goltsov::MapIterator< goltsov::DateTime, goltsov::Task > goltsov::detail::rfindByPredicate(Schedule& schedule,
  Predicate pred)
{
  for (MapIterator< DateTime, Task > it = schedule.tasks.getLast(); it != schedule.tasks.begin(); --it)
  {
    if (pred(*it))
    {
      return it;
    }
  }
  if (pred(*schedule.tasks.begin()))
  {
    return schedule.tasks.begin();
  }
  return schedule.tasks.end();
}

void goltsov::parsingAdd(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  std::string title, description;
  DateTime left_boundary_time, right_boundary_time;
  TimeInterval duration;
  size_t priority;
  is >> id;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> title;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> description;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> left_boundary_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> right_boundary_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> duration;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> priority;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (right_boundary_time < left_boundary_time || right_boundary_time - left_boundary_time < duration)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  add(os, current_state, id, title, description, left_boundary_time, right_boundary_time, duration, priority);
}
void goltsov::parsingAddProtected(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  std::string title, description;
  DateTime start_time, end_time;
  is >> id;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> title;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> description;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> start_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> end_time;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (end_time < start_time)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  addProtected(os, current_state, id, title, description, start_time, end_time);
}
void goltsov::parsingRemove(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  remove(os, current_state, id);
}
void goltsov::parsingList(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  list(os, current_state);
}
void goltsov::parsingMerge(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  merge(os, current_state, schedule_name);
}
void goltsov::parsingShowUnplanned(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  showUnplanned(os, current_state);
}
void goltsov::parsingUnplannedRemove(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  unplannedRemove(os, current_state, id);
}
void goltsov::parsingUnplannedForce(std::istream& is, std::ostream& os, State& current_state)
{
  std::string id;
  is >> id;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  unplannedForce(os, current_state, id);
}
void goltsov::parsingMergeScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name, context_name;
  is >> schedule_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> context_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  mergeScheduleOtherContext(os, current_state, schedule_name, context_name);
}
void goltsov::parsingAddScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name, context_name;
  is >> schedule_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> context_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  addScheduleOtherContext(os, current_state, schedule_name, context_name);
}
void goltsov::parsingAddForceScheduleOtherContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name, context_name;
  is >> schedule_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> context_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  addForceScheduleOtherContext(os, current_state, schedule_name, context_name);
}
void goltsov::parsingSwitchSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  switchSchedule(os, current_state, schedule_name);
}
void goltsov::parsingSwitchContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string context_name;
  is >> context_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  switchContext(os, current_state, context_name);
}
void goltsov::parsingStats(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
  is >> start_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> end_time;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  stats(os, current_state, start_time, end_time);
}
void goltsov::parsingNewSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string name_schedule;
  is >> name_schedule;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  newSchedule(os, current_state, name_schedule);
}
void goltsov::parsingNewContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string name_context;
  is >> name_context;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  newContext(os, current_state, name_context);
}
void goltsov::parsingLoadSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name, file_name;
  is >> schedule_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> file_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  loadSchedule(os, current_state, schedule_name, file_name);
}
void goltsov::parsingSaveSchedule(std::istream& is, std::ostream& os, State& current_state)
{
  std::string schedule_name, file_name;
  is >> schedule_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> file_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  saveSchedule(os, current_state, schedule_name, file_name);
}
void goltsov::parsingLoadContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string context_name, file_name;
  is >> context_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> file_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  loadContext(os, current_state, context_name, file_name);
}
void goltsov::parsingSaveContext(std::istream& is, std::ostream& os, State& current_state)
{
  std::string context_name, file_name;
  is >> context_name;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> file_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  saveContext(os, current_state, context_name, file_name);
}
void goltsov::parsingFindGap(std::istream& is, std::ostream& os, State& current_state)
{
  TimeInterval gap;
  is >> gap;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  findGap(os, current_state, gap);
}
void goltsov::parsingFindGapOnInterval(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
  TimeInterval gap;
  is >> start_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> end_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> gap;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  findGapOnInterval(os, current_state, start_time, end_time, gap);
}
void goltsov::parsingFindCommonGap(std::istream& is, std::ostream& os, State& current_state)
{
  TimeInterval gap;
  size_t count;
  goltsov::Vector< std::string > names_schedules;
  is >> gap;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> count;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  for (size_t i = 0; i + 1 < count; ++i)
  {
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    names_schedules.pushBack(schedule_name);
  }
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  names_schedules.pushBack(schedule_name);
  findCommonGap(os, current_state, gap, count, names_schedules);
}
void goltsov::parsingFindCommonGapOnInterval(std::istream& is, std::ostream& os, State& current_state)
{
  DateTime start_time, end_time;
  TimeInterval gap;
  size_t count;
  goltsov::Vector< std::string > names_schedules;
  is >> start_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> end_time;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> gap;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is >> count;
  if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  for (size_t i = 0; i + 1 < count; ++i)
  {
    std::string schedule_name;
    is >> schedule_name;
    if (is.fail() || detail::isEndOfLine(detail::skipSpaces(is)))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    names_schedules.pushBack(schedule_name);
  }
  std::string schedule_name;
  is >> schedule_name;
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  names_schedules.pushBack(schedule_name);
  findCommonGapOnInterval(os, current_state, start_time, end_time, gap, count, names_schedules);
}
void goltsov::parsingExit(std::istream& is, std::ostream&, State&)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  is.setstate(std::ios_base::eofbit);
}
void goltsov::parsingListSchedules(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  listSchedules(os, current_state);
}
void goltsov::parsingListContexts(std::istream& is, std::ostream& os, State& current_state)
{
  if (is.fail() || !detail::isEndOfLine(detail::skipSpaces(is)))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  listContexts(os, current_state);
}
void goltsov::add(std::ostream& os, State& current_state, const std::string& id, const std::string& title,
  const std::string& description, const DateTime& left_boundary_time,
  const DateTime& right_boundary_time, const TimeInterval& duration, const size_t& priority)
{
  Task temp{id, title, description, left_boundary_time, right_boundary_time, left_boundary_time,
    left_boundary_time + duration, priority, false};
  bool is_planned = detail::pushSoftTask(current_state, temp, duration);
  if (is_planned)
  {
    DateTime k = current_state.current_schedule->id_start_time[id];
    os << "<TASK ADDED: Scheduled at " << current_state.current_schedule->tasks.at(k).start_time
      << ' ' << current_state.current_schedule->tasks.at(k).end_time << ">\n";
  }
  else
  {
    current_state.current_schedule->unplanned_tasks.insert({temp.id, temp});
    throw std::runtime_error("<TASK DID NOT ADDED>");
  }
  detail::pushUnplanned(current_state);
}
void goltsov::addProtected(std::ostream& os, State& current_state, const std::string& id, const std::string& title,
  const std::string& description, const DateTime& left_boundary_time,
  const DateTime& right_boundary_time)
{
  Task task{id, title, description, left_boundary_time, right_boundary_time, left_boundary_time,
    right_boundary_time, 0, true};
  MapIterator< DateTime, Task > res = detail::pushProtectedTask(current_state, task);
  if (res == current_state.current_schedule->tasks.end())
  {
    os << "<TASK ADDED: Scheduled at " << left_boundary_time << ' ' << right_boundary_time << ">\n";
  }
  else
  {
    std::ostringstream oss;
    oss << "<TASK DID NOT ADDED: Allready scheduled " << (* res).second.title << " at " << (* res).second.start_time
      << ' ' << (* res).second.end_time << ">";
    throw std::runtime_error(oss.str());
  }
  detail::pushUnplanned(current_state);
}
void goltsov::remove(std::ostream& os, State& current_state, const std::string& id)
{
  try
  {
    DateTime k = current_state.current_schedule->id_start_time.at(id);
    current_state.current_schedule->tasks.erase(k);
    os << "<TASK REMOVED: ID " << id << ">\n";
  }
  catch (...)
  {
    throw std::runtime_error("<NO TASK IN SCHEDULE>");
  }
}
void goltsov::list(std::ostream& os, State& current_state)
{
  MapIterator< DateTime, Task > it = current_state.current_schedule->tasks.begin();
  if (it != current_state.current_schedule->tasks.end())
  {
    os << (*it).second;
    ++it;
  }
  for (; it != current_state.current_schedule->tasks.end(); ++it)
  {
    os << '\n' << (*it).second;
  }
  os << '\n';
}
void goltsov::merge(std::ostream& os, State& current_state, const std::string& name_other_schedule)
{
  goltsov::Schedule* other_schedule;
  try
  {
    other_schedule = &(current_state.current_context->schedules.at(name_other_schedule));
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  std::pair< size_t, size_t > res = detail::mergeInterval(current_state, other_schedule->tasks.begin(),
    other_schedule->tasks.end());
  os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
}
void goltsov::showUnplanned(std::ostream& os, State& current_state)
{
  MapIterator< std::string, Task > it = current_state.current_schedule->unplanned_tasks.begin();
  if (it != current_state.current_schedule->unplanned_tasks.end())
  {
    Task& a = it->second;
    os << "<UNPLANNED: id=" << a.id << " Task=\"" << a.title << "\", left_boundary_time=" << a.left_boundary_time
      << ", right_boundary_time=" << a.right_boundary_time << ", duration="
      << a.end_time - a.start_time << ", priority=" << a.priority << ", is_protected="
      << a.is_protected << ">";
    ++it;
  }
  for (; it != current_state.current_schedule->unplanned_tasks.end(); ++it)
  {
    Task& a = it->second;
    os << "\n<UNPLANNED: id=" << a.id << " Task=\"" << a.title << "\", left_boundary_time=" << a.left_boundary_time
      << ", right_boundary_time=" << a.right_boundary_time << ", duration="
      << a.end_time - a.start_time << ", priority=" << a.priority << ", is_protected="
      << a.is_protected << ">";
  }
  os << '\n';
}
void goltsov::unplannedRemove(std::ostream& os, State& current_state, const std::string& id)
{
  try
  {
    current_state.current_schedule->unplanned_tasks.erase(id);
    os << "<TASK " << id << " REMOVED>\n";
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
}
void goltsov::unplannedForce(std::ostream& os, State& current_state, const std::string& id)
{
  Task task;
  try
  {
    task = current_state.current_schedule->unplanned_tasks.at(id);
    current_state.current_schedule->unplanned_tasks.erase(id);
    size_t now_size_unplanned = current_state.current_schedule->unplanned_tasks.size();
    MapIterator< DateTime, Task > res;
    if (task.is_protected)
    {
      res = detail::pushProtectedTaskForce(current_state, task);
    }
    else
    {
      res = detail::pushProtectedTask(current_state, task);
    }
    if (res == current_state.current_schedule->tasks.end())
    {
      os << "<FORCED ADD: " << task.title << ", NEW UNPLANNED: "
        << current_state.current_schedule->unplanned_tasks.size() - now_size_unplanned << ">\n";
    }
    else
    {
      os << "<FORCED ADD FAILED: " << res->second.title << " allready scheduled at "
        << res->second.start_time << ' ' << res->second.end_time << ">\n";
    }
  }
  catch (...)
  {
    throw std::runtime_error("<NO TASK>");
  }
}
void goltsov::mergeScheduleOtherContext(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& context_name)
{
  goltsov::Context context;
  goltsov::Schedule schedule;
  try
  {
    context = current_state.contexts.at(context_name);
  }
  catch (...)
  {
    throw std::runtime_error("<NO CONTEXT>");
  }
  try
  {
    schedule = context.schedules.at(schedule_name);
  }
  catch (...)
  {
    throw std::runtime_error("<NO SCHEDULE>");
  }
  std::pair< size_t, size_t > res = detail::mergeInterval(
    current_state, schedule.tasks.begin(), schedule.tasks.end());
  os << "<MERGE DONE. Added: " << res.first << ", Conflicts: " << res.second << ">\n";
}
void goltsov::addScheduleOtherContext(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& context_name)
{
  goltsov::Context context;
  goltsov::Schedule schedule;
  try
  {
    context = current_state.contexts.at(context_name);
  }
  catch (...)
  {
    throw std::runtime_error("<NO CONTEXT>");
  }
  try
  {
    schedule = context.schedules.at(schedule_name);
  }
  catch (...)
  {
    throw std::runtime_error("<NO SCHEDULE>");
  }
  try
  {
    current_state.current_context->schedules.at(schedule_name);
    throw std::runtime_error("<SCHEDULE ALLREADY EXISTS>");
  }
  catch (...)
  {
    current_state.current_context->schedules.insert({schedule_name, schedule});
    os << "<ADD DONE. Added: " << schedule.tasks.size() << ", Conflicts: "
      << schedule.unplanned_tasks.size() << ">\n";
  }
}
void goltsov::addForceScheduleOtherContext(std::ostream& os, State& current_state,
  const std::string& schedule_name, const std::string& context_name)
{
  bool is_current_schedule = false;
  try
  {
    current_state.current_context->schedules.at(schedule_name);
    if (current_state.current_context->name_context != context_name)
    {
      is_current_schedule = current_state.current_schedule->name_schedule == schedule_name;
      current_state.current_context->schedules.erase(schedule_name);
    }
  }
  catch (...)
  {}
  addScheduleOtherContext(os, current_state, schedule_name, context_name);
  if (is_current_schedule)
  {
    current_state.current_schedule = &current_state.current_context->schedules.at(schedule_name);
  }
}
void goltsov::switchSchedule(std::ostream& os, State& current_state, const std::string& schedule_name)
{
  try
  {
    current_state.current_schedule = &current_state.current_context->schedules.at(schedule_name);
    os << "<SCHEDULE SWITCHED>\n";
  }
  catch (...)
  {
    throw std::runtime_error("<NO SCHEDULE>");
  }
}
void goltsov::switchContext(std::ostream& os, State& current_state, const std::string& context_name)
{
  try
  {
    current_state.current_context = & current_state.contexts.at(context_name);
    current_state.current_schedule = & current_state.current_context->schedules.begin()->second;
    os << "<CONTEXT SWITCHED>\n";
  }
  catch (...)
  {
    throw std::runtime_error("<NO CONTEXT>");
  }
}
void goltsov::stats(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time)
{
  Task a;
  a.start_time = start_time;
  goltsov::MapIterator< DateTime, Task > current =
    detail::rfindByPredicate(*current_state.current_schedule, detail::FindTaskHasETLessLB{a});
  TimeInterval busy_time {0, 0, 0, 0, 0, 0};
  size_t count_tasks = 0;
  try
  {
    for (; current != current_state.current_schedule->tasks.end()
      && current->second.start_time < end_time; ++current)
    {
      busy_time = busy_time + (std::max(end_time, current->second.end_time) - current->second.start_time);
      count_tasks += 1;
    }
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
    return;
  }
  os << "<Tasks: " << count_tasks << ", Load: ";
  os << std::fixed << std::setprecision(2) << (busy_time / (end_time - start_time)) * 100 << "%>\n";
}
void goltsov::newSchedule(std::ostream&, State& current_state, const std::string& name_schedule)
{
  current_state.current_context->schedules.insert({name_schedule, goltsov::Schedule {}});
  current_state.current_context->schedules.at(name_schedule).name_schedule = name_schedule;
}
void goltsov::newContext(std::ostream&, State& current_state, const std::string& name_context)
{
  current_state.contexts.insert({name_context, goltsov::Context {}});
  current_state.contexts.at(name_context).name_context = name_context;
  current_state.contexts.at(name_context).schedules.insert({"Base_schedule", goltsov::Schedule {}});
  current_state.contexts.at(name_context).schedules.at("Base_schedule").name_schedule = "Base_schedule";
}
void goltsov::loadSchedule(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& filename)
{
  try
  {
    current_state.current_context->schedules.at(schedule_name);
    throw std::runtime_error("<NAME IS OCCUPIED>");
  }
  catch (...)
  {
    goltsov::Schedule schedule;
    schedule.name_schedule = schedule_name;
    std::fstream inp_f (filename);
    if (!inp_f)
    {
      throw std::runtime_error("<BAD FILE>");
    }
    inp_f >> schedule;
    current_state.current_context->schedules.insert({schedule_name, schedule});
    current_state.current_schedule = &current_state.current_context->schedules.at(schedule_name);
    os << "<SCHEDULE LOADED>\n";
  }
}
void goltsov::saveSchedule(std::ostream& os, State& current_state, const std::string& schedule_name,
  const std::string& filename)
{
  try
  {
    goltsov::Schedule schedule = current_state.current_context->schedules.at(schedule_name);
    std::fstream out_f(filename, std::ios::out | std::ios::trunc);
    if (!out_f.is_open())
    {
      throw std::runtime_error("<BAD FILE>");
    }
    out_f << schedule << '\n';
    os << "<SCHEDULE SAVED>\n";
  }
  catch (...)
  {
    throw std::runtime_error("<NO SUCH SCHEDULE>");
  }
}
void goltsov::loadContext(std::ostream& os, State& current_state, const std::string& context_name,
  const std::string& filename)
{
  try
  {
    goltsov::Context context = current_state.contexts.at(context_name);
    throw std::runtime_error("<NAME IS OCCUPIED>");
  }
  catch (...)
  {
    goltsov::Context context;
    context.name_context = context_name;
    std::fstream inp_f(filename);
    if (!inp_f)
    {
      throw std::runtime_error("<BAD FILE>");
    }
    inp_f >> context;
    if (context.schedules.empty())
    {
      context.schedules.insert({"Base schedule", goltsov::Schedule{}});
    }
    current_state.contexts.insert({context_name, context});
    current_state.contexts.at(context_name).name_context = context_name;
    current_state.current_context = &current_state.contexts.at(context_name);
    current_state.current_schedule = &current_state.current_context->schedules.begin()->second;
    os << "<CONTEXT LOADED>\n";
  }
}
void goltsov::saveContext(std::ostream& os, State& current_state, const std::string& context_name,
  const std::string& filename)
{
  try
  {
    goltsov::Context context = current_state.contexts.at(context_name);
    std::fstream out_f(filename, std::ios::out | std::ios::trunc);
    if (!out_f.is_open())
    {
      throw std::runtime_error("<BAD FILE>");
    }
    out_f << context << '\n';
    os << "<CONTEXT SAVED>\n";
  }
  catch (...)
  {
    throw std::runtime_error("<NO SUCH CONTEXT>");
  }
}
void goltsov::findGap(std::ostream& os, State& current_state, const TimeInterval& interval)
{
  try
  {
    goltsov::Vector< std::string > a;
    a.pushBack(current_state.current_schedule->name_schedule);
    auto last = current_state.current_schedule->tasks.getLast();
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state,
      current_state.current_time, ((last != current_state.current_schedule->tasks.end()) ? last->second.end_time :
      current_state.current_time) + interval, interval, a);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::findGapOnInterval(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time, const TimeInterval& interval)
{
  try
  {
    goltsov::Vector< std::string > a;
    a.pushBack(current_state.current_schedule->name_schedule);
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state, start_time,
      end_time, interval, a);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::findCommonGap(std::ostream& os, State& current_state, const TimeInterval& interval,
  const size_t&, const goltsov::Vector< std::string >& names_schedules)
{
  if (!current_state.current_schedule)
  {
    throw std::runtime_error("<NO SCHEDULE>");
  }
  auto last = current_state.current_schedule->tasks.getLast();
  for (auto it = current_state.current_context->schedules.begin();
    it != current_state.current_context->schedules.end(); ++it)
  {
    if (it->second.tasks.getLast() != it->second.tasks.end())
    {
      if (last->second.end_time < it->second.tasks.getLast()->second.end_time)
      {
        last = it->second.tasks.getLast();
      }
    }
  }
  std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state,
    current_state.current_time, ((last != current_state.current_schedule->tasks.end()) ? last->second.end_time :
    current_state.current_time) + interval, interval, names_schedules);
  os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
}
void goltsov::findCommonGapOnInterval(std::ostream& os, State& current_state, const DateTime& start_time,
  const DateTime& end_time, const TimeInterval& interval, const size_t&,
  const goltsov::Vector< std::string >& names_schedules)
{
  try
  {
    std::pair< DateTime, DateTime > gap = detail::findCommonGapInVector(current_state, start_time,
      end_time, interval, names_schedules);
    os << "<GAP FOUND: " << gap.first << ' ' << gap.second << ">\n";
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what();
  }
}
void goltsov::listSchedules(std::ostream& os, State& current_state)
{
  for (MapIterator< std::string, goltsov::Schedule > it = current_state.current_context->schedules.begin();
    it != current_state.current_context->schedules.end(); ++it)
  {
    if (current_state.current_schedule->name_schedule == it->second.name_schedule)
    {
      os << "* " << it->second.name_schedule << '\n';
    }
    else
    {
      os << "  " << it->second.name_schedule << '\n';
    }
  }
}
void goltsov::listContexts(std::ostream& os, State& current_state)
{
  for (MapIterator< std::string, goltsov::Context > it = current_state.contexts.begin();
    it != current_state.contexts.end(); ++it)
  {
    if (current_state.current_context->name_context == it->second.name_context)
    {
      os << "* " << it->second.name_context << '\n';
    }
    else
    {
      os << "  " << it->second.name_context << '\n';
    }
  }
}
