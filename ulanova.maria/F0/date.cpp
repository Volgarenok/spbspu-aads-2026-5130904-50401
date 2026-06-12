#include "date.hpp"

#include <ctime>
#include <stdexcept>

ulanova::Date ulanova::parse_date(const std::string& text)
{
  if ((text.size() != 10) || (text[2] != '.') || (text[5] != '.'))
  {
    throw std::logic_error("wrong date");
  }

  const int day = std::stoi(text.substr(0, 2));
  const int month = std::stoi(text.substr(3, 2));
  const int year = std::stoi(text.substr(6, 4));

  if ((day < 1) || (day > 31) || (month < 1) || (month > 12))
  {
    throw std::logic_error("wrong date");
  }

  std::tm time{};
  time.tm_mday = day;
  time.tm_mon = month - 1; // месяцы считаются с нуля
  time.tm_year = year - 1900; // год хранится как кол-во лет после 1900
  time.tm_hour = 12;
  time.tm_isdst = -1; // поле которое библ решает сама (летнее вреям или нет)

  const std::time_t raw_time = std::mktime(&time);

  if (raw_time == static_cast< std::time_t >(-1))
  {
    throw std::logic_error("wrong date");
  }

  return std::chrono::system_clock::from_time_t(raw_time);
}

bool ulanova::is_before_or_equal(const Date& lhs, const Date& rhs)
{
  return lhs <= rhs;
}
