#ifndef AVLTREENODE_HPP
#define AVLTREENODE_HPP
#include <utility>

namespace sedov
{
  template< class Key, class Value >
  class AVLTree;

  template< class Key, class Value >
  class AVLTreeNode
  {
  private:
      Key key_;
      Value value_;
      AVLTreeNode * left_;
      AVLTreeNode * right_;
      AVLTreeNode * parent_;
      int height_;

      void updateHeight() noexcept;
      int balanceFactor() const noexcept;
      explicit AVLTreeNode(const Key & k, const Value & v, AVLTreeNode * parent);
      explicit AVLTreeNode(Key && k, Value && v, AVLTreeNode * parent);

      template< class K, class V, class C >
      friend class AVLTree;
  };

  template < class Key, class Value >
  AVLTreeNode< Key, Value >::AVLTreeNode(const Key & key, const Value & value, AVLTreeNode< Key, Value > * parent):
    key_(key),
    value_(value),
    left_(nullptr),
    right_(nullptr),
    parent_(parent),
    height_(1)
  {}

  template < class Key, class Value >
  AVLTreeNode< Key, Value >::AVLTreeNode(Key && key, Value && value, AVLTreeNode< Key, Value > * parent):
    key_(std::move(key)),
    value_(std::move(value)),
    left_(nullptr),
    right_(nullptr),
    parent_(parent),
    height_(1)
  {}

  template < class Key, class Value >
  void AVLTreeNode< Key, Value >::updateHeight() noexcept
  {
    int leftHeight = (left_ ? left_->height_ : 0);
    int rightHeight = (right_ ? right_->height_ : 0);
    height_ = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }

  template < class Key, class Value >
  int AVLTreeNode< Key, Value >::balanceFactor() const noexcept
  {
    int leftHeight = (left_ ? left_->height_ : 0);
    int rightHeight = (right_ ? right_->height_ : 0);
    return leftHeight - rightHeight;
  }
}

#endif
