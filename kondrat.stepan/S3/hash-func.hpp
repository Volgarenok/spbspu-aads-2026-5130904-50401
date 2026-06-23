#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace kondrat
{
  using EdgeKey = std::pair< std::string, std::string >;

  struct blake2
  {
    size_t operator()(const std::string & str) const
    {
      boost::hash2::blake2b_512 hash;
      hash.update(str.data(), str.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  struct edgeBlake2
  {
    size_t operator()(const EdgeKey & edge) const
    {
      boost::hash2::blake2b_512 hash;
      hash.update(edge.first.data(), edge.first.size());
      const char sep = '\0';
      hash.update(std::addressof(sep), 1);
      hash.update(edge.second.data(), edge.second.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };
}

#endif
