#include <ostream>
#include "node-properties.hpp"
#include "node.hpp"
namespace rl {
  std::ostream& operator<<(std::ostream& os, const Size& size)
  {
    if (size.type == SizeType::Auto) {
      os << "Auto";
    } else {
      os << size.value << "px";
    }
    return os;
  }
}
