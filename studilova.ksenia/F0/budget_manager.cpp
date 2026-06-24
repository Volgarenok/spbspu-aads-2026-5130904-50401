#include "budget_manager.hpp"

#include <stdexcept>

void studilova::BudgetManager::createBudget(const std::string& name)
{
  budgets_.add(name, Budget(name));
}

studilova::Budget& studilova::BudgetManager::getBudget(const std::string& name)
{
  return budgets_.get(name);
}

const studilova::Budget& studilova::BudgetManager::getBudget(const std::string& name) const
{
  return budgets_.get(name);
}

bool studilova::BudgetManager::hasBudget(const std::string& name) const
{
  try
  {
    budgets_.get(name);
    return true;
  }
  catch (const std::out_of_range&)
  {
    return false;
  }
}

void studilova::BudgetManager::removeBudget(const std::string& name)
{
  budgets_.erase(name);
}

studilova::Vector<std::string> studilova::BudgetManager::getBudgetNames() const
{
  Vector< std::string > result;
  for (auto it = budgets_.begin(); it != budgets_.end(); ++it)
  {
    result.pushBack((*it).first);
  }
  return result;
}
