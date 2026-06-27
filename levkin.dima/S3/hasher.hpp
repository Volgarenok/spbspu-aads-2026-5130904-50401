#ifndef HASHER_HPP
#define HASHER_HPP

#include <boost/uuid/detail/sha1.hpp>
#include <cstddef>
#include <string>
#include <utility>

namespace levkin {
  using boost_digest_t = boost::uuids::detail::sha1::digest_type;

  template < class T > struct Sha1Hasher {
    size_t operator()(const T& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(&key, sizeof(T));

      boost_digest_t digest;
      sha1.get_digest(digest);

      if (sizeof(size_t) >= 8) {
        return (static_cast< size_t >(digest[0]) << 32) | digest[1];
      }
      return static_cast< size_t >(digest[0]);
    }
  };

  template <> struct Sha1Hasher< std::string > {
    size_t operator()(const std::string& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(key.data(), key.size());

      boost_digest_t digest;
      sha1.get_digest(digest);

      if (sizeof(size_t) >= 8) {
        return (static_cast< size_t >(digest[0]) << 32) | digest[1];
      }
      return static_cast< size_t >(digest[0]);
    }
  };

  template <> struct Sha1Hasher< std::pair< std::string, std::string > > {
    size_t operator()(const std::pair< std::string, std::string >& key) const
    {
      boost::uuids::detail::sha1 sha1;
      sha1.process_bytes(key.first.data(), key.first.size());
      char separator = '\0';
      sha1.process_bytes(&separator, 1);
      sha1.process_bytes(key.second.data(), key.second.size());

      boost_digest_t digest;
      sha1.get_digest(digest);

      if (sizeof(size_t) >= 8) {
        return (static_cast< size_t >(digest[0]) << 32) | digest[1];
      }
      return static_cast< size_t >(digest[0]);
    }
  };
}

#endif
