#ifndef RELATIONSHIP_HPP
#define RELATIONSHIP_HPP

#include <string>
#include <vector.hpp>

namespace chernov {

  class Tree;

  namespace detail {
    Vector< std::string > findAncestors(const Tree & tree, const std::string & id, int maxDepth);
    Vector< std::string > findDescendants(const Tree & tree, const std::string & id, int maxDepth);
    struct Relative {
      std::string id;
      std::string relation;
    };
    Vector< Relative > findRelatives(const Tree & tree, const std::string & id, int maxDepth);
  }

}

#endif
