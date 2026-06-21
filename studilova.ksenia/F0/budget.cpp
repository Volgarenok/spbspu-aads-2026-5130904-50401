#include "budget.hpp"

studilova::Budget::Budget():
  name_(),
  rootCategory_("root", nullptr),
  operations_(),
  categories_()
{
  categories_.push("root", &rootCategory_);
}

studilova::Budget::Budget(const std::string& name):
  name_(name),
  rootCategory_("root", nullptr),
  operations_(),
  categories_()
{
  categories_.push("root", &rootCategory_);
}

void studilova::Budget::clearCategoryChildren(Category& category)
{
  Vector< Category* >& children = category.getChildren();

  for (size_t i = 0; i < children.getSize(); ++i)
  {
    clearCategoryChildren(*children[i]);
    delete children[i];
  }
}

studilova::Budget::~Budget()
{
  clearCategoryChildren(rootCategory_);
}

const std::string& studilova::Budget::getName() const noexcept
{
  return name_;
}

studilova::Category& studilova::Budget::getRootCategory() noexcept
{
  return rootCategory_;
}

const studilova::Category& studilova::Budget::getRootCategory() const noexcept
{
  return rootCategory_;
}

void studilova::Budget::addOperation(const Operation& operation)
{
  operations_.pushBack(operation);
}

const studilova::Vector< studilova::Operation >& studilova::Budget::getOperations() const noexcept
{
  return operations_;
}

studilova::Vector< studilova::Operation >& studilova::Budget::getOperations() noexcept
{
  return operations_;
}

bool studilova::Budget::hasCategory(const std::string& name) const
{
  try
  {
    categories_.get(name);
    return true;
  }
  catch (const std::out_of_range&)
  {
    return false;
  }
}

studilova::Category& studilova::Budget::getCategory(const std::string& name)
{
  return *categories_.get(name);
}

const studilova::Category& studilova::Budget::getCategory(const std::string& name) const
{
  return *categories_.get(name);
}

void studilova::Budget::addCategory(const std::string& name, const std::string& parentName)
{
  if (hasCategory(name))
  {
    throw std::logic_error("Category already exists");
  }

  Category& parent = getCategory(parentName);

  Category* category = new Category(name, &parent);
  parent.addChild(category);
  categories_.push(name, category);
}
