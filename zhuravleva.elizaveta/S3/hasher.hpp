#ifndef HASHER_HPP
#define HASHER_HPP

#include <cstddef>
#include <utility>
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace zhuravleva
{
  template< class T >
  struct Blake2Hasher
  {
    size_t operator()(const T& value) const
    {
      boost::hash2::blake2b_512 hash;
      boost::hash2::hash_append(hash, {}, value);
      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  template< class First, class Second >
  struct Blake2Hasher< std::pair< First, Second > >
  {
    size_t operator()(const std::pair< First, Second >& value) const
    {
      boost::hash2::blake2b_512 hash;
      boost::hash2::hash_append(hash, {}, value.first);
      boost::hash2::hash_append(hash, {}, value.second);
      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  struct KeyEqual
  {
    template< class T >
    bool operator()(const T& a, const T& b) const
    {
      return a == b;
    }
  };
}

#endif