#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "budget_manager.hpp"

namespace studilova
{
  void createBudget(std::istream&, std::ostream&, BudgetManager&);
  void createCategory(std::istream&, std::ostream&, BudgetManager&);

  void showBudgets(std::istream& in, std::ostream& out, BudgetManager& state);
  void showCategories(std::istream& in, std::ostream& out, BudgetManager& state);
  void showBalance(std::istream&, std::ostream&, BudgetManager&);
  void showExpenses(std::istream&, std::ostream&, BudgetManager&);

  void loadOperations(std::istream&, std::ostream&, BudgetManager&);
  void save(std::istream&, std::ostream&, BudgetManager&);

  void findExpenses(std::istream&, std::ostream&, BudgetManager&);
  void checkBalanceTrend(std::istream&, std::ostream&, BudgetManager&);
  void categoryTrend(std::istream&, std::ostream&, BudgetManager&);
  void compareTrends(std::istream&, std::ostream&, BudgetManager&);
}

#endif
