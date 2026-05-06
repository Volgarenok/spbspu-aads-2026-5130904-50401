#ifndef HASH_FUNC
#define HASH_FUNC
#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <cstddef>
#include <string>
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
}

#endif
