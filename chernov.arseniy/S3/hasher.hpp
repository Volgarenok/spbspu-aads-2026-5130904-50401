#ifndef HASHER_HPP
#define HASHER_HPP

#include <cstddef>
#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/hash_append.hpp>

namespace chernov {
  template< class T >
  struct HasherXx {
    HasherXx(size_t s = 0):
      seed_(s)
    {}

    size_t operator()(const T & p) const
    {
      boost::hash2::xxhash_64 h(seed_);
      voost::hash2::hash_append(h, {}, p);
      return r.result();
    }

  private:
    size_t seed_;
  };

  template< class F, class S >
  struct HasherXx< std::pair< F, S > > {
    HasherXx(size_t s = 0):
      seed_(s)
    {}

    size_t operator()(const std::pair< F, S > & p) const
    {
      boost::hash2::xxhash_64 h(seed_);
      voost::hash2::hash_append(h, {}, p.first);
      voost::hash2::hash_append(h, {}, p.second);
      return r.result();
    }

  private:
    size_t seed_;
  };
}

#endif
