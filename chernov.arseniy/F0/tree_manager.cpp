#include "tree_manager.hpp"
#include "file_io.hpp"
#include "merge_engine.hpp"

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

void chernov::TreeManager::editTreeDescription(const std::string & name,
  const std::string & description,
  std::ostream & out)
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

void chernov::TreeManager::loadTree(const std::string & filename, const std::string & forcedName, std::ostream & out)
{
  std::string treeNameFromFile;
  size_t nextIdFromFile;
  std::string error;
  if (!detail::peekTreeMetadata(filename, treeNameFromFile, nextIdFromFile, error)) {
    out << "<ERROR: " << error << ">\n";
    return;
  }

  std::string finalName = forcedName.empty() ? treeNameFromFile : forcedName;
  if (!forcedName.empty() && trees_.has(finalName)) {
    out << "<ERROR: Tree '" << finalName << "' already exists>\n";
    return;
  }
  if (forcedName.empty() && trees_.has(treeNameFromFile)) {
    out << "<ERROR: Tree '" << treeNameFromFile << "' already exists>\n";
    return;
  }

  trees_.add(finalName, Tree(finalName, ""));
  Tree & tree = trees_.at(finalName);
  if (!detail::loadTree(tree, filename, error)) {
    trees_.remove(finalName);
    out << "<ERROR: " << error << ">\n";
    return;
  }
  out << "<OK: Loaded " << finalName << " from " << filename << ">\n";
}

void chernov::TreeManager::compareTrees(const std::string & nameA, const std::string & nameB, std::ostream & out)
{
  if (!trees_.has(nameA)) {
    out << "<ERROR: Tree '" << nameA << "' not found>\n";
    return;
  }
  if (!trees_.has(nameB)) {
    out << "<ERROR: Tree '" << nameB << "' not found>\n";
    return;
  }
  detail::compareTrees(trees_.at(nameA), trees_.at(nameB), nameA, nameB, out);
}

void chernov::TreeManager::mergePersons(const std::string & tree1,
  const std::string & id1,
  const std::string & tree2,
  const std::string & id2,
  const std::string & newTreeName,
  std::ostream & out)
{
  if (!trees_.has(tree1)) {
    out << "<ERROR: Tree '" << tree1 << "' not found>\n";
    return;
  }
  if (!trees_.has(tree2)) {
    out << "<ERROR: Tree '" << tree2 << "' not found>\n";
    return;
  }
  if (trees_.has(newTreeName)) {
    out << "<ERROR: Tree '" << newTreeName << "' already exists>\n";
    return;
  }

  Tree mergedTree(newTreeName, "");
  std::string error;
  if (detail::mergePersons(trees_.at(tree1), id1, trees_.at(tree2), id2, mergedTree, error)) {
    trees_.add(newTreeName, std::move(mergedTree));
    out << "<CREATED: " << newTreeName << ">\n";
    out << "<MERGE SUCCESS>\n";
  } else {
    out << "<ERROR: " << error << ">\n";
  }
}
