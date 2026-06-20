#ifndef DATE_HPP
#define DATE_HPP

#include <iosfwd>

namespace studilova
{
  class Date
  {
    public:
      Date() noexcept;
      Date(int year, int month, int day);

      int getYear() const noexcept;
      int getMonth() const noexcept;
      int getDay() const noexcept;

      bool operator==(const Date& other) const;
      bool operator!=(const Date& other) const;
      bool operator<(const Date& other) const;

    private:
      int year_;
      int month_;
      int day_;
  };

  std::istream& operator>>(std::istream& in, Date& date);
  std::ostream& operator<<(std::ostream& out, const Date& date);
}

#endif