#ifndef BUDGET_HPP
#define BUDGET_HPP

#include <string>

#include <vector.hpp>

#include "category.hpp"
#include "operation.hpp"

namespace studilova
{
  class Budget
  {
    public:
      Budget() noexcept;
      explicit Budget(const std::string& name) noexcept;
      ~Budget();

      const std::string& getName() const noexcept;

      Category& getRootCategory() noexcept;
      const Category& getRootCategory() const noexcept;

      void addOperation(const Operation& operation);

      const Vector< Operation >& getOperations() const noexcept;
      Vector< Operation >& getOperations() noexcept;

    private:
      std::string name_;
      Category rootCategory_;
      Vector< Operation > operations_;

      void clearCategoryChildren(Category& category);
  };
}

#endif
