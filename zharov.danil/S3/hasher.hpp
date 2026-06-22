#ifndef HASHER_HPP
#define HASHER_HPP
#include <cstddef>
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/hash_append.hpp>

namespace zharov
{
  template< class T >
  struct Blake2Hasher
  {
    Blake2Hasher(std::size_t s = 0);
    std::size_t operator()(const T& value) const;

  private:
    std::size_t seed_;
  };
}

template< class T >
zharov::Blake2Hasher< T >::Blake2Hasher(std::size_t s):
  seed_(s)
{}

template< class T >
std::size_t zharov::Blake2Hasher< T >::operator()(const T& value) const
{
  boost::hash2::blake2b_512 h(seed_);
  boost::hash2::hash_append(h, {}, value);
  return *reinterpret_cast< const std::size_t* >(h.result().data());
}

#endif
