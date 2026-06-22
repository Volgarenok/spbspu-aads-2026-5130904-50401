#include "hash_functions.hpp"
size_t madieva::StringHash1::operator()(const std::string & key) const {
  size_t hash = 0;
  for (char c : key) {
    hash = hash * 31 + static_cast<size_t>(c);
  }
  return hash;
}

size_t madieva::StringHash2::operator()(const std::string & key) const {
  size_t hash = 0;
  for (char c : key) {
    hash = hash * 131 + static_cast<size_t>(c);
  }
  return hash;
}

bool madieva::StringEqual::operator()(const std::string & a, const std::string & b) const {
  return a == b;
}
