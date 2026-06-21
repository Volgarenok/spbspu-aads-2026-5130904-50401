#ifndef BUDGET_HPP
#define BUDGET_HPP

#include <string>

#include <vector.hpp>

#include "category.hpp"
#include "operation.hpp"
#include "avltree.hpp"

namespace studilova
{
  class Budget
  {
    public:
      Budget();
      explicit Budget(const std::string& name);
      ~Budget();

      const std::string& getName() const noexcept;

      Category& getRootCategory() noexcept;
      const Category& getRootCategory() const noexcept;

      void addOperation(const Operation& operation);

      const Vector< Operation >& getOperations() const noexcept;
      Vector< Operation >& getOperations() noexcept;

      bool hasCategory(const std::string& name) const;

      Category& getCategory(const std::string& name);
      const Category& getCategory(const std::string& name) const;

      void addCategory(const std::string& name, const std::string& parentName);

    private:
      std::string name_;
      Category rootCategory_;
      Vector< Operation > operations_;
      AVLTree< std::string, Category* > categories_;

      void clearCategoryChildren(Category& category);
  };
}

#endif
