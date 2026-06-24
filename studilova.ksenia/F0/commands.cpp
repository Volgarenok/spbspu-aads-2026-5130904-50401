#include "commands.hpp"

#include <fstream>
#include <stdexcept>
#include <string>

namespace
{
  void printCategory(std::ostream& out, const studilova::Category& category, size_t depth)
  {
    for (size_t i = 0; i < depth; ++i)
    {
      out << "  ";
    }

    out << category.getName() << '\n';

    const studilova::Vector< studilova::Category* >& children = category.getChildren();

    for (size_t i = 0; i < children.getSize(); ++i)
    {
      printCategory(out, *children[i], depth + 1);
    }
  }
}

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

void studilova::showBudgets(std::istream&, std::ostream& out, BudgetManager& state)
{
  Vector< std::string > names = state.getBudgetNames();

  for (size_t i = 0; i < names.getSize(); ++i)
  {
    if (i != 0)
    {
      out << ' ';
    }
    out << names[i];
  }

  out << '\n';
}

void studilova::showCategories(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  in >> budgetName;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  printCategory(out, budget.getRootCategory(), 0);
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

void studilova::loadOperations(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  std::string filename;

  in >> budgetName >> filename;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::ifstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Budget& budget = state.getBudget(budgetName);
  Operation operation;

  while (file >> operation)
  {
    if (!budget.hasCategory(operation.getCategory()))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    budget.addOperation(operation);
  }

  if (!file.eof())
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  out << "<OK>\n";
}

void studilova::save(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string filename;
  in >> filename;

  if (!in)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::ofstream file(filename);
  if (!file)
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  Vector<std::string> names = state.getBudgetNames();

  for (size_t i = 0; i < names.getSize(); ++i)
  {
    const Budget& budget = state.getBudget(names[i]);
    file << "budget " << budget.getName() << '\n';

    const Vector< Operation >& operations = budget.getOperations();

    for (size_t j = 0; j < operations.getSize(); ++j)
    {
      file << operations[j] << '\n';
    }
  }

  out << "<OK>\n";
}

void studilova::findExpenses(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  int min = 0;
  int max = 0;

  in >> budgetName >> min >> max;

  if (!in || min > max || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  const Vector< Operation >& operations = budget.getOperations();

  bool first = true;

  for (size_t i = 0; i < operations.getSize(); ++i)
  {
    const Operation& operation = operations[i];

    if (operation.getType() == OperationType::Expense && operation.getAmount() >= min && operation.getAmount() <= max)
    {
      if (!first)
      {
        out << ", ";
      }

      out << operation.getCategory() << ": " << operation.getAmount();
      first = false;
    }
  }

  out << '\n';
}
