#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <cstddef>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>

namespace hvostov {
  template< class T >
  struct SipHash {
    size_t operator()(const T& key) const
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, key);
      return static_cast< size_t >(hasher.result());
    }
  };
}
#endif
