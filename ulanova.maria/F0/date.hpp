#ifndef DATE_HPP
#define DATE_HPP

#include <chrono>
#include <string>

namespace ulanova
{
  using Date = std::chrono::system_clock::time_point;

  Date parse_date(const std::string& text);
  bool is_before_or_equal(const Date& lhs, const Date& rhs);
  bool is_after_or_equal(const Date& lhs, const Date& rhs);
}


#endif
