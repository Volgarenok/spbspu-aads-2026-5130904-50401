#include <boost/container_hash/hash.hpp>

#include "robinTableStructs.hpp"

size_t donkeev::StringHash::operator()(const std::string& key) const
{
  boost::hash<std::string> hasher;
  return hasher(key);
}

bool donkeev::StringEqual::operator()(const std::string& a, const std::string& b) const
{
  return a == b;
}

size_t donkeev::SizeTHash::operator()(size_t key) const
{
  boost::hash<size_t> hasher;
  return hasher(key);
}

bool donkeev::SizeTEqual::operator()(size_t a, size_t b) const
{
  return a == b;
}
