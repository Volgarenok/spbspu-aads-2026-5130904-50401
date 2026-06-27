#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <cstddef>
#include <string>

#include <boost/hash2/sha2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace ulanova
{
  template <class T>
  struct EqualTo
  {
      bool operator()(const T& lhs, const T& rhs) const
      {
        return lhs == rhs;
      }
  };

  template <class T>
  class HmacHash
  {
  public:
    HmacHash():
      hmac_(defaultKey(), defaultKeySize())
    {}

    HmacHash(const unsigned char* key, size_t size):
      hmac_(key, size)
    {}

    size_t operator()(const T& value) const
    {
      boost::hash2::hmac_sha2_256 hmac(hmac_);
      boost::hash2::hash_append(hmac, {}, value);
      return boost::hash2::get_integral_result<size_t>(hmac);
    }
  private:
    boost::hash2::hmac_sha2_256 hmac_;

    static const unsigned char* defaultKey()
    {
      static const unsigned char key[] = {
        0x54, 0x4f, 0x50, 0x49,
        0x54, 0x5f, 0x53, 0x33,
        0x5f, 0x48, 0x4d, 0x41,
        0x43, 0x5f, 0x4b, 0x45,
        0x59, 0x5f, 0x32, 0x30,
        0x32, 0x36, 0x5f, 0x41,
        0x49, 0x50, 0x5f, 0x4c,
        0x41, 0x42, 0x5f, 0x33
      };

      return key;
    }

    static size_t defaultKeySize() noexcept
    {
      return 32;
    }
  };
  struct EdgeKey
  {
    std::string from;
    std::string to;
    unsigned weight;

    EdgeKey():
      from(),
      to(),
      weight(0)
    {}

    EdgeKey(const std::string& edgeFrom, const std::string& edgeTo, unsigned edgeWeight):
      from(edgeFrom),
      to(edgeTo),
      weight(edgeWeight)
    {}

    bool operator==(const EdgeKey& rhs) const
    {
      return from == rhs.from && to == rhs.to && weight == rhs.weight;
    }
  };

  template <class Hash, class Flavor>
  void hash_append(Hash& hash, const Flavor& flavor, const EdgeKey& key)
  {
    boost::hash2::hash_append(hash, flavor, key.from);
    boost::hash2::hash_append(hash, flavor, key.to);
    boost::hash2::hash_append(hash, flavor, key.weight);
  }

  using EdgeKeyHash = HmacHash<EdgeKey>;
  using EdgeKeyEqual = EqualTo<EdgeKey>;

  using StringHash = HmacHash<std::string>;
  using StringEqual = EqualTo<std::string>;
}

#endif
