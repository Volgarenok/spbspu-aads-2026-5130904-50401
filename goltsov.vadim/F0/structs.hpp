#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <ios>
#include <string>
#include <myvector.hpp>
#include "mymap.hpp"

namespace goltsov
{
  struct TimeInterval;
  struct DateTime;
  struct Task;
  struct Schedule;
  struct Context;
  namespace detail
  {
    struct FindTaskHasETLessLB
    {
      const goltsov::Task& a;
      bool operator()(const std::pair< goltsov::DateTime, goltsov::Task >&);
    };
    struct FindTaskHasEnoughET
    {
      const goltsov::Task& a;
      const goltsov::TimeInterval& duration;
      bool operator()(const std::pair< goltsov::DateTime, goltsov::Task >&);
    };
    struct FindDateTime
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
    };
    struct FindDateTime2
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
    };
    struct FindDateTime3
    {
      const goltsov::DateTime& a;
      bool operator()(const std::pair< goltsov::DateTime, std::pair < goltsov::DateTime, goltsov::DateTime > >&);
    };
    struct Delimeter
    {
      char expected;
    };
    std::istream& operator>>(std::istream&, const Delimeter&);
    struct IOGuard
    {
      explicit IOGuard(std::basic_ios< char >& s):
        s_(s),
        precision_(s.precision()),
        width_(s.width()),
        flags_(s.flags()),
        fill_(s.fill())
      {}
      ~IOGuard()
      {
        s_.precision(precision_);
        s_.width(width_);
        s_.flags(flags_);
        s_.fill(fill_);
      }
    private:
      std::basic_ios< char >& s_;
      std::streamsize precision_;
      std::streamsize width_;
      std::basic_ios< char >::fmtflags flags_;
      char fill_;
    };
    bool isLeapYear(const size_t& year);
    size_t getDaysInMonth(const size_t& year, const size_t& month);
    long long TimeIntervalToSeconds(const TimeInterval& interval);
  }

  struct TimeInterval
  {
    size_t years, months, days;
    size_t hours, minutes, seconds;
  };
  TimeInterval operator-(const TimeInterval&, const TimeInterval&);
  TimeInterval operator+(const TimeInterval&, const TimeInterval&);
  double operator/(const TimeInterval&, const TimeInterval&);
  std::istream& operator>>(std::istream&, TimeInterval&);
  std::ostream& operator<<(std::ostream&, const TimeInterval&);
  bool operator<(const TimeInterval&, const TimeInterval&);
  bool operator>(const TimeInterval&, const TimeInterval&);
  bool operator==(const TimeInterval&, const TimeInterval&);
  bool operator!=(const TimeInterval&, const TimeInterval&);
  bool operator<=(const TimeInterval&, const TimeInterval&);
  bool operator>=(const TimeInterval&, const TimeInterval&);

  struct DateTime
  {
    size_t year, month, day;
    size_t hour, minute, second;
  };
  std::istream& operator>>(std::istream&, DateTime&);
  std::ostream& operator<<(std::ostream&, const DateTime&);
  bool operator<(const DateTime&, const DateTime&);
  bool operator>(const DateTime&, const DateTime&);
  bool operator==(const DateTime&, const DateTime&);
  bool operator!=(const DateTime&, const DateTime&);
  bool operator<=(const DateTime&, const DateTime&);
  bool operator>=(const DateTime&, const DateTime&);
  TimeInterval operator-(const DateTime&, const DateTime&);
  DateTime operator-(const DateTime&, const TimeInterval&);
  DateTime operator+(const DateTime&, const TimeInterval&);

  struct Task
  {
    std::string id;
    std::string title, description;
    DateTime left_boundary_time, right_boundary_time;
    DateTime start_time, end_time;
    size_t priority;
    bool is_protected;
  };
  std::istream& operator>>(std::istream&, Task&);
  std::ostream& operator<<(std::ostream&, const Task&);

  struct Schedule
  {
    std::string name_schedule;
    goltsov::Map< DateTime, Task > tasks;
    goltsov::Map< std::string, goltsov::Task > unplanned_tasks;
    goltsov::Map< std::string, goltsov::DateTime > id_start_time;
  };
  std::istream& operator>>(std::istream&, Schedule&);
  std::ostream& operator<<(std::ostream&, Schedule&);

  struct Context
  {
    std::string name_context;
    goltsov::Map< std::string, Schedule > schedules;
  };
  std::istream& operator>>(std::istream&, Context&);
  std::ostream& operator<<(std::ostream&, Context&);

  struct State
  {
    Schedule* current_schedule;
    Context* current_context;
    goltsov::Map< std::string, Context > contexts;
    goltsov::DateTime current_time;
    State();
    State(Context*, Schedule*, const Map<std::string, Context>&, const DateTime&);
  };
}

#endif
