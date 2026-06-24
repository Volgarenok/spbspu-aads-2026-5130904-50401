#ifndef NODE_TREE_HPP
#define NODE_TREE_HPP

#include <utility>

namespace lukashevich
{
  struct TreeNodeBase
  {
    TreeNodeBase * parent_;
    TreeNodeBase * left_;
    TreeNodeBase * right_;

    TreeNodeBase():
      parent_(nullptr),
      left_(nullptr),
      right_(nullptr)
    {}
  };

  template< class Key, class Value >
  struct TreeNode: TreeNodeBase
  {
    std::pair< const Key, Value > value_;

    TreeNode(const Key & key, const Value & value):
      TreeNodeBase(),
      value_(key, value)
    {}
  };
}

#endif
