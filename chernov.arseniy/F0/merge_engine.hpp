#ifndef MERGE_ENGINE_HPP
#define MERGE_ENGINE_HPP

#include <iosfwd>
#include <string>
#include "person.hpp"
#include <vector.hpp>

namespace chernov {
  class Tree;

  namespace detail {
    std::string formatPersonDisplay(const Person & p);
    bool visitContains(const Vector< std::pair< std::string, std::string > > & visited,
      const std::string & idA,
      const std::string & idB);
    bool fieldsMatch(const Person & a, const Person & b);
    bool areMatching(const std::string & idA,
      const std::string & idB,
      const Tree & treeA,
      const Tree & treeB,
      Vector< std::pair< std::string, std::string > > & visited);
    void compareTrees(
      const Tree & treeA, const Tree & treeB, const std::string & nameA, const std::string & nameB, std::ostream & out);
  }
}

#endif
