#include "Budget.hpp"

studilova::Budget::Budget():
  name_()
{}

studilova::Budget::Budget(const std::string& name):
  name_(name)
{}

const std::string& studilova::Budget::getName() const
{
  return name_;
}
