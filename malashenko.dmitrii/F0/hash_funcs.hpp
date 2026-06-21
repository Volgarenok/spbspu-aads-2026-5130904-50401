#ifndef HASH_FUNCS_HPP
#define HASH_FUNCS_HPP

#include <string>
#include <cstddef>
#include <utility>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>

namespace malashenko
{
  template< class T >
  class HmacHash {
  public:
    HmacHash();
    HmacHash(std::string key);
    size_t operator()(const T& key) const;
  private:
    std::string key_;
  };


  template< class T >
  HmacHash< T >::HmacHash():
    key_("secret")
  {}

  template< class T >
  HmacHash< T >::HmacHash(std::string key):
    key_(key)
  {}

  template< class T >
  size_t HmacHash< T >::operator()(const T& value) const
  {
    boost::hash2::hmac<boost::hash2::sha2_256> hash(key_.data(), key_.size());
    boost::hash2::hash_append(hash, boost::hash2::default_flavor{}, value);
    size_t out = 0;
    boost::hash2::sha2_256::result_type result = hash.result();
    std::memcpy(&out, result.data(), std::min(sizeof(out), result.size()));
    return out;
  }



  template< class T >
  struct SipHasher {
  public:
    SipHasher();
    SipHasher(size_t s);
    size_t operator()(const T& p) const;
  private:
    size_t seed_;
  };

  template< class T >
  SipHasher< T >::SipHasher():
    seed_(0)
  {}

  template< class T >
  SipHasher< T >::SipHasher(size_t s):
    seed_(s)
  {}

  template< class T >
  size_t SipHasher< T >::operator()(const T& p) const
  {
    boost::hash2::siphash_64 h(seed_);
    boost::hash2::hash_append(h, {}, p);
    return h.result();
  }


}
#endif
