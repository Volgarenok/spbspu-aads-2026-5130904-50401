#include <iostream>
#include <string>
#include <boost/functional/hash.hpp>
#include "hash-structs.hpp"

  size_t donkeev::VertexPairHash::operator()(const std::pair< std::string, std::string >& namePair) const
  {
    boost::hash<std::string> hasher;
    size_t h1 = hasher(namePair.first);
    size_t h2 = hasher(namePair.second);

    return h1 ^ (h2 << 1);
  }

  bool donkeev::VertexPairEqual::operator()(const std::pair< std::string, std::string >& from, const std::pair< std::string, std::string >& to) const
  {
    return from.first == to.first && from.second == to.second;
  }

  size_t donkeev::GraphNameHash::operator()(const std::string& str) const
  {
    boost::hash<std::string> hasher;
    return hasher(str);
  }

  bool donkeev::GraphEqual::operator()(const std::string& first, const std::string& second) const
  {
    return first == second;
  }
