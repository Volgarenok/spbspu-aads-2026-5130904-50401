#ifndef TASK_HPP
#define TASK_HPP
#include <string>

namespace sedov
{
  namespace detail
  {
    std::string pad(int val)
    {
      return (val < 10) ? "0" + std::to_string(val) : std::to_string(val);
    }

    int parseInt(const std::string & s, size_t pos, size_t len)
    {
      for (size_t i = pos; i < pos + len && i < s.length(); ++i)
      {
        if (s[i] < '0' || s[i] > '9')
        {
          return -1;
        }
      }
      try
      {
        return std::stoi(s.substr(pos, len));
      }
      catch (...)
      {
        return -1;
      }
    }

    bool isValidDate(int year, int month, int day)
    {
      if (month < 1 || month > 12)
      {
        return false;
      }
      if (day < 1)
      {
        return false;
      }
      static const int daysInMonth[] = {31,28,31,30,31,30,31,31,30,31,30,31};
      bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
      int maxDay = (month == 2 && isLeap) ? 29 : daysInMonth[month - 1];
      return day <= maxDay;
    }

    bool parseDate(const std::string & date, int & year, int & month, int & day)
    {
      if (date.length() != 10 || date[4] != '-' || date[7] != '-')
      {
        return false;
      }
      for (int i = 0; i < 10; ++i)
      {
        if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9'))
        {
          return false;
        }
      }
      year = parseInt(date, 0, 4);
      month = parseInt(date, 5, 2);
      day = parseInt(date, 8, 2);
      return (year >= 1000 && year <= 9999 && month > 0 && day > 0 && isValidDate(year, month, day));
    }

    bool parseTime(const std::string & time, int & hours, int & minutes)
    {
      if (time.length() != 5 || time[2] != ':')
      {
        return false;
      }
      hours = parseInt(time, 0, 2);
      minutes = parseInt(time, 3, 2);
      return (hours >= 0 && hours <= 23 && minutes >= 0 && minutes <= 59);
    }

    std::string formatTime(int totalMinutes)
    {
      return pad(totalMinutes / 60) + ":" + pad(totalMinutes % 60);
    }
  }
}

#endif
