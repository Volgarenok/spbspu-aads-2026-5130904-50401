#ifndef HASHERS_HPP
#define HASHERS_HPP

#include <cstddef>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/xxhash.hpp>

namespace chernov {
  struct Hasher1 {
    size_t operator()(const std::string & s) const
    {
      boost::hash2::xxhash_64 h(0);
      boost::hash2::hash_append(h, {}, s);
      return h.result();
    }
  };

  struct Hasher2 {
    size_t operator()(const std::string & s) const
    {
      boost::hash2::fnv1a_64 h;
      boost::hash2::hash_append(h, {}, s);
      return h.result();
    }
  };
}

#endif
