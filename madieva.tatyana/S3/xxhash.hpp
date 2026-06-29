#ifndef XXHASH_HPP
#define XXHASH_HPP

#include <boost/hash2/xxhash.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace madieva {
  class XXHash64 {
  public:
    explicit XXHash64(std::uint64_t seed = 0);
    size_t operator()(const std::string & value) const;
    size_t operator()(const std::pair< std::string, std::string > & value) const;
  private:
    std::uint64_t seed_;
  };

  struct PairEqual {
    bool operator()(const std::pair< std::string, std::string > & a,
      const std::pair< std::string, std::string > & b) const;
  };

  struct StringEqual {
    bool operator()(const std::string & a, const std::string & b) const;
  };

  XXHash64::XXHash64(std::uint64_t seed) : seed_(seed)
  {}

  size_t XXHash64::operator()(const std::string & value) const
  {
    boost::hash2::xxhash_64 hasher(seed_);
    boost::hash2::hash_append(hasher, {}, value);
    return boost::hash2::get_integral_result<size_t>(hasher);
  }

  size_t XXHash64::operator()(const std::pair< std::string, std::string >
    & value) const
  {
    boost::hash2::xxhash_64 hasher(seed_);
    boost::hash2::hash_append(hasher, {}, value);
    return boost::hash2::get_integral_result<size_t>(hasher);
  }

  bool PairEqual::operator()(const std::pair< std::string, std::string > & a,
    const std::pair< std::string, std::string > & b) const {
    return a.first == b.first && a.second == b.second;
  }

  bool StringEqual::operator()(const std::string & a,
    const std::string & b) const
  {
    return a == b;
  }

}

#endif
