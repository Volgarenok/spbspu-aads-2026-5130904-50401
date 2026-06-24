#ifndef ROBINTABLESTRUCTS_HPP
#define ROBINTABLESTRUCTS_HPP

#include <cstddef>
#include <string>

namespace donkeev
{
  struct StringHash
  {
    size_t operator()(const std::string&) const;
  };

  struct StringEqual
  {
    bool operator()(const std::string&, const std::string&) const;
  };

  struct SizeTHash
  {
    size_t operator()(size_t) const;
  };

  struct SizeTEqual
  {
    bool operator()(size_t, size_t) const;
  };
}

#endif
