#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>
namespace zhuravleva
{
  template< class Key, class Value >
  struct TreeNode
  {
    std::pair< Key, Value > data_;
    TreeNode< Key, Value >* parent_;
    TreeNode< Key, Value >* left_;
    TreeNode< Key, Value >* right_;

    TreeNode(const Key& key, const Value& value,
      TreeNode< Key, Value >* parent = nullptr):
      data_(key, value),
      parent_(parent),
      left_(nullptr),
      right_(nullptr)
    {}
  };
}

#endif
