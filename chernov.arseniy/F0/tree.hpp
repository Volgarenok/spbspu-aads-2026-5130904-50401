#ifndef TREE_HPP
#define TREE_HPP

#include <string>
#include "person.hpp"
#include <vector.hpp>

namespace chernov {
  class Tree {
  public:
    Tree(const std::string & name, const std::string & description = "");
    const std::string & getName() const;
    const std::string & getDescription() const;
    void setDescription(const std::string & desc);
    size_t personCount() const;

  private:
    std::string name_;
    std::string description_;
    Vector< Person > persons_;
  };
}

#endif
