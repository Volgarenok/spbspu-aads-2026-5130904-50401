#ifndef SIPHASH_HPP
#define SIPHASH_HPP

#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <cstddef>
#include <string>
#include <utility>

namespace burukov
{
  template< typename KeyType >
  struct SipHash
  {
    size_t operator()(const KeyType &key) const
    {
      boost::hash2::siphash_64 state;
      boost::hash2::hash_append(state, {}, key);
      return boost::hash2::get_integral_result< size_t >(state);
    }
  };

  struct PairHash
  {
    size_t operator()(const std::pair< std::string, std::string > &pair) const
    {
      boost::hash2::siphash_64 state;
      boost::hash2::hash_append(state, {}, pair.first);
      boost::hash2::hash_append(state, {}, pair.second);
      return boost::hash2::get_integral_result< size_t >(state);
    }
  };
}

#endif
