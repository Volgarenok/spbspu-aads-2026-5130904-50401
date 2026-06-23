#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <cstddef>
#include <string>

#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace ulanova
{
  namespace detail
  {
    constexpr unsigned char kDefaultHmacKey[] = {
      0x54, 0x4f, 0x50, 0x49,
      0x54, 0x5f, 0x53, 0x33,
      0x5f, 0x48, 0x4d, 0x41,
      0x43, 0x5f, 0x4b, 0x45,
      0x59, 0x5f, 0x32, 0x30,
      0x32, 0x36, 0x5f, 0x41,
      0x49, 0x50, 0x5f, 0x4c,
      0x41, 0x42, 0x5f, 0x33
    };
    constexpr size_t kDefaultHmacKeySize = 32;
  }

  template< class T >
  struct EqualTo
  {
    bool operator()(const T& lhs, const T& rhs) const;
  };

  template< class T >
  class HmacHash
  {
  public:
    HmacHash();
    HmacHash(const unsigned char* key, size_t size);
    size_t operator()(const T& value) const;
  private:
    boost::hash2::hmac_sha2_256 hmac_;
  };

  using StringHash = HmacHash< std::string >;
  using StringEqual = EqualTo< std::string >;
}


template< class T >
bool ulanova::EqualTo< T >::operator()(const T& lhs, const T& rhs) const
{
  return lhs == rhs;
}

template< class T >
ulanova::HmacHash< T >::HmacHash():
  hmac_(detail::kDefaultHmacKey, detail::kDefaultHmacKeySize)
{}

template< class T >
ulanova::HmacHash< T >::HmacHash(const unsigned char* key, size_t size):
  hmac_(key, size)
{}

template< class T >
size_t ulanova::HmacHash< T >::operator()(const T& value) const
{
  boost::hash2::hmac_sha2_256 hmac(hmac_);
  boost::hash2::hash_append(hmac, {}, value);
  return boost::hash2::get_integral_result< size_t >(hmac);
}

#endif
