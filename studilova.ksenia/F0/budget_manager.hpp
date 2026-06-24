#ifndef BUDGET_MANAGER_HPP
#define BUDGET_MANAGER_HPP

#include <string>
#include <stdexcept>

#include <hash_table.hpp>
#include <hmac_hash.hpp>

#include "budget.hpp"

namespace studilova
{
	class BudgetManager
	{
		public:
  		using BudgetTable = HashTable< std::string, Budget, HMACHash, std::equal_to< std::string > >;

  		BudgetManager() = default;
  		BudgetManager(BudgetManager&&) = delete;
			BudgetManager(const BudgetManager&) = delete;
			BudgetManager& operator=(const BudgetManager&) = delete;
  		BudgetManager& operator=(BudgetManager&&) = delete;
			~BudgetManager() = default;

			void createBudget(const std::string& name);
			Budget& getBudget(const std::string& name);
			const Budget& getBudget(const std::string& name) const;
			bool hasBudget(const std::string& name) const;
			void removeBudget(const std::string& name);

			Vector<std::string> getBudgetNames() const;

		private:
  		BudgetTable budgets_;
	};
}

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

#endif
