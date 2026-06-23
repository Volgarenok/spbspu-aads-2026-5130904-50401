#include "commands.hpp"

void studilova::createBudget(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string name;
  in >> name;

  if (name.empty())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (state.hasBudget(name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  state.createBudget(name);

  out << "<OK>\n";
}

void studilova::createCategory(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  std::string categoryName;
  std::string parentName;

  in >> budgetName >> categoryName >> parentName;

  if (!state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    Budget& budget = state.getBudget(budgetName);
    budget.addCategory(categoryName, parentName);

    out << "<OK>\n";
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}
