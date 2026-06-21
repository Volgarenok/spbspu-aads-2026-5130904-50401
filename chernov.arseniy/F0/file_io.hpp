#ifndef FILE_IO_HPP
#define FILE_IO_HPP

#include <string>

namespace chernov {

  class Tree;

  namespace detail {
    bool saveTree(const Tree & tree, const std::string & filename);
    bool loadTree(Tree & tree, const std::string & filename, std::string & errorMsg);
  }
}

#endif
