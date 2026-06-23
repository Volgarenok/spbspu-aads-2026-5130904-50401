#ifndef HASH_FUNC_HPP
#define HASH_FUNC_HPP

#include <cstddef>
#include <string>

namespace kondrat
{
  struct Hash1
  {
    size_t operator()(const std::string & value) const;
  };

  struct Hash2
  {
    size_t operator()(const std::string & value) const;
  };
}

#endif
