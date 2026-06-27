#ifndef HMAC_HASH_HPP
#define HMAC_HASH_HPP

#include <boost/hash2/hmac.hpp>
#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

#include <cstddef>
#include <string>
#include <utility>

namespace studilova
{
  class HMACHash
  {
    public:
      HMACHash();

      explicit HMACHash(std::string key);

      size_t operator()(const std::string& value) const;
      size_t operator()(const std::pair< std::string, std::string >& value) const;

    private:
      std::string key_;
  };
}

studilova::HMACHash::HMACHash():
  key_("s3")
{}

explicit studilova::HMACHash::HMACHash(std::string key):
  key_(std::move(key))
{}

size_t studilova::HMACHash::operator()(const std::string& value) const
{
  boost::hash2::hmac< boost::hash2::sha2_256 > hmac(reinterpret_cast< const unsigned char* >(key_.data()),
    static_cast< int >(key_.size())
  );
  boost::hash2::hash_append(hmac, {}, value);
  return boost::hash2::get_integral_result< size_t >(hmac);
}

size_t studilova::HMACHash::operator()(const std::pair< std::string, std::string >& value) const
{
  boost::hash2::hmac< boost::hash2::sha2_256 > hmac(reinterpret_cast< const unsigned char* >(key_.data()),
    static_cast< int >(key_.size())
  );
  boost::hash2::hash_append(hmac, {}, value.first);
  boost::hash2::hash_append(hmac, {}, value.second);
  return boost::hash2::get_integral_result< size_t >(hmac);
}

#endif
