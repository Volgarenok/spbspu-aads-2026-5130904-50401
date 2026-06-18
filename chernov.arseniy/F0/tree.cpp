#include "tree.hpp"

chernov::Tree::Tree(const std::string & name, const std::string & description):
  name_(name),
  description_(description)
{}

const std::string & chernov::Tree::getName() const
{
  return name_;
}

const std::string & chernov::Tree::getDescription() const
{
  return description_;
}

void chernov::Tree::setDescription(const std::string & desc)
{
  description_ = desc;
}

std::size_t chernov::Tree::personCount() const
{
  return persons_.getSize();
}
