#ifndef HASHERS_HPP
#define HASHERS_HPP

#include <cstddef>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/hash_append.hpp>

namespace chernov {
  struct Hasher1 {
    std::size_t operator()(const std::string & s) const {
      boost::hash2::xxhash_64 h(0);
      boost::hash2::hash_append(h, {}, s);
      return h.result();
    }
  };

  struct Hasher2 {
    std::size_t operator()(const std::string & s) const {
      boost::hash2::xxhash_64 h(1);
      boost::hash2::hash_append(h, {}, s);
      return h.result();
    }
  };
}

#endif
