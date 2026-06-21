#include "budget.hpp"

studilova::Budget::Budget() noexcept:
  name_(),
  rootCategory_("root", nullptr),
  operations_()
{}

studilova::Budget::Budget(const std::string& name) noexcept:
  name_(name),
  rootCategory_("root", nullptr),
  operations_()
{}

const std::string& studilova::Budget::getName() const noexcept
{
  return name_;
}

studilova::Category& studilova::Budget::getRootCategory() noexcept
{
  return rootCategory_;
}

const studilova::Category& studilova::Budget::getRootCategory() const noexcept
{
  return rootCategory_;
}

void studilova::Budget::addOperation(const Operation& operation)
{
  operations_.pushBack(operation);
}

const studilova::Vector< studilova::Operation >& studilova::Budget::getOperations() const noexcept
{
  return operations_;
}

studilova::Vector< studilova::Operation >& studilova::Budget::getOperations() noexcept
{
  return operations_;
}
