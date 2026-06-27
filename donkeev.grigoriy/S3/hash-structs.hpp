#ifndef HASH_STRUCTS_HPP
#define HASH_STRUCTS_HPP

#include "iostream"

namespace donkeev
{
  using edgesPair_t = std::pair< std::string, std::string >;

  struct VertexPairHash
  {
    size_t operator()(const edgesPair_t&) const;
  };

  struct VertexPairEqual
  {
    bool operator()(const edgesPair_t&, const edgesPair_t&) const;
  };

  struct GraphNameHash
  {
    size_t operator()(const std::string&) const;
  };

  struct GraphEqual
  {
    bool operator()(const std::string&, const std::string&) const;
  };

  struct CommandsHash
  {
    size_t operator()(const std::string&) const;
  };

  struct CommandsEqual
  {
    bool operator()(const std::string&, const std::string&) const;
  };

  struct TestStructHash
  {
    size_t operator()(const std::string&) const;
  };

  struct TestStructEqual
  {
    bool operator()(const std::string&, const std::string&) const;
  };
}

#endif
