#include "operation.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
  studilova::OperationType operationStringToEnum(const std::string& text)
  {
    if (text == "income")
    {
      return studilova::OperationType::Income;
    }
    if (text == "expense")
    {
      return studilova::OperationType::Expense;
    }
    throw std::invalid_argument("Incorrect operation type");
  }

  const char* operationEnumToString(studilova::OperationType type)
  {
    if (type == studilova::OperationType::Income)
    {
      return "income";
    }
    return "expense";
  }
}

studilova::Operation::Operation() noexcept:
  type_(OperationType::Income),
  category_(),
  amount_(0),
  date_()
{}

studilova::Operation::Operation(OperationType type, const std::string& category, int amount, const Date& date):
  type_(type),
  category_(category),
  amount_(amount),
  date_(date)
{
  if (category.empty())
  {
    throw std::invalid_argument("Empty category");
  }
  if (amount < 0)
  {
    throw std::invalid_argument("Negative amount");
  }
}

studilova::OperationType studilova::Operation::getType() const noexcept
{
  return type_;
}

const std::string& studilova::Operation::getCategory() const noexcept
{
  return category_;
}

int studilova::Operation::getAmount() const noexcept
{
  return amount_;
}

const studilova::Date& studilova::Operation::getDate() const noexcept
{
  return date_;
}

std::istream& studilova::operator>>(std::istream& in, Operation& operation)
{
  std::istream::sentry sentry(in);
  if (!sentry)
  {
    return in;
  }

  std::string type;
  std::string category;
  int amount = 0;
  Date date;

  in >> type >> category >> amount >> date;
  if (!in)
  {
    return in;
  }

  try
  {
    operation = Operation(operationStringToEnum(type), category, amount, date);
  }
  catch(const std::invalid_argument&)
  {
    in.setstate(std::ios::failbit);
  }
  return in;
}

std::ostream& studilova::operator<<(std::ostream& out, const Operation& operation)
{
  std::ostream::sentry sentry(out);
  if (!sentry)
  {
    return out;
  }

  out << operationEnumToString(operation.getType());
  out << ' ' << operation.getCategory();
  out << ' ' << operation.getAmount();
  out << ' ' << operation.getDate();

  return out;
}
