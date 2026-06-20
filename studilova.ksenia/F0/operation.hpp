#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <string>
#include <iosfwd>

#include "date.hpp"

namespace studilova
{
  enum class OperationType
  {
    Income,
    Expense
  };

  class Operation
  {
    public:
      Operation() noexcept;
      Operation(OperationType type, const std::string& category, int amount, const Date& date);

      OperationType getType() const noexcept;
      const std::string& getCategory() const noexcept;
      int getAmount() const noexcept;
      const Date& getDate() const noexcept;

    private:
      OperationType type_;
      std::string category_;
      int amount_;
      Date date_;
  };

  std::istream& operator >>(std::istream& in, Operation& operation);
  std::ostream& operator<<(std::ostream& out, const Operation& operation);
}

#endif
