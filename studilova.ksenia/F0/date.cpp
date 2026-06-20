#include "date.hpp"

#include <iomanip>
#include <iostream>
#include <stdexcept>

#include "ioguard.hpp"

studilova::Date::Date() noexcept:
  year_(0),
  month_(0),
  day_(0)
{}

studilova::Date::Date(int year, int month, int day):
  year_(year),
  month_(month),
  day_(day)
{
  if ((month < 1) || (month > 12))
  {
    throw std::invalid_argument("Incorrect month");
  }

  if ((day < 1) || (day > 31))
  {
    throw std::invalid_argument("Incorrect day");
  }
}

int studilova::Date::getYear() const noexcept
{
  return year_;
}

int studilova::Date::getMonth() const noexcept
{
  return month_;
}

int studilova::Date::getDay() const noexcept
{
  return day_;
}

bool studilova::Date::operator==(const Date& other) const
{
  return year_ == other.year_ && month_ == other.month_ && day_ == other.day_;
}

bool studilova::Date::operator!=(const Date& other) const
{
  return !(*this == other);
}

bool studilova::Date::operator<(const Date& other) const
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

std::istream& studilova::operator>>(std::istream& in, Date& date)
{
  int year = 0;
  int month = 0;
  int day = 0;

  char dash1 = '\0';
  char dash2 = '\0';

  in >> year >> dash1 >> month >> dash2 >> day;

  if (!in)
  {
    return in;
  }

  if ((dash1 != '-') || (dash2 != '-'))
  {
    in.setstate(std::ios::failbit);
    return in;
  }

  try
  {
    date = studilova::Date(year, month, day);
  }
  catch(const std::invalid_argument&)
  {
   in.setstate(std::ios::failbit);
  }

  return in;
}

std::ostream& studilova::operator<<(std::ostream& out, const Date& date)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  studilova::IOGuard guard(out);

  out << std::setw(4) << std::setfill('0') << date.getYear();
  out << '-';
  out << std::setw(2) << std::setfill('0') << date.getMonth();
  out << '-';
  out << std::setw(2) << std::setfill('0') << date.getDay();

  return out;
}