#ifndef BSTNODE_HPP
#define BSTNODE_HPP

#include <utility>

namespace donkeev
{
  template< class Key, class Value >
  struct BSTNode
  {
    std::pair< const Key, Value > data_;

    BSTNode< Key, Value >* left_;
    BSTNode< Key, Value >* right_;
    BSTNode< Key, Value >* parent_;
  };
}

#endif
