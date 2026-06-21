#include <string>
#include "vector.hpp"
#ifndef DOM
#define DOM
struct El {
  std::string id;
  stuff::Vector< El* > children;
  // TODO стили и размеры
};

struct DomTree {
  El root;
  // TODO: absolute
  // std::vector<El*>
};

struct Database {
  stuff::Vector< DomTree* > history;
  DomTree* current;
};
#endif
