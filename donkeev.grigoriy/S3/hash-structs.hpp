#ifndef HASH_STRUCTS_HPP
#define HASH_STRUCTS_HPP

#include "iostream"

namespace donkeev
{
  struct VertexPairHash
  {
    size_t operator()(const std::pair< std::string, std::string >&) const;
  };

  struct VertexPairEqual
  {
    bool operator()(const std::pair< std::string, std::string >&, const std::pair< std::string, std::string >&) const;
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
}

#endif
