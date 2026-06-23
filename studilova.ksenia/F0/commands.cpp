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

void studilova::showBalance(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  in >> budgetName;

  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  int balance = 0;
  const Vector< Operation >& ops = budget.getOperations();

  for (size_t i = 0; i < ops.getSize(); ++i)
  {
    const Operation& op = ops[i];

    if (op.getType() == OperationType::Income)
    {
      balance += op.getAmount();
    }
    else
    {
      balance -= op.getAmount();
    }
  }
  out << balance << '\n';
}

void studilova::showExpenses(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  in >> budgetName;

  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (!state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  const Vector< Operation >& ops = budget.getOperations();
  bool first = true;

  for (size_t i = 0; i < ops.getSize(); ++i)
  {
    const Operation& op = ops[i];

    if (op.getType() == OperationType::Expense)
    {
      if (!first)
      {
        out << ", ";
      }
      out << op.getCategory() << ": " << op.getAmount();
      first = false;
    }
  }
  out << '\n';
}
