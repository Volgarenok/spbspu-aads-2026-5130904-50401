#ifndef MERGE_ENGINE_HPP
#define MERGE_ENGINE_HPP

#include <iosfwd>
#include <string>
#include "hashers.hpp"
#include <cuckooht.hpp>
#include <vector.hpp>

namespace chernov {

  class Tree;
  class Person;

  namespace detail {

    struct MergeContext {
      Tree & mergedTree;
      const Tree & treeA;
      const Tree & treeB;

      CuckooHT< std::string, std::string, Hasher1, Hasher2, std::equal_to< std::string > > pairMap;
      CuckooHT< std::string, std::string, Hasher1, Hasher2, std::equal_to< std::string > > copyMapA;
      CuckooHT< std::string, std::string, Hasher1, Hasher2, std::equal_to< std::string > > copyMapB;
      CuckooHT< std::string, std::string, Hasher1, Hasher2, std::equal_to< std::string > > mergedA;
      CuckooHT< std::string, std::string, Hasher1, Hasher2, std::equal_to< std::string > > mergedB;

      MergeContext(Tree & mTree, const Tree & a, const Tree & b);
      std::string makeKey(const std::string & idA, const std::string & idB);
      std::string mergePair(const std::string & idA, const std::string & idB, std::string & error);
      std::string copyFromA(const std::string & idA, std::string & error);
      std::string copyFromB(const std::string & idB, std::string & error);
    };

    bool areMatching(const std::string & idA,
      const std::string & idB,
      const Tree & treeA,
      const Tree & treeB,
      Vector< std::pair< std::string, std::string > > & visited);

    void compareTrees(
      const Tree & treeA, const Tree & treeB, const std::string & nameA, const std::string & nameB, std::ostream & out);

    std::string formatPersonDisplay(const Person & p);
    bool visitContains(const Vector< std::pair< std::string, std::string > > & visited,
      const std::string & idA,
      const std::string & idB);
    bool fieldsMatch(const Person & a, const Person & b);

    bool mergePersons(const Tree & treeA,
      const std::string & idA,
      const Tree & treeB,
      const std::string & idB,
      Tree & mergedTree,
      std::string & errorMsg);
  }
}

#endif
