#ifndef HASHER_HPP
#define HASHER_HPP

#include <boost/hash2/blake2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <cstddef>
#include <cstring>
#include <string>
#include <utility>

namespace lukashevich {
  template< class T >
  struct Blake2Hasher {
    explicit Blake2Hasher(std::size_t seed = 0):
      seed_(seed)
    {}

    std::size_t operator()(const T & value) const
    {
      boost::hash2::blake2b_512 hasher(seed_);
      boost::hash2::hash_append(hasher, {}, value);

      std::size_t result = 0;
      const boost::hash2::blake2b_512::result_type digest = hasher.result();
      std::memcpy(&result, digest.data(), sizeof(result));
      return result;
    }

  private:
    std::size_t seed_;
  };

  using Blake2StringHash = Blake2Hasher< std::string >;

  struct Blake2EdgeHash {
    explicit Blake2EdgeHash(std::size_t seed = 0):
      seed_(seed)
    {}

    std::size_t operator()(const std::pair< std::string, std::string > & edge) const
    {
      boost::hash2::blake2b_512 hasher(seed_);
      boost::hash2::hash_append(hasher, {}, edge.first.size());
      boost::hash2::hash_append(hasher, {}, edge.first);
      boost::hash2::hash_append(hasher, {}, edge.second.size());
      boost::hash2::hash_append(hasher, {}, edge.second);

      std::size_t result = 0;
      const boost::hash2::blake2b_512::result_type digest = hasher.result();
      std::memcpy(&result, digest.data(), sizeof(result));
      return result;
    }

  private:
    std::size_t seed_;
  };
}

#endif
