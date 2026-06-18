#ifndef TREE_MANAGER_HPP
#define TREE_MANAGER_HPP

#include <iostream>
#include <string>
#include "hashers.hpp"
#include "tree.hpp"
#include <cuckooht.hpp>

namespace chernov {
  class TreeManager {
  public:
    TreeManager();
    void createTree(const std::string & name, const std::string & description, std::ostream & out);
    void dropTree(const std::string & name, std::ostream & out);
    void renameTree(const std::string & oldName, const std::string & newName, std::ostream & out);
    void editTreeDescription(const std::string & name, const std::string & description, std::ostream & out);
    void listTrees(std::ostream & out) const;
    bool hasTree(const std::string & name) const;
    Tree & getTree(const std::string & name);
    const Tree & getTree(const std::string & name) const;

  private:
    CuckooHT< std::string, Tree, Hasher1, Hasher2, std::equal_to< std::string > > trees_;
  };
}

#endif
