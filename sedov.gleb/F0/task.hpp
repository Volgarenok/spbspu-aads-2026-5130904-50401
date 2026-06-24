#ifndef TASK_HPP
#define TASK_HPP
#include <string>
#include "utils.hpp"

namespace sedov
{
  class Task
  {
  public:
    Task() noexcept = default;
    Task(const Task &) = default;
    Task(Task &&) = default;
    Task & operator=(const Task &) = default;
    Task & operator=(Task &&) = default;
    ~Task() = default;
    explicit Task(int id, const std::string & title, const std::string & date, const std::string & timeStart,
      const std::string & timeEnd, const std::string & imp, const std::string & scheduleName, bool active = true);

    int getId() const noexcept;
    const std::string & getTitle() const noexcept;
    const std::string & getDescription() const noexcept;
    const std::string & getDate() const noexcept;
    const std::string & getTimeStart() const noexcept;
    const std::string & getTimeEnd() const noexcept;
    const std::string & getImportance() const noexcept;
    const std::string & getScheduleName() const noexcept;
    bool isActive() const noexcept;

    void setTitle(const std::string & title);
    void setDescription(const std::string & desc);
    void setDate(const std::string & date);
    void setTimeStart(const std::string & time);
    void setTimeEnd(const std::string & time);
    void setImportance(const std::string & imp);
    void setScheduleName(const std::string & name);
    void setActive(bool active) noexcept;

    int getDurationMinutes() const noexcept;
    bool overlapsWith(const Task & h) const noexcept;
    int getImportanceValue() const noexcept;
    std::string makeDatetimeKey() const;
    void validate() const;

  private:
    int id_;
    std::string title_;
    std::string description_;
    std::string date_;
    std::string timeStart_;
    std::string timeEnd_;
    std::string importance_;
    std::string scheduleName_;
    bool active_;
  };

  struct ProfileKey
  {
  public:
    ProfileKey() = default;
    ProfileKey(const ProfileKey &) = default;
    ProfileKey(ProfileKey &&) = default;
    ProfileKey & operator=(const ProfileKey &) = default;
    ProfileKey & operator=(ProfileKey &&) = default;
    ~ProfileKey() = default;
    explicit ProfileKey(const std::string & n);
    explicit ProfileKey(const char * n);
    const std::string & getName() const noexcept;
    bool operator<(const ProfileKey & h) const noexcept;
    bool operator==(const ProfileKey & h) const noexcept;
  private:
    std::string name_;
  };

  struct ScheduleKey
  {
  public:
    ScheduleKey() = default;
    ScheduleKey(const ScheduleKey &) = default;
    ScheduleKey(ScheduleKey &&) = default;
    ScheduleKey & operator=(const ScheduleKey &) = default;
    ScheduleKey & operator=(ScheduleKey &&) = default;
    ~ScheduleKey() = default;
    explicit ScheduleKey(const std::string & n);
    explicit ScheduleKey(const char * n);
    const std::string & getName() const noexcept;
    bool operator<(const ScheduleKey & h) const noexcept;
    bool operator==(const ScheduleKey & h) const noexcept;
  private:
    std::string name_;
  };

  struct TaskIDKey
  {
  public:
    TaskIDKey() noexcept = default;
    TaskIDKey(const TaskIDKey &) = default;
    TaskIDKey(TaskIDKey &&) = default;
    TaskIDKey & operator=(const TaskIDKey &) = default;
    TaskIDKey & operator=(TaskIDKey &&) = default;
    ~TaskIDKey() = default;
    explicit TaskIDKey(int i) noexcept;
    int getId() const noexcept;
    bool operator<(const TaskIDKey & h) const noexcept;
    bool operator==(const TaskIDKey & h) const noexcept;
  private:
    int id_;
  };

  struct DateTimeKey
  {
  public:
    DateTimeKey() = default;
    DateTimeKey(const DateTimeKey &) = default;
    DateTimeKey(DateTimeKey &&) = default;
    DateTimeKey & operator=(const DateTimeKey &) = default;
    DateTimeKey & operator=(DateTimeKey &&) = default;
    ~DateTimeKey() = default;
    explicit DateTimeKey(const std::string & dt);
    const std::string & getDatetime() const noexcept;
  private:
    std::string datetime_;
  };
  bool operator<(const DateTimeKey & a, const DateTimeKey & b) noexcept;

  class TimeWindow
  {
  public:
    TimeWindow() noexcept = default;
    TimeWindow(const TimeWindow &) = default;
    TimeWindow(TimeWindow &&) = default;
    TimeWindow & operator=(const TimeWindow &) = default;
    TimeWindow & operator=(TimeWindow &&) = default;
    ~TimeWindow() = default;
    TimeWindow(const std::string & date, int startMin, int endMin) noexcept;
    const std::string & getDate() const noexcept;
    int getStartMinutes() const noexcept;
    int getEndMinutes() const noexcept;
    int getDurationMinutes() const noexcept;
    std::string format() const;
  private:
    std::string date_;
    int startMinutes_;
    int endMinutes_;
  };
}

#endif
