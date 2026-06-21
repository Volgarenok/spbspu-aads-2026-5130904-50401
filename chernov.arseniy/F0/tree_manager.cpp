#include "tree_manager.hpp"
#include "file_io.hpp"

chernov::TreeManager::TreeManager():
  trees_(8)
{}

bool chernov::TreeManager::hasTree(const std::string & name) const
{
  return trees_.has(name);
}

void chernov::TreeManager::createTree(const std::string & name, const std::string & description, std::ostream & out)
{
  if (trees_.has(name)) {
    out << "<ERROR: Tree '" << name << "' already exists>\n";
    return;
  }
  Tree newTree(name, description);
  trees_.add(name, newTree);
  out << "<CREATED: " << name << ">\n";
}

void chernov::TreeManager::dropTree(const std::string & name, std::ostream & out)
{
  if (!trees_.has(name)) {
    out << "<ERROR: Tree '" << name << "' not found>\n";
    return;
  }
  trees_.remove(name);
  out << "<OK: Tree " << name << " removed>\n";
}

void chernov::TreeManager::renameTree(const std::string & oldName, const std::string & newName, std::ostream & out)
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

void chernov::TreeManager::editTreeDescription(
  const std::string & name, const std::string & description, std::ostream & out)
{
  if (!trees_.has(name)) {
    out << "<ERROR: Tree '" << name << "' not found>\n";
    return;
  }
  trees_.at(name).setDescription(description);
  out << "<OK>\n";
}

void chernov::TreeManager::listTrees(std::ostream & out) const
{
  out << "<TREES:>\n";
  for (auto it = trees_.cbegin(); it != trees_.cend(); ++it) {
    const auto & pair = *it;
    out << "<  " << pair.first << ": \"" << pair.second.getDescription() << "\" (" << pair.second.personCount()
        << " persons)>\n";
  }
}

chernov::Tree & chernov::TreeManager::getTree(const std::string & name)
{
  return trees_.at(name);
}

const chernov::Tree & chernov::TreeManager::getTree(const std::string & name) const
{
  return trees_.at(name);
}

void chernov::TreeManager::saveTree(const std::string & treeName, const std::string & filename, std::ostream & out)
{
  if (!trees_.has(treeName)) {
    out << "<ERROR: Tree '" << treeName << "' not found>\n";
    return;
  }
  if (detail::saveTree(trees_.at(treeName), filename)) {
    out << "<OK: Saved to " << filename << ">\n";
  } else {
    out << "<ERROR: Failed to save>\n";
  }
}

void chernov::TreeManager::loadTree(const std::string & treeName, const std::string & filename, std::ostream & out)
{
  if (trees_.has(treeName)) {
    out << "<ERROR: Tree '" << treeName << "' already exists>\n";
    return;
  }
  Tree emptyTree(treeName, "");
  trees_.add(treeName, emptyTree);
  Tree & tree = trees_.at(treeName);
  std::string error;
  if (!detail::loadTree(tree, filename, error)) {
    trees_.remove(treeName);
    out << "<ERROR: " << error << ">\n";
    return;
  }
  out << "<OK: Loaded " << treeName << " from " << filename << ">\n";
}
