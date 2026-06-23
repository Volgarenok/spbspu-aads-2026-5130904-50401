#include "hash-func.hpp"
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/get_integral_result.hpp>
#include <boost/hash2/xxhash.hpp>

size_t kondrat::Hash1::operator()(const std::string & value) const
{
  boost::hash2::blake2b_512 hash;
  hash.update(value.data(), value.size());
  return boost::hash2::get_integral_result< size_t >(hash);
}

size_t kondrat::Hash2::operator()(const std::string & value) const
{
  boost::hash2::xxhash_64 hash;
  hash.update(value.data(), value.size());
  return boost::hash2::get_integral_result< size_t >(hash);
}
