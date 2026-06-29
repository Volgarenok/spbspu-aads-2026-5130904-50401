#include "structs.hpp"
#include <iostream>

std::istream& goltsov::detail::operator>>(std::istream& is, const Delimeter& a)
{
  char c;
  is.get(c);
  if (c != a.expected)
  {
    is.setstate(std::ios_base::failbit);
  }
  return is;
}
bool goltsov::detail::FindTaskHasETLessLB::operator()(const std::pair< DateTime, Task >& current)
{
  return (current.second.end_time <= a.left_boundary_time);
}
bool goltsov::detail::FindTaskHasEnoughET::operator()(const std::pair< DateTime, Task >& current)
{
  return (a.right_boundary_time >= current.second.end_time
    && a.right_boundary_time - current.second.end_time >= duration);
}
bool goltsov::detail::FindDateTime::operator()(const std::pair< goltsov::DateTime, std::pair < DateTime,
  DateTime > >& current)
{
  return current.second.first <= a && a <= current.second.second;
}
bool goltsov::detail::FindDateTime2::operator()(const std::pair< goltsov::DateTime, std::pair < DateTime,
  DateTime > >& current)
{
  return current.second.second < a;
}
bool goltsov::detail::FindDateTime3::operator()(const std::pair< DateTime, std::pair < DateTime,
  DateTime > >& current)
{
  return current.second.first > a;
}
bool goltsov::detail::isLeapYear(const size_t& year)
{
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
size_t goltsov::detail::getDaysInMonth(const size_t& year, const size_t& month)
{
  if (month == 2)
  {
    return isLeapYear(year) ? 29 : 28;
  }
  if (month == 4 || month == 6 || month == 9 || month == 11)
  {
    return 30;
  }
  return 31;
}
long long goltsov::detail::TimeIntervalToSeconds(const TimeInterval& interval)
{
  long long sec_in_min = 60;
  long long sec_in_hour = 60 * sec_in_min;
  long long sec_in_day = 24 * sec_in_hour;
  long long sec_ind_mounth = 30 * sec_in_day;
  long long sec_in_year = 365 * sec_in_day;
  long long total_seconds = 0;
  total_seconds += static_cast< long long >(interval.years) * sec_in_year;
  total_seconds += static_cast< long long >(interval.months) * sec_ind_mounth;
  total_seconds += static_cast< long long >(interval.days) * sec_in_day;
  total_seconds += static_cast< long long >(interval.hours) * sec_in_hour;
  total_seconds += static_cast< long long >(interval.minutes) * sec_in_min;
  total_seconds += static_cast< long long >(interval.seconds);
  return total_seconds;
}

std::istream& goltsov::operator>>(std::istream& is, TimeInterval& a)
{
  std::istream::sentry s (is);
  if (!s)
  {
    return is;
  }
  detail::IOGuard guard (is);
  using d_t = detail::Delimeter;
  size_t years, months, days;
  size_t hours, minutes, seconds;
  is >> years >> d_t{'-'} >> months >> d_t{'-'} >> days;
  is >> d_t{'_'};
  is >> hours >> d_t{':'} >> minutes >> d_t{':'} >> seconds;
  a.years = years;
  a.months = months;
  a.days = days;
  a.hours = hours;
  a.minutes = minutes;
  a.seconds = seconds;
  return is;
}
std::istream& goltsov::operator>>(std::istream& is, DateTime& a)
{
  std::istream::sentry s (is);
  if (!s)
  {
    return is;
  }
  detail::IOGuard guard (is);
  using d_t = detail::Delimeter;
  size_t year, month, day;
  size_t hour, minute, second;
  is >> year >> d_t{'-'} >> month >> d_t{'-'} >> day;
  is >> d_t{'_'};
  is >> hour >> d_t{':'} >> minute >> d_t{':'} >> second;
  a.year = year;
  a.month = month;
  a.day = day;
  a.hour = hour;
  a.minute = minute;
  a.second = second;
  return is;
}
std::istream& goltsov::operator>>(std::istream& is, Task& a)
{
  std::istream::sentry s (is);
  if (!s)
  {
    return is;
  }
  detail::IOGuard guard (is);
  using d_t = detail::Delimeter;
  std::string id;
  std::string title, description;
  DateTime left_boundary_time, right_boundary_time;
  DateTime start_time, end_time;
  size_t priority;
  bool is_protected;
  is >> id >> d_t{' '} >> title >> d_t{' '} >> description >> d_t{' '}  >> left_boundary_time
    >> d_t{' '} >> right_boundary_time >> d_t{' '} >> start_time >> d_t{' '} >> end_time
    >> d_t{' '} >> priority >> d_t{' '} >> is_protected;
  a.id = id;
  a.title = title;
  a.description = description;
  a.left_boundary_time = left_boundary_time;
  a.right_boundary_time = right_boundary_time;
  a.start_time = start_time;
  a.end_time = end_time;
  a.priority = priority;
  return is;
}
std::istream& goltsov::operator>>(std::istream& is, Schedule& a)
{
  size_t count;
  is >> count;
  for (size_t i = 0; i < count; ++i)
  {
    goltsov::Task task;
    is >> task;
    a.tasks.insert({task.start_time, task});
    a.id_start_time.insert({task.id, task.start_time});
  }
  return is;
}
std::istream& goltsov::operator>>(std::istream& is, Context& a)
{
  size_t count;
  is >> count;
  for (size_t i = 0; i < count; ++i)
  {
    goltsov::Schedule schedule;
    std::string name_schedule;
    is >> name_schedule >> schedule;
    a.schedules.insert({name_schedule, schedule});
    a.schedules.at(name_schedule).name_schedule = name_schedule;
  }
  return is;
}
std::ostream& goltsov::operator<<(std::ostream& os, const TimeInterval& a)
{
  os << a.years << '-' << a.months << '-' << a.days << '_' << a.hours << ':' << a.minutes << ':' << a.seconds;
  return os;
}
std::ostream& goltsov::operator<<(std::ostream& os, const DateTime& a)
{
  os << a.year << '-' << a.month << '-' << a.day << '_' << a.hour << ':' << a.minute << ':' << a.second;
  return os;
}
std::ostream& goltsov::operator<<(std::ostream& os, const Task& a)
{
  os << "(id: " << a.id << ")" << " (title: " << a.title << ")" << " (description: " << a.description << ")"
    << " (left_boundary_time: " << a.left_boundary_time << ")" << " (right_boundary_time: "
    << a.right_boundary_time << ")"<< " (start_time: " << a.start_time << ")" << " (end_time: " << a.end_time
    << ")" << " (priority: " << a.priority << ")" << " (is_protected: " << (a.is_protected ? 1 : 0) << ")";
  return os;
}
std::ostream& goltsov::operator<<(std::ostream& os, Schedule& a)
{
  os << a.tasks.size();
  for (MapIterator< goltsov::DateTime, goltsov::Task > it = a.tasks.begin(); it != a.tasks.end(); ++it)
  {
    os << '\n' << it->second.id << ' ' << it->second.title << ' ' << it->second.description << ' '
       << it->second.left_boundary_time << ' ' << it->second.right_boundary_time << ' '
        << it->second.start_time << ' ' << it->second.end_time << ' ' << it->second.priority << ' '
         << it->second.is_protected;
  }
  return os;
}
std::ostream& goltsov::operator<<(std::ostream& os, Context& a)
{
  os << a.schedules.size();
  for (MapIterator< std::string, goltsov::Schedule > it = a.schedules.begin(); it != a.schedules.end(); ++it)
  {
    os << '\n' << it->second.name_schedule << ' ' << it->second;
  }
  return os;
}
bool goltsov::operator<(const TimeInterval& lhs, const TimeInterval& rhs)
{
  if (lhs.years != rhs.years)
  {
    return lhs.years < rhs.years;
  }
  if (lhs.months != rhs.months)
  {
    return lhs.months < rhs.months;
  }
  if (lhs.days != rhs.days)
  {
    return lhs.days < rhs.days;
  }
  if (lhs.hours != rhs.hours)
  {
    return lhs.hours < rhs.hours;
  }
  if (lhs.minutes != rhs.minutes)
  {
    return lhs.minutes < rhs.minutes;
  }
  return lhs.seconds < rhs.seconds;
}
bool goltsov::operator>(const TimeInterval& lhs, const TimeInterval& rhs)
{
  return rhs < lhs;
}
bool goltsov::operator==(const TimeInterval& lhs, const TimeInterval& rhs)
{
  return !(lhs < rhs) && !(rhs < lhs);
}
bool goltsov::operator!=(const TimeInterval& lhs, const TimeInterval& rhs)
{
  return !(lhs == rhs);
}
bool goltsov::operator<=(const TimeInterval& lhs, const TimeInterval& rhs)
{
  return !(lhs > rhs);
}
bool goltsov::operator>=(const TimeInterval& lhs, const TimeInterval& rhs)
{
  return !(lhs < rhs);
}
bool goltsov::operator<(const DateTime& lhs, const DateTime& rhs)
{
  if (lhs.year != rhs.year)
  {
    return lhs.year < rhs.year;
  }
  if (lhs.month != rhs.month)
  {
    return lhs.month < rhs.month;
  }
  if (lhs.day != rhs.day)
  {
    return lhs.day < rhs.day;
  }
  if (lhs.hour != rhs.hour)
  {
    return lhs.hour < rhs.hour;
  }
  if (lhs.minute != rhs.minute)
  {
    return lhs.minute < rhs.minute;
  }
  return lhs.second < rhs.second;
}
bool goltsov::operator>(const DateTime& lhs, const DateTime& rhs)
{
  return rhs < lhs;
}
bool goltsov::operator==(const DateTime& lhs, const DateTime& rhs)
{
  return !(lhs < rhs) && ! (rhs < lhs);
}
bool goltsov::operator!=(const DateTime& lhs, const DateTime& rhs)
{
  return !(lhs == rhs);
}
bool goltsov::operator<=(const DateTime& lhs, const DateTime& rhs)
{
  return !(lhs > rhs);
}
bool goltsov::operator>=(const DateTime& lhs, const DateTime& rhs)
{
  return !(lhs < rhs);
}
goltsov::TimeInterval goltsov::operator-(const DateTime& lhs, const DateTime& rhs)
{
  if (lhs < rhs)
  {
    throw std::logic_error("The first operand must be greater than second");
  }
  TimeInterval result;
  int sec = static_cast< int >(lhs.second) - static_cast< int >(rhs.second);
  int borrow_min = 0;
  if (sec < 0)
  {
    sec += 60;
    borrow_min = 1;
  }
  result.seconds = static_cast< size_t >(sec);
  int min = static_cast< int >(lhs.minute) - static_cast< int >(rhs.minute) - borrow_min;
  int borrow_hour = 0;
  if (min < 0)
  {
    min += 60;
    borrow_hour = 1;
  }
  result.minutes = static_cast< size_t >(min);
  int hour = static_cast< int >(lhs.hour) - static_cast< int >(rhs.hour) - borrow_hour;
  int borrow_day = 0;
  if (hour < 0)
  {
    hour += 24;
    borrow_day = 1;
  }
  result.hours = static_cast< size_t >(hour);
  int day = static_cast< int >(lhs.day) - static_cast< int >(rhs.day) - borrow_day;
  int borrow_month = 0;
  if (day < 0)
  {
    size_t prev_month = lhs.month == 1 ? 12 : lhs.month - 1;
    size_t prev_year = lhs.month == 1 ? lhs.year - 1 : lhs.year;
    day += static_cast< int >(detail::getDaysInMonth(prev_year, prev_month));
    borrow_month = 1;
  }
  result.days = static_cast< size_t >(day);
  int month = static_cast< int >(lhs.month) - static_cast< int >(rhs.month) - borrow_month;
  int borrow_year = 0;
  if (month < 0)
  {
    month += 12;
    borrow_year = 1;
  }
  result.months = static_cast< size_t >(month);
  int year = static_cast< int >(lhs.year) - static_cast< int >(rhs.year) - borrow_year;
  result.years = static_cast< size_t >(year);
  return result;
}
goltsov::DateTime goltsov::operator+(const DateTime& lhs, const TimeInterval& rhs)
{
  DateTime result = lhs;
  int sec = static_cast< int >(result.second) + static_cast< int >(rhs.seconds);
  int carry_min = sec / 60;
  result.second = sec % 60;
  int min = static_cast< int >(result.minute) + static_cast< int >(rhs.minutes) + carry_min;
  int carry_hour = min / 60;
  result.minute = min % 60;
  int hour = static_cast< int >(result.hour) + static_cast< int >(rhs.hours) + carry_hour;
  int carry_day = hour / 24;
  result.hour = hour % 24;
  result.year += rhs.years;
  int total_months = static_cast< int >(result.month) + static_cast< int >(rhs.months);
  int carry_yearfrom_month = (total_months - 1) / 12;
  result.year += carry_yearfrom_month;
  result.month = (total_months - 1) % 12 + 1;
  int daysto_add = static_cast< int >(rhs.days) + carry_day;
  while (daysto_add > 0)
  {
    size_t daysin_current_month = detail::getDaysInMonth(result.year, result.month);
    int remaining_daysin_month = static_cast< int >(daysin_current_month) - static_cast< int >(result.day);
    if (daysto_add <= remaining_daysin_month)
    {
      result.day += daysto_add;
      daysto_add = 0;
    } else
    {
      daysto_add -= (remaining_daysin_month + 1);
      result.day = 1;
      result.month++;
      if (result.month > 12)
      {
        result.month = 1;
        result.year++;
      }
    }
  }
  return result;
}
goltsov::DateTime goltsov::operator-(const DateTime& lhs, const TimeInterval& rhs)
{
  DateTime result = lhs;
  int sec = static_cast< int >(result.second) - static_cast< int >(rhs.seconds);
  int borrow_min = 0;
  if (sec < 0)
  {
    sec += 60;
    borrow_min = 1;
  }
  result.second = static_cast< size_t >(sec);
  int min = static_cast< int >(result.minute) - static_cast< int >(rhs.minutes) - borrow_min;
  int borrow_hour = 0;
  if (min < 0)
  {
    min += 60;
    borrow_hour = 1;
  }
  result.minute = static_cast< size_t >(min);
  int hour = static_cast< int >(result.hour) - static_cast< int >(rhs.hours) - borrow_hour;
  int borrow_day = 0;
  if (hour < 0)
  {
    hour += 24;
    borrow_day = 1;
  }
  result.hour = static_cast< size_t >(hour);
  if (rhs.years > result.year)
  {
    throw std::logic_error("Resulting year would be negative");
  }
  result.year -= rhs.years;
  int total_months = static_cast< int >(result.month) - static_cast< int >(rhs.months);
  int borrow_yearfrom_month = 0;
  while (total_months <= 0) {
    total_months += 12;
    borrow_yearfrom_month++;
  }
  result.month = static_cast< size_t >(total_months);
  if (result.year < static_cast< size_t >(borrow_yearfrom_month))
  {
    throw std::logic_error("Resulting year would be negative");
  }
  result.year -= borrow_yearfrom_month;
  int daysto_subtract = static_cast< int >(rhs.days) + borrow_day;
  while (daysto_subtract > 0)
  {
    if (static_cast< int >(result.day) > daysto_subtract)
    {
      result.day -= daysto_subtract;
      daysto_subtract = 0;
    }
    else
    {
      daysto_subtract -= result.day;
      if (result.month == 1)
      {
        result.month = 12;
        if (result.year == 0)
        {
          throw std::logic_error("Resulting year would be negative");
        }
        result.year--;
      }
      else
      {
        result.month--;
      }
      result.day = detail::getDaysInMonth(result.year, result.month);
    }
  }
  return result;
}
goltsov::TimeInterval goltsov::operator+(const TimeInterval& lhs, const TimeInterval& rhs)
{
  TimeInterval result;
  long long sec = static_cast< long long >(lhs.seconds) + static_cast< long long >(rhs.seconds);
  long long carry_min = sec / 60;
  result.seconds = static_cast< size_t >(sec % 60);
  long long min = static_cast< long long >(lhs.minutes) + static_cast< long long >(rhs.minutes) + carry_min;
  long long carry_hour = min / 60;
  result.minutes = static_cast< size_t >(min % 60);
  long long hour = static_cast< long long >(lhs.hours) + static_cast< long long >(rhs.hours) + carry_hour;
  long long carry_day = hour / 24;
  result.hours = static_cast< size_t >(hour % 24);
  long long day = static_cast< long long >(lhs.days) + static_cast< long long >(rhs.days) + carry_day;
  long long carry_month = day / 30;
  result.days = static_cast< size_t >(day % 30);
  long long month = static_cast< long long >(lhs.months) + static_cast< long long >(rhs.months) + carry_month;
  long long carry_year = month / 12;
  result.months = static_cast< size_t >(month % 12);
  result.years = static_cast< size_t >(static_cast< long long >(lhs.years)
    + static_cast< long long >(rhs.years) + carry_year);
  return result;
}
goltsov::TimeInterval goltsov::operator-(const TimeInterval& lhs, const TimeInterval& rhs)
{
  TimeInterval result;
  long long sec = static_cast< long long >(lhs.seconds) - static_cast< long long >(rhs.seconds);
  long long borrow_min = 0;
  if (sec < 0)
  {
    sec += 60;
    borrow_min = 1;
  }
  result.seconds = static_cast< size_t >(sec);
  long long min = static_cast< long long >(lhs.minutes) - static_cast< long long >(rhs.minutes) - borrow_min;
  long long borrow_hour = 0;
  if (min < 0)
  {
    min += 60;
    borrow_hour = 1;
  }
  result.minutes = static_cast< size_t >(min);
  long long hour = static_cast< long long >(lhs.hours) - static_cast< long long >(rhs.hours) - borrow_hour;
  long long borrow_day = 0;
  if (hour < 0)
  {
    hour += 24;
    borrow_day = 1;
  }
  result.hours = static_cast< size_t >(hour);
  long long day = static_cast< long long >(lhs.days) - static_cast< long long >(rhs.days) - borrow_day;
  long long borrow_month = 0;
  if (day < 0)
  {
    day += 30;
    borrow_month = 1;
  }
  result.days = static_cast< size_t >(day);
  long long month = static_cast< long long >(lhs.months) - static_cast< long long >(rhs.months) - borrow_month;
  long long borrow_year = 0;
  if (month < 0)
  {
    month += 12;
    borrow_year = 1;
  }
  result.months = static_cast< size_t >(month);
  long long year = static_cast< long long >(lhs.years) - static_cast< long long >(rhs.years) - borrow_year;
  if (year < 0)
  {
    throw std::logic_error("Resulting interval would be negative");
  }
  result.years = static_cast< size_t >(year);
  return result;
}
double goltsov::operator/(const TimeInterval& lhs, const TimeInterval& rhs)
{
  long long lhs_seconds = detail::TimeIntervalToSeconds(lhs);
  long long rhs_seconds = detail::TimeIntervalToSeconds(rhs);
  if (rhs_seconds == 0)
  {
    throw std::invalid_argument("Division by zero interval");
  }
  return static_cast< double >(lhs_seconds) / static_cast< double >(rhs_seconds);
}
goltsov::State::State():
  current_schedule(nullptr),
  current_context(nullptr),
  contexts(goltsov::Map< std::string, Context >{}),
  current_time(goltsov::DateTime{})
{}
goltsov::State::State(Context* ctx, Schedule* sch, const Map<std::string, Context>& ctxs, const DateTime& time):
  current_schedule(sch),
  current_context(ctx),
  contexts(ctxs),
  current_time(time)
{}
