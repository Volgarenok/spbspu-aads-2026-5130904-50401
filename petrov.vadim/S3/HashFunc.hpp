#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>
#include <utility>

#include <boost/hash2/sha1.hpp>
#include <boost/hash2/get_integral_result.hpp>

namespace petrov
{
  using EdgeKey = std::pair< std::string, std::string >;

  struct sha1
  {
    size_t operator()(const std::string & str) const
    {
      boost::hash2::sha1_160 hash;

      hash.update(str.data(), str.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };

  struct edgeSha1
  {
    size_t operator()(const EdgeKey & edge) const
    {
      boost::hash2::sha1_160 hash;

      hash.update(edge.first.data(), edge.first.size());

      const char sep = '\0';
      hash.update(&sep, 1);

      hash.update(edge.second.data(), edge.second.size());

      return boost::hash2::get_integral_result< size_t >(hash);
    }
  };
}

#endif
