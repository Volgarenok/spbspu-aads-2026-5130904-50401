#ifndef BUDGET_MANAGER_HPP
#define BUDGET_MANAGER_HPP

#include <string>

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

#endif
