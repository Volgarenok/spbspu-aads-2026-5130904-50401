#include "iostream"

namespace donkeev
{
  struct VertexPairHash
  {
    size_t operator()(const std::pair< std::string, std::string >&) const;
  };

  struct VertexPairEqual
  {
    bool operator()(const std::pair< std::string, std::string >&, const std::pair< std::string, std::string >&) const;
  };
}
