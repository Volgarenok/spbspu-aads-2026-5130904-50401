#include "task.hpp"

namespace sedov
{
  Task::Task() noexcept:
    id_(0),
    active_(true)
  {}

  Task::Task(int id, const std::string & title, const std::string & date, const std::string & timeStart,
    const std::string & timeEnd, const std::string & imp, const std::string & scheduleName, bool active = true):
      id_(id),
      title_(title),
      date_(date),
      timeStart_(timeStart),
      timeEnd_(timeEnd),
      importance_(imp),
      scheduleName_(scheduleName),
      active_(active)
  {}

  int Task::getId() const noexcept
  {
    return id_;
  }

  const std::string & Task::getTitle() const noexcept
  {
    return title_;
  }

  const std::string & Task::getDescription() const noexcept
  {
    return description_;
  }

  const std::string & Task::getDate() const noexcept
  {
    return date_;
  }

  const std::string & Task::getTimeStart() const noexcept
  {
    return timeStart_;
  }

  const std::string & Task::getTimeEnd() const noexcept
  {
    return timeEnd_;
  }

  const std::string & Task::getImportance() const noexcept
  {
    return importance_;
  }

  const std::string & Task::getScheduleName() const noexcept
  {
    return scheduleName_;
  }

  bool Task::isActive() const noexcept
  {
    return active_;
  }

  void Task::setTitle(const std::string & title)
  {
    title_ = title;
  }

  void Task::setDescription(const std::string & desc)
  {
    description_ = desc;
  }

  void Task::setDate(const std::string & date)
  {
    date_ = date;
  }

  void Task::setTimeStart(const std::string & time)
  {
    timeStart_ = time;
  }

  void Task::setTimeEnd(const std::string & time)
  {
    timeEnd_ = time;
  }

  void Task::setImportance(const std::string & imp)
  {
    importance_ = imp;
  }

  void Task::setScheduleName(const std::string & name)
  {
    scheduleName_ = name;
  }

  void Task::setActive(bool active) noexcept
  {
    active_ = active;
  }

  int Task::getDurationMinutes() const noexcept
  {
    int sh, sm, eh, em;
    parseTime(timeStart_, sh, sm);
    parseTime(timeEnd_, eh, em);
    return (eh * 60 + em) - (sh * 60 + sm);
  }

  bool Task::overlapsWith(const Task & h) const noexcept
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

  int Task::getImportanceValue() const noexcept
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

  std::string Task::makeDatetimeKey() const
  {
    return date_ + " " + timeStart_;
  }

  void Task::validate() const
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

  ProfileKey::ProfileKey(const std::string & n):
    name_(n)
  {}

  ProfileKey::ProfileKey(const char * n):
    name_(n ? n : "")
  {}

  const std::string & ProfileKey::getName() const noexcept
  {
    return name_;
  }

  bool ProfileKey::operator<(const ProfileKey & h) const noexcept
  {
    return name_ < h.name_;
  }

  bool ProfileKey::operator==(const ProfileKey & h) const noexcept
  {
    return name_ == h.name_;
  }

  ScheduleKey::ScheduleKey(const std::string & n):
    name_(n)
  {}

  ScheduleKey::ScheduleKey(const char * n):
    name_(n ? n : "")
  {}

  const std::string & ScheduleKey::getName() const noexcept
  {
    return name_;
  }

  bool ScheduleKey::operator<(const ScheduleKey & h) const noexcept
  {
    return name_ < h.name_;
  }

  bool ScheduleKey::operator==(const ScheduleKey & h) const noexcept
  {
    return name_ == h.name_;
  }

  TaskIDKey::TaskIDKey() noexcept:
    id_(0)
  {}

  TaskIDKey::TaskIDKey(int i) noexcept:
    id_(i)
  {}

  int TaskIDKey::getId() const noexcept
  {
    return id_;
  }

  bool TaskIDKey::operator<(const TaskIDKey & h) const noexcept
  {
    return id_ < h.id_;
  }

  bool TaskIDKey::operator==(const TaskIDKey & h) const noexcept
  {
    return id_ == h.id_;
  }

  DateTimeKey::DateTimeKey(const std::string & dt):
    datetime_(dt)
  {}

  const std::string & DateTimeKey::getDatetime() const noexcept
  {
    return datetime_;
  }

  bool operator<(const DateTimeKey & a, const DateTimeKey & b) noexcept
  { 
    return a.getDatetime() < b.getDatetime(); 
  }

  HistoryEntry::HistoryEntry():
    taskId_(0)
  {}

  HistoryEntry::HistoryEntry(int id, const std::string & title, const std::string & date, const std::string & time):
    taskId_(id),
    title_(title),
    originalDate_(date)
  {
    std::time_t now = std::time(nullptr);
    std::tm * timeinfo = std::localtime(&now);
    completedAt_ = pad(timeinfo->tm_year + 1900) + "-" + pad(timeinfo->tm_mon + 1) + "-" +
      pad(timeinfo->tm_mday) + " " + pad(timeinfo->tm_hour) + ":" + pad(timeinfo->tm_min);
    (void)time;
  }

  int HistoryEntry::getTaskId() const noexcept
  {
    return taskId_;
  }

  const std::string & HistoryEntry::getTitle() const noexcept
  {
    return title_;
  }

  const std::string & HistoryEntry::getCompletedAt() const noexcept
  {
    return completedAt_;
  }

  const std::string & HistoryEntry::getOriginalDate() const noexcept
  {
    return originalDate_;
  }

  TimeWindow::TimeWindow() noexcept:
    startMinutes_(0),
    endMinutes_(0)
  {}

  TimeWindow::TimeWindow(const std::string & date, int startMin, int endMin) noexcept:
    date_(date),
    startMinutes_(startMin),
    endMinutes_(endMin)
  {}

  const std::string & TimeWindow::getDate() const noexcept
  {
    return date_;
  }

  int TimeWindow::getStartMinutes() const noexcept
  {
    return startMinutes_;
  }

  int TimeWindow::getEndMinutes() const noexcept
  {
    return endMinutes_;
  }

  int TimeWindow::getDurationMinutes() const noexcept
  {
    return endMinutes_ - startMinutes_;
  }

  std::string TimeWindow::format() const
  {
    return date_ + " " + formatTime(startMinutes_) + "-" + formatTime(endMinutes_) + " ("
      + std::to_string(getDurationMinutes()) + " min)";
  }
}
