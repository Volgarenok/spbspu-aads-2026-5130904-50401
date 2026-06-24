#ifndef HMAC_HASH_HPP
#define HMAC_HASH_HPP

#include <cstddef>
#include <string>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace tarasenko
{
  template< class T >
  class HmacHash
  {
  public:
    HmacHash(const std::string& key = "my_key"):
      key_(key)
    {}
    size_t operator()(const T& val) const
    {
      boost::hash2::hmac_sha2_256 hasher(key_.data(), key_.size());
      boost::hash2::hash_append(hasher, {}, val);
      return boost::hash2::get_integral_result< size_t >(hasher);
    }
  private:
    std::string key_;
  };
}

#endif
