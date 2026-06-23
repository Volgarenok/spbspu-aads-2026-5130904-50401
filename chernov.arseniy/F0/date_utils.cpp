#include "date_utils.hpp"
#include <string>

namespace chernov {
  namespace detail {
    bool isLeapYear(int year)
    {
      return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    bool isValidDayMonth(int day, int month, int year)
    {
      if (month < 1 || month > 12) {
        return false;
      }
      if (day < 1) {
        return false;
      }
      static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
      int maxDay = daysInMonth[month - 1];
      if (month == 2 && isLeapYear(year)) {
        maxDay = 29;
      }
      return day <= maxDay;
    }
  }

  bool isValidDate(const std::string & dateStr)
  {
    if (dateStr.empty() || dateStr == "-") {
      return true;
    }
    if (dateStr.size() != 10) {
      return false;
    }
    if (dateStr[2] != '.' || dateStr[5] != '.') {
      return false;
    }
    for (int i = 0; i < 10; ++i) {
      if (i == 2 || i == 5) {
        continue;
      }
      if (dateStr[i] < '0' || dateStr[i] > '9') {
        return false;
      }
    }
    int day = (dateStr[0] - '0') * 10 + (dateStr[1] - '0');
    int month = (dateStr[3] - '0') * 10 + (dateStr[4] - '0');
    int year = (dateStr[6] - '0') * 1000 + (dateStr[7] - '0') * 100 + (dateStr[8] - '0') * 10 + (dateStr[9] - '0');
    return detail::isValidDayMonth(day, month, year);
  }

  std::string toStorageFormat(const std::string & dateStr)
  {
    if (dateStr.empty() || dateStr == "-") {
      return "";
    }
    std::string storage;
    storage += dateStr[6];
    storage += dateStr[7];
    storage += dateStr[8];
    storage += dateStr[9];
    storage += '-';
    storage += dateStr[3];
    storage += dateStr[4];
    storage += '-';
    storage += dateStr[0];
    storage += dateStr[1];
    return storage;
  }

  std::string toDisplayFormat(const std::string & storage)
  {
    if (storage.empty()) {
      return "";
    }
    std::string display;
    display += storage[8];
    display += storage[9];
    display += '.';
    display += storage[5];
    display += storage[6];
    display += '.';
    display += storage[0];
    display += storage[1];
    display += storage[2];
    display += storage[3];
    return display;
  }
}
