#include <string>
#include "vector.hpp"
#ifndef DOM
#define DOM
struct RLNode {
  std::string id;
  stuff::Vector< RLNode* > children;
  // TODO стили и размеры
};

struct RTRoot {
  RLNode root;
  // TODO: absolute
  // std::vector<El*>
};

#endif
