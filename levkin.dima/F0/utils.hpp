#ifndef UTILS_RL
#define UTILS_RL

#include "hashtable.hpp"
#include "node.hpp"
#include <string>

namespace rl {

  template < typename Key, typename Value >
  using Map = levkin::HashTable< Key, Value >;

  namespace config {
    extern const float DEFAULT_WIDTH;
    extern const float DEFAULT_HEIGHT;
    extern const char* const DEFAULT_LAYOUT_DIR;
  }

  std::string generateLayoutName(float width, float height);
  std::string generateLayoutPath(const std::string& layoutName);
  void exportToFlatHtml(std::ostream& os, const RLNode& node);
}
#endif
