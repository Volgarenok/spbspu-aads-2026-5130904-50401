#include <iostream>
#include <limits>
#include <string>

#include <hash_table.hpp>
#include <hmac_hash.hpp>

#include "commands.hpp"
#include "budget_manager.hpp"

int main()
{
  studilova::BudgetManager state;

  using command_table_t = studilova::HashTable< std::string, studilova::cmd_t, studilova::HMACHash, std::equal_to< std::string > >;

  command_table_t commands;

  commands.add("create-budget", studilova::createBudget);
  commands.add("create-category", studilova::createCategory);
  commands.add("show-budgets", studilova::showBudgets);
  commands.add("show-categories", studilova::showCategories);
  commands.add("load-operations", studilova::loadOperations);
  commands.add("show-balance", studilova::showBalance);
  commands.add("merge-budgets", studilova::mergeBudgets);
  commands.add("show-expenses", studilova::showExpenses);
  commands.add("find-expenses", studilova::findExpenses);
  commands.add("check-balance-trend", studilova::checkBalanceTrend);
  commands.add("category-trend", studilova::categoryTrend);
  commands.add("compare-trends", studilova::compareTrends);
  commands.add("show-category-diagram", studilova::showCategoryDiagram);
  commands.add("save", studilova::save);

  std::string commandName;
  while (std::cin >> commandName)
  {
    try
    {
      commands.get(commandName)(std::cin, std::cout, state);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
