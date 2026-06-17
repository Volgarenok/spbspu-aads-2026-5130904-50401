#ifndef DATE_HPP
#define DATE_HPP
#include <string>
#include <cstddef>

namespace vasyakin
{
  class Date
  {
  public:
    Date() noexcept;
    Date(size_t date, size_t month, size_t year);

    size_t getDay() const noexcept;
    size_t getMonth() const noexcept;
    size_t getYear() const noexcept;

    Date operator+(size_t days) const noexcept;
    size_t daysDiff(const Date& other) const noexcept;
    bool isValid() const noexcept;

    std::string toString() const;
    static Date fromString(const std::string& str);

    bool operator<(const Date& other) const noexcept;
    bool operator>(const Date& other) const noexcept;
    bool operator==(const Date& other) const noexcept;
    bool operator!=(const Date& other) const noexcept;
    bool operator<=(const Date& other) const noexcept;
    bool operator>=(const Date& other) const noexcept;
  private:
    size_t day_;
    size_t month_;
    size_t year_;

    void normalize() noexcept;
    static bool isLeapYear(size_t year) noexcept;
    static size_t getDaysInMonth(size_t month, size_t year) noexcept;
  };
}

#endif
