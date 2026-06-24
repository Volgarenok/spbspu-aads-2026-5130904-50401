#include "category.hpp"

studilova::Category::Category() noexcept:
  name_(),
  parent_(nullptr),
  children_()
{}

studilova::Category::Category(const std::string& name, Category* parent) noexcept:
  name_(name),
  parent_(parent),
  children_()
{}

const std::string& studilova::Category::getName() const noexcept
{
  return name_;
}

studilova::Category* studilova::Category::getParent() const noexcept
{
  return parent_;
}

void studilova::Category::addChild(Category* child)
{
  children_.pushBack(child);
}

const studilova::Vector< studilova::Category* >& studilova::Category::getChildren() const noexcept
{
  return children_;
}

studilova::Vector< studilova::Category* >& studilova::Category::getChildren() noexcept
{
  return children_;
}
