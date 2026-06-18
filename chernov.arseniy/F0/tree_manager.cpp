#include "tree_manager.hpp"

namespace chernov {

  TreeManager::TreeManager():
    trees_(8)
  {}

  bool TreeManager::hasTree(const std::string & name) const
  {
    return trees_.has(name);
  }

  void TreeManager::createTree(const std::string & name, const std::string & description, std::ostream & out)
  {
    if (trees_.has(name)) {
      out << "<ERROR: Tree '" << name << "' already exists>\n";
      return;
    }
    Tree newTree(name, description);
    trees_.add(name, newTree);
    out << "<CREATED: " << name << ">\n";
  }

  void TreeManager::dropTree(const std::string & name, std::ostream & out)
  {
    if (!trees_.has(name)) {
      out << "<ERROR: Tree '" << name << "' not found>\n";
      return;
    }
    trees_.remove(name);
    out << "<OK: Tree " << name << " removed>\n";
  }

  void TreeManager::renameTree(const std::string & oldName, const std::string & newName, std::ostream & out)
  {
    if (!trees_.has(oldName)) {
      out << "<ERROR: Tree '" << oldName << "' not found>\n";
      return;
    }
    if (trees_.has(newName)) {
      out << "<ERROR: Tree '" << newName << "' already exists>\n";
      return;
    }
    Tree tree = trees_.at(oldName);
    Tree newTree(newName, tree.getDescription());
    trees_.remove(oldName);
    trees_.add(newName, newTree);
    out << "<OK: Renamed to " << newName << ">\n";
  }

  void TreeManager::editTreeDescription(const std::string & name, const std::string & description, std::ostream & out)
  {
    if (!trees_.has(name)) {
      out << "<ERROR: Tree '" << name << "' not found>\n";
      return;
    }
    Tree & tree = trees_.at(name);
    tree.setDescription(description);
    out << "<OK>\n";
  }

  void TreeManager::listTrees(std::ostream & out) const
  {
    out << "<TREES:>\n";
    for (auto it = trees_.cbegin(); it != trees_.cend(); ++it) {
      const auto & pair = *it;
      const std::string & name = pair.first;
      const Tree & tree = pair.second;
      out << "<  " << name << ": \"" << tree.getDescription() << "\" (" << tree.personCount() << " persons)>\n";
    }
  }
}
