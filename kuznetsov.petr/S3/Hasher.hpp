#ifndef HASHER_HPP
#define HASHER_HPP
#include <cstddef>
#include <utility>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
namespace kuznetsov {

  template< class T >
  struct SipHasher {
    SipHasher(size_t s = 0);
    size_t operator()(const T& p) const;
  private:
    size_t seed_;
  };

  template< class First, class Second >
  struct SipHasher< std::pair< First, Second > > {
    SipHasher(size_t s = 0);

    size_t operator()(const std::pair< First, Second >& p) const;
  private:
    size_t seed_;
  };
}

template< class T >
kuznetsov::SipHasher< T >::SipHasher(size_t s):
  seed_(s)
{}

template< class T >
size_t kuznetsov::SipHasher< T >::operator()(const T& p) const
{
  boost::hash2::siphash_64 h(seed_);
  boost::hash2::hash_append(h, {}, p);
  return h.result();
}

template< class First, class Second >
kuznetsov::SipHasher< std::pair< First, Second > >::SipHasher(size_t s):
  seed_(s)
{}

template< class First, class Second >
size_t kuznetsov::SipHasher< std::pair< First, Second > >::operator()(const std::pair< First, Second >& p) const
{
  boost::hash2::siphash_64 h(seed_);
  boost::hash2::hash_append(h, {}, p.first);
  boost::hash2::hash_append(h, {}, p.second);
  return h.result();
}

#endif
