#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "budget_manager.hpp"

namespace studilova
{
  void createBudget(std::istream&, std::ostream&, BudgetManager&);
  void createCategory(std::istream&, std::ostream&, BudgetManager&);
  void showBalance(std::istream&, std::ostream&, BudgetManager&);
  void showExpenses(std::istream&, std::ostream&, BudgetManager&);
}

#endif
