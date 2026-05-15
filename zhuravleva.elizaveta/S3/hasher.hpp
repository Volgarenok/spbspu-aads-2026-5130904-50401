#ifndef HASHER_HPP
#define HASHER_HPP

#include <cstddef>
#include <string>
#include <utility>
#include <boost/hash2/blake2.hpp>

namespace zhuravleva
{
  struct StringHash
  {
    size_t operator()(const std::string& str) const;
  };
  struct StringEqual
  {
    bool operator()(const std::string& a, const std::string& b) const;
  };
  struct PairHash
  {
    size_t operator()(const std::pair< std::string, std::string >& p) const;
  };
  struct PairEqual
  {
    bool operator()(const std::pair< std::string, std::string >& a,
        const std::pair< std::string, std::string >& b) const;
  };
}

#endif