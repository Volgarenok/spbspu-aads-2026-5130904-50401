#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>
#include <boost/hash2/fnv1a.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace petrov
{
  template < class Key >
  struct KKHash
  {
    size_t seed_;

    KKHash(size_t seed = 0):
      seed_(seed)
    {}

    size_t operator()(const Key& key) const
    {
      boost::hash2::fnv1a_64 hash;
      hash.update(reinterpret_cast< const char* >(&seed_), sizeof(seed_));
      hash.update(reinterpret_cast< const char* >(&key), sizeof(key));

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  template <>
  struct KKHash< std::string >
  {
    size_t seed_;

    KKHash(size_t seed = 0):
      seed_(seed)
    {}

    size_t operator()(const std::string& key) const
    {
      boost::hash2::fnv1a_64 hash;
      hash.update(reinterpret_cast< const char* >(&seed_), sizeof(seed_));
      hash.update(key.data(), key.size());
      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };
}

#endif
