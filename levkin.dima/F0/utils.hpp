#ifndef UTILS_RL
#define UTILS_RL

#include "hashtable.hpp"
#include "node.hpp"
#include <string>
#include <iosfwd>

namespace rl {

  template < typename Key, typename Value >
  using Map = levkin::HashTable< Key, Value >;
  struct RLRootNode;

  namespace config {
    extern const float DEFAULT_WIDTH;
    extern const float DEFAULT_HEIGHT;
    extern const char* const DEFAULT_LAYOUT_DIR;
  }

  std::string generateLayoutName(float width, float height);
  std::string generateLayoutPath(const std::string& layoutName);
  void exportToFlatHtml(std::ostream& os, const RLNode& node);
  void saveVisualTree(const RLRootNode*, const std::string&);
  void saveVisualTreeToPng(const RLRootNode*, const std::string&);

}
#endif
