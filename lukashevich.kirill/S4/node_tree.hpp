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
    using value_type = std::pair< const Key, Value >;

    value_type value_;

    TreeNode(const Key & key, const Value & value):
      TreeNodeBase(),
      value_(key, value)
    {}

    explicit TreeNode(const value_type & value):
      TreeNodeBase(),
      value_(value)
    {}
  };
}

#endif
