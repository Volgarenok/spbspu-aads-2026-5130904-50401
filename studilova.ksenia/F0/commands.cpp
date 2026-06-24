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

  int getCategoryTrendValue(const studilova::Budget& budget, const std::string& categoryName)
  {
    const studilova::Vector< studilova::Operation >& operations = budget.getOperations();

    int firstHalf = 0;
    int secondHalf = 0;
    size_t middle = operations.getSize() / 2;

    for (size_t i = 0; i < operations.getSize(); ++i)
    {
      const studilova::Operation& operation = operations[i];

      if (operation.getType() == studilova::OperationType::Expense && operation.getCategory() == categoryName)
      {
        if (i < middle)
        {
          firstHalf += operation.getAmount();
        }
        else
        {
          secondHalf += operation.getAmount();
        }
      }
    }

    return secondHalf - firstHalf;
  }

  void copyCategoryChildren(const studilova::Category& from, studilova::Budget& dest, const std::string& destParent)
  {
    const studilova::Vector< studilova::Category* >& children = from.getChildren();

    for (size_t i = 0; i < children.getSize(); ++i)
    {
      const studilova::Category* child = children[i];

      if (!dest.hasCategory(child->getName()))
      {
        dest.addCategory(child->getName(), destParent);
      }

      copyCategoryChildren(*child, dest, child->getName());
    }
  }

  void copyOperations(const studilova::Budget& from, studilova::Budget& dest)
  {
    const studilova::Vector< studilova::Operation >& operations = from.getOperations();

    for (size_t i = 0; i < operations.getSize(); ++i)
    {
      dest.addOperation(operations[i]);
    }
  }

  const char* getDiagramSymbols()
  {
    return "#&@%+=*xo";
  }

  int calculateCategoryExpenses(const studilova::Budget& budget, const std::string& categoryName)
  {
    int sum = 0;
    const studilova::Vector< studilova::Operation >& operations = budget.getOperations();

    for (size_t i = 0; i < operations.getSize(); ++i)
    {
      const studilova::Operation& operation = operations[i];

      if (operation.getType() == studilova::OperationType::Expense && operation.getCategory() == categoryName)
      {
        sum += operation.getAmount();
      }
    }
    return sum;
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

void studilova::checkBalanceTrend(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  in >> budgetName;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  const Vector< Operation >& operations = budget.getOperations();

  int balance = 0;

  for (size_t i = 0; i < operations.getSize(); ++i)
  {
    const Operation& operation = operations[i];

    if (operation.getType() == OperationType::Income)
    {
      balance += operation.getAmount();
    }
    else
    {
      balance -= operation.getAmount();
    }

    if (balance < 0)
    {
      out << "<EXPENSES EXCEED INCOME>\n";
      return;
    }
  }

  out << "<INCOME COVERS EXPENSES>\n";
}

void studilova::categoryTrend(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  std::string categoryName;

  in >> budgetName >> categoryName;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);

  if (!budget.hasCategory(categoryName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  int trend = getCategoryTrendValue(budget, categoryName);

  if (trend > 0)
  {
    out << "<INCREASING>\n";
  }
  else if (trend < 0)
  {
    out << "<DECREASING>\n";
  }
  else
  {
    out << "<STABLE>\n";
  }
}

void studilova::compareTrends(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  std::string categoryName1;
  std::string categoryName2;

  in >> budgetName >> categoryName1 >> categoryName2;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);

  if (!budget.hasCategory(categoryName1) || !budget.hasCategory(categoryName2))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  int trend1 = getCategoryTrendValue(budget, categoryName1);
  int trend2 = getCategoryTrendValue(budget, categoryName2);

  if (trend1 > trend2)
  {
    out << "<" << categoryName1 << " GROWS FASTER>\n";
  }
  else if (trend2 > trend1)
  {
    out << "<" << categoryName2 << " GROWS FASTER>\n";
  }
  else
  {
    out << "<TRENDS ARE EQUAL>\n";
  }
}

void studilova::mergeBudgets(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string newName;
  std::string firstName;
  std::string secondName;

  in >> newName >> firstName >> secondName;

  if (!in || state.hasBudget(newName) || !state.hasBudget(firstName) || !state.hasBudget(secondName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  try
  {
    const Budget& first = state.getBudget(firstName);
    const Budget& second = state.getBudget(secondName);

    state.createBudget(newName);
    Budget& merged = state.getBudget(newName);

    copyCategoryChildren(first.getRootCategory(), merged, "root");
    copyCategoryChildren(second.getRootCategory(), merged, "root");

    copyOperations(first, merged);
    copyOperations(second, merged);

    out << "<OK>\n";
  }
  catch (...)
  {
    out << "<INVALID COMMAND>\n";
  }
}

void studilova::showCategoryDiagram(std::istream& in, std::ostream& out, BudgetManager& state)
{
  std::string budgetName;
  in >> budgetName;

  if (!in || !state.hasBudget(budgetName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const Budget& budget = state.getBudget(budgetName);
  const Vector< Category* >& categories = budget.getRootCategory().getChildren();

  if (categories.isEmpty())
  {
    out << "<EMPTY>\n";
    return;
  }

  const char* symbols = getDiagramSymbols();
  const size_t maxWidth = 30;

  Vector< int > sums;
  int total = 0;

  for (size_t i = 0; i < categories.getSize(); ++i)
  {
    int sum = calculateCategoryExpenses(budget, categories[i]->getName());
    sums.pushBack(sum);
    total += sum;
  }

  if (total == 0)
  {
    out << "<EMPTY>\n";
    return;
  }

  for (size_t i = 0; i < categories.getSize(); ++i)
  {
    out << categories[i]->getName() << ' ' << symbols[i % 9] << '\n';
  }

  out << '[';

  for (size_t i = 0; i < categories.getSize(); ++i)
  {
    int count = static_cast< int >((sums[i] * maxWidth) / total);

    if (sums[i] > 0 && count == 0)
    {
      count = 1;
    }

    if (i != 0)
    {
      out << ' ';
    }

    for (int j = 0; j < count; ++j)
    {
      out << symbols[i % 9];
    }
  }

  out << "]\n";
}
