#include "task.hpp"
#include <stdexcept>

sedov::Task::Task(int id, const std::string & title, const std::string & date, const std::string & timeStart,
  const std::string & timeEnd, const std::string & imp, const std::string & scheduleName, bool active):
    id_(id),
    title_(title),
    date_(date),
    timeStart_(timeStart),
    timeEnd_(timeEnd),
    importance_(imp),
    scheduleName_(scheduleName),
    active_(active)
{}

int sedov::Task::getId() const noexcept
{
  return id_;
}

const std::string & sedov::Task::getTitle() const noexcept
{
  return title_;
}

const std::string & sedov::Task::getDescription() const noexcept
{
  return description_;
}

const std::string & sedov::Task::getDate() const noexcept
{
  return date_;
}

const std::string & sedov::Task::getTimeStart() const noexcept
{
  return timeStart_;
}

const std::string & sedov::Task::getTimeEnd() const noexcept
{
  return timeEnd_;
}

const std::string & sedov::Task::getImportance() const noexcept
{
  return importance_;
}

const std::string & sedov::Task::getScheduleName() const noexcept
{
  return scheduleName_;
}

bool sedov::Task::isActive() const noexcept
{
  return active_;
}

void sedov::Task::setTitle(const std::string & title)
{
  title_ = title;
}

void sedov::Task::setDescription(const std::string & desc)
{
  description_ = desc;
}

void sedov::Task::setDate(const std::string & date)
{
  date_ = date;
}

void sedov::Task::setTimeStart(const std::string & time)
{
  timeStart_ = time;
}

void sedov::Task::setTimeEnd(const std::string & time)
{
  timeEnd_ = time;
}

void sedov::Task::setImportance(const std::string & imp)
{
  importance_ = imp;
}

void sedov::Task::setScheduleName(const std::string & name)
{
  scheduleName_ = name;
}

void sedov::Task::setActive(bool active) noexcept
{
  active_ = active;
}

int sedov::Task::getDurationMinutes() const noexcept
{
  int sh, sm, eh, em;
  parseTime(timeStart_, sh, sm);
  parseTime(timeEnd_, eh, em);
  return (eh * 60 + em) - (sh * 60 + sm);
}

bool sedov::Task::overlapsWith(const Task & h) const noexcept
{
  if (date_ != h.date_)
  {
    return false;
  }
  int s1h, s1m, e1h, e1m, s2h, s2m, e2h, e2m;
  parseTime(timeStart_, s1h, s1m);
  parseTime(timeEnd_, e1h, e1m);
  parseTime(h.timeStart_, s2h, s2m);
  parseTime(h.timeEnd_, e2h, e2m);
  int start1 = s1h * 60 + s1m;
  int end1 = e1h * 60 + e1m;
  int start2 = s2h * 60 + s2m;
  int end2 = e2h * 60 + e2m;
  return !(end1 <= start2 || end2 <= start1);
}

int sedov::Task::getImportanceValue() const noexcept
{
  if (importance_ == "high")
  {
    return 3;
  }
  if (importance_ == "mid")
  {
    return 2;
  }
  if (importance_ == "low")
  {
    return 1;
  }
  return 2;
}

std::string sedov::Task::makeDatetimeKey() const
{
  return date_ + " " + timeStart_;
}

void sedov::Task::validate() const
{
  int y, m, d;
  if (!parseDate(date_, y, m, d))
  {
    throw std::invalid_argument("Invalid date: " + date_);
  }
  int sh, sm, eh, em;
  if (!parseTime(timeStart_, sh, sm) || !parseTime(timeEnd_, eh, em))
  {
    throw std::invalid_argument("Invalid time format");
  }
  int startMin = sh * 60 + sm;
  int endMin = eh * 60 + em;
  if (startMin >= endMin)
  {
    throw std::invalid_argument("Task duration must be positive");
  }
  if (importance_ != "low" && importance_ != "mid" && importance_ != "high")
  {
    throw std::invalid_argument("Invalid importance: " + importance_);
  }
}

sedov::ProfileKey::ProfileKey(const std::string & n):
  name_(n)
{}

sedov::ProfileKey::ProfileKey(const char * n):
  name_(n ? n : "")
{}

const std::string & sedov::ProfileKey::getName() const noexcept
{
  return name_;
}

bool sedov::ProfileKey::operator<(const ProfileKey & h) const noexcept
{
  return name_ < h.name_;
}

bool sedov::ProfileKey::operator==(const ProfileKey & h) const noexcept
{
  return name_ == h.name_;
}

sedov::ScheduleKey::ScheduleKey(const std::string & n):
  name_(n)
{}

sedov::ScheduleKey::ScheduleKey(const char * n):
  name_(n ? n : "")
{}

const std::string & sedov::ScheduleKey::getName() const noexcept
{
  return name_;
}

bool sedov::ScheduleKey::operator<(const ScheduleKey & h) const noexcept
{
  return name_ < h.name_;
}

bool sedov::ScheduleKey::operator==(const ScheduleKey & h) const noexcept
{
  return name_ == h.name_;
}

sedov::TaskIDKey::TaskIDKey(int i) noexcept:
  id_(i)
{}

int sedov::TaskIDKey::getId() const noexcept
{
  return id_;
}

bool sedov::TaskIDKey::operator<(const TaskIDKey & h) const noexcept
{
  return id_ < h.id_;
}

bool sedov::TaskIDKey::operator==(const TaskIDKey & h) const noexcept
{
  return id_ == h.id_;
}

sedov::DateTimeKey::DateTimeKey(const std::string & dt):
  datetime_(dt)
{}

const std::string & sedov::DateTimeKey::getDatetime() const noexcept
{
  return datetime_;
}

bool sedov::operator<(const DateTimeKey & a, const DateTimeKey & b) noexcept
{
  return a.getDatetime() < b.getDatetime();
}

sedov::TimeWindow::TimeWindow(const std::string & date, int startMin, int endMin) noexcept:
  date_(date),
  startMinutes_(startMin),
  endMinutes_(endMin)
{}

const std::string & sedov::TimeWindow::getDate() const noexcept
{
  return date_;
}

int sedov::TimeWindow::getStartMinutes() const noexcept
{
  return startMinutes_;
}

int sedov::TimeWindow::getEndMinutes() const noexcept
{
  return endMinutes_;
}

int sedov::TimeWindow::getDurationMinutes() const noexcept
{
  return endMinutes_ - startMinutes_;
}

std::string sedov::TimeWindow::format() const
{
  return date_ + " " + formatTime(startMinutes_) + "-" + formatTime(endMinutes_) + " ("
    + std::to_string(getDurationMinutes()) + " min)";
}
