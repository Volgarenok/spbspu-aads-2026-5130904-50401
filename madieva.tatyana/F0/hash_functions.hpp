#ifndef HASH_FUNCTIONS_HPP
#define HASH_FUNCTIONS_HPP

#include <string>

namespace madieva
{
  struct StringHash1 {
    size_t operator()(const std::string & key) const;
  };

  struct StringHash2 {
    size_t operator()(const std::string & key) const;
  };

  struct StringEqual {
    bool operator()(const std::string & a, const std::string & b) const;
  };
}

#endif
