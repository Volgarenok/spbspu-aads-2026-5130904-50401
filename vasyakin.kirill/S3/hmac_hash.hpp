#ifndef HMAC_HASH_HPP
#define HMAC_HASH_HPP

#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/container_hash/hash.hpp>
#include <cstddef>
#include <string>
#include <utility>

namespace vasyakin
{
  class HMACHash
  {
  public:
    HMACHash():
      key_("")
    {}

    explicit HMACHash(std::string key):
      key_(std::move(key))
    {}

    size_t operator()(const std::string& str) const
    {

      boost::hash2::hmac< boost::hash2::sha2_256 > hmac_obj(reinterpret_cast< const unsigned char* >(key_.data()), static_cast< int >(key_.size()));
      boost::hash2::hash_append(hmac_obj, {}, str);

      return boost::hash2::get_integral_result< size_t >(hmac_obj);
    }

    size_t operator()(const std::pair< std::string, std::string >& p) const
    {
      size_t h1 = (*this)(p.first);
      boost::hash_combine(h1, p.second);
      return h1;
    }

  private:
    std::string key_;
  };
}

#endif
