#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>

namespace sedov
{
  std::string pad(int val);
  int parseInt(const std::string & s, size_t pos, size_t len);
  bool isValidDate(int year, int month, int day);
  bool parseDate(const std::string & date, int & year, int & month, int & day);
  bool parseTime(const std::string & time, int & hours, int & minutes);
  std::string formatTime(int totalMinutes);
}

#endif
