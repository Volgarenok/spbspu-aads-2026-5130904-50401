#ifndef RELATIONSHIP_HPP
#define RELATIONSHIP_HPP

#include <string>
#include "hashers.hpp"
#include <cuckooht.hpp>
#include <vector.hpp>

namespace chernov {
  class Tree;
  class Person;

  namespace detail {
    struct Relative {
      std::string id;
      std::string relation;
    };

    Vector< std::string > findAncestors(const Tree & tree, const std::string & id, int maxDepth);
    Vector< std::string > findDescendants(const Tree & tree, const std::string & id, int maxDepth);
    Vector< Relative > findRelatives(const Tree & tree, const std::string & id, int maxDepth);

    using AncestorDistances = CuckooHT< std::string, int, Hasher1, Hasher2, std::equal_to< std::string > >;
    void collectAncestorDistances(const Tree & tree, const std::string & id, AncestorDistances & distances);
    std::string makeGreatPrefix(int count);
    std::string getAncestorTitle(const std::string & gender, int upDist);
    std::string getDescendantTitle(const std::string & gender, int downDist);

    Vector< std::string > findCommonAncestors(const Tree & tree, const std::string & id1, const std::string & id2);
    std::string getRelationship(const Tree & tree, const std::string & id1, const std::string & id2);

    std::string getRelationLabel(const Tree & tree, const std::string & from, const std::string & to);
  }
}

#endif
