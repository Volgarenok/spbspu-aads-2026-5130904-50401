#ifndef HASHER_HPP
#define HASHER_HPP
#include <cstddef>
#include <utility>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
namespace kuznetsov {

  template< class T >
  struct SipHasher {
    SipHasher(size_t s = 0):
      seed_(s)
    {}
    size_t operator()(const T& p) const
    {
      boost::hash2::siphash_64 h(seed_);
      boost::hash2::hash_append(h, {}, p);
      return h.result();
    }
  private:
    size_t seed_;
  };
}

#endif
