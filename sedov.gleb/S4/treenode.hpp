#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace sedov
{
  template < class Key, class Value >
  struct TreeNode
  {
    Key key_;
    Value value_;
    TreeNode * left_;
    TreeNode * right_;
    TreeNode * parent_;
    
    static TreeNode * fakeLeaf;

    TreeNode(const Key & key, const Value & value, TreeNode * parent);
    TreeNode(Key && key, Value && value, TreeNode * parent);
    
    bool isFake() const noexcept;
  };
  
  template < class Key, class Value >
  TreeNode< Key, Value > * TreeNode< Key, Value >::fakeLeaf = nullptr;
}

template < class Key, class Value >
sedov::TreeNode< Key, Value >::TreeNode(const Key & key, const Value & value, sedov::TreeNode< Key, Value > * parent):
  key_(key),
  value_(value),
  left_(fakeLeaf),
  right_(fakeLeaf),
  parent_(parent)
{}

template < class Key, class Value >
sedov::TreeNode< Key, Value >::TreeNode(Key && key, Value && value, sedov::TreeNode< Key, Value > * parent):
  key_(std::move(key)),
  value_(std::move(value)),
  left_(fakeLeaf),
  right_(fakeLeaf),
  parent_(parent)
{}

template < class Key, class Value >
bool sedov::TreeNode< Key, Value >::isFake() const noexcept
{
  return this == fakeLeaf;
}

#endif
