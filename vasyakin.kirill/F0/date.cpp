#include "date.hpp"

vasyakin::Date::Date() noexcept:
  day_(1),
  month_(1),
  year_(2026)
{}

vasyakin::Date::Date(size_t day, size_t month, size_t year):
  day_(day),
  month_(month), 
  year_(year)
{
  normalize();
}

size_t vasyakin::Date::getDay() const noexcept
{
  return day_;
}

size_t vasyakin::Date::getMonth() const noexcept
{
  return month_;
}

size_t vasyakin::Date::getYear() const noexcept
{
  return year_;
}

vasyakin::Date vasyakin::Date::operator+(size_t days) const noexcept
{
  Date result{day_, month_, year_};
  result.day_ += days;
  result.normalize();
  return result;
}

size_t vasyakin::Date::daysDiff(const Date& other) const noexcept
{
  auto toDays = [](const Date& date) -> size_t
  {
    size_t days = date.year_ * 365 + date.day_;
    for (size_t m = 1; m < date.month_; ++m)
    {
      days += getDaysInMonth(m, date.year_);
    }
    days += date.year_ / 4 - date.year_ / 100 + date.year_ / 400;
    return days;
  };

  return toDays(*this) - toDays(other);
}

bool vasyakin::Date::isValid() const noexcept
{
  return day_ >= 1 && day_ <= getDaysInMonth(month_, year_)
    && year_ >= 2020 && year_ <= 2100 && month_ >= 1 && month_ <= 12;
}

std::string vasyakin::Date::toString() const
{
  std::string result = std::to_string(year_) + "-";
  if (month_ < 10)
  {
    result += "0";
  }

  result += std::to_string(month_);
  result += "-";

  if (day_ < 10)
  {
    result += "0";
  }

  result += std::to_string(day_);
  return result;
}

vasyakin::Date vasyakin::Date::fromString(const std::string& str)
{
  size_t year = std::stoul(str.substr(0, 4));
  size_t month = std::stoul(str.substr(5, 2));
  size_t day = std::stoul(str.substr(8, 2));

  return Date(day, month, year);
}

bool vasyakin::Date::operator<(const Date& other) const noexcept
{
  if (year_ != other.year_)
  {
    return year_ < other.year_;
  }
  if (month_ != other.month_)
  {
    return month_ < other.month_;
  }

  return day_ < other.day_;
}

bool vasyakin::Date::operator>(const Date& other) const noexcept
{
  return other < *this;
}

bool vasyakin::Date::operator==(const Date& other) const noexcept
{
  return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool vasyakin::Date::operator!=(const Date& other) const noexcept
{
  return !(*this == other);
}

bool vasyakin::Date::operator<=(const Date& other) const noexcept
{
  return !(*this > other);
}

bool vasyakin::Date::operator>=(const Date& other) const noexcept
{
  return !(*this < other);
}

bool vasyakin::Date::isLeapYear(size_t year) noexcept
{
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

size_t vasyakin::Date::getDaysInMonth(size_t month, size_t year) noexcept
{
  if (month == 2 && isLeapYear(year))
  {
    return 29;
  }

  size_t days[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  return days[month];
}

void vasyakin::Date::normalize() noexcept
{
  while (day_ > getDaysInMonth(month_, year_))
  {
    day_ -= getDaysInMonth(month_, year_);
    month_++;

    if (month_ > 12)
    {
      month_ = 1;
      year_++;
    }
  }
}
