#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP
#include <utility>
#include <memory>
#include "treenode.hpp"

namespace sedov
{
  template < class Key, class Value, class Compare >
  class BSTree;

  template < class Key, class Value >
  class BSTConstIterator
  {
  public:
    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;
    BSTConstIterator & operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator & operator--();
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator & other) const;
    bool operator!=(const BSTConstIterator & other) const;

    template < class K, class V, class C >
    friend class BSTree;

  private:
    detail::TreeNode< Key, Value > * node_;
    explicit BSTConstIterator(detail::TreeNode< Key, Value > * node = nullptr);
  };

  template < class Key, class Value >
  class BSTIterator
  {
  public:
    std::pair< Key, Value > & operator*() const;
    std::pair< Key, Value > * operator->() const;
    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);
    bool operator==(const BSTIterator & other) const;
    bool operator!=(const BSTIterator & other) const;

    template < class K, class V, class C >
    friend class BSTree;

  private:
    detail::TreeNode< Key, Value > * node_;
    explicit BSTIterator(detail::TreeNode< Key, Value > * node = nullptr);
  };
}

template < class Key, class Value >
sedov::BSTConstIterator< Key, Value >::BSTConstIterator(detail::TreeNode< Key, Value > * node):
  node_(node)
{}

template < class Key, class Value >
const std::pair< Key, Value > & sedov::BSTConstIterator< Key, Value >::operator*() const
{
  return node_->data;
}

template < class Key, class Value >
const std::pair< Key, Value > * sedov::BSTConstIterator< Key, Value >::operator->() const
{
  return std::addressof(node_->data);
}

template < class Key, class Value >
sedov::BSTConstIterator< Key, Value > & sedov::BSTConstIterator< Key, Value >::operator++()
{
  if (node_->isFake())
  {
    return *this;
  }
  if (!node_->right->isFake())
  {
    node_ = node_->right;
    while (!node_->left->isFake())
    {
      node_ = node_->left;
    }
  }
  else
  {
    detail::TreeNode< Key, Value > * parent = node_->parent;
    while (!parent->isFake() && node_ == parent->right)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
    if (node_->isFake())
    {
      node_ = nullptr;
    }
  }
  return *this;
}

template < class Key, class Value >
sedov::BSTConstIterator< Key, Value > sedov::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value >
sedov::BSTConstIterator< Key, Value > & sedov::BSTConstIterator< Key, Value >::operator--()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  if (!node_->left->isFake())
  {
    node_ = node_->left;
    while (!node_->right->isFake())
    {
      node_ = node_->right;
    }
  }
  else
  {
    detail::TreeNode< Key, Value > * parent = node_->parent;
    while (!parent->isFake() && node_ == parent->left)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
sedov::BSTConstIterator< Key, Value > sedov::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator tmp = *this;
  --(*this);
  return tmp;
}

template < class Key, class Value >
bool sedov::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator & other) const
{
  return node_ == other.node_;
}

template < class Key, class Value >
bool sedov::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator & other) const
{
  return node_ != other.node_;
}

template < class Key, class Value >
sedov::BSTIterator< Key, Value >::BSTIterator(detail::TreeNode< Key, Value > * node):
  node_(node)
{}

template < class Key, class Value >
std::pair< Key, Value > & sedov::BSTIterator< Key, Value >::operator*() const
{
  return const_cast< std::pair< Key, Value > & >(node_->data);
}

template < class Key, class Value >
std::pair< Key, Value > * sedov::BSTIterator< Key, Value >::operator->() const
{
  return const_cast< std::pair< Key, Value > * >(std::addressof(node_->data));
}

template < class Key, class Value >
sedov::BSTIterator< Key, Value > & sedov::BSTIterator< Key, Value >::operator++()
{
  if (node_->isFake())
  {
    return *this;
  }
  if (!node_->right->isFake())
  {
    node_ = node_->right;
    while (!node_->left->isFake())
    {
      node_ = node_->left;
    }
  }
  else
  {
    detail::TreeNode< Key, Value > * parent = node_->parent;
    while (!parent->isFake() && node_ == parent->right)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
    if (node_->isFake())
    {
      node_ = nullptr;
    }
  }
  return *this;
}

template < class Key, class Value >
sedov::BSTIterator< Key, Value > sedov::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template < class Key, class Value >
sedov::BSTIterator< Key, Value > & sedov::BSTIterator< Key, Value >::operator--()
{
  if (node_ == nullptr)
  {
    return *this;
  }
  if (!node_->left->isFake())
  {
    node_ = node_->left;
    while (!node_->right->isFake())
    {
      node_ = node_->right;
    }
  }
  else
  {
    detail::TreeNode< Key, Value > * parent = node_->parent;
    while (!parent->isFake() && node_ == parent->left)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
}

template < class Key, class Value >
sedov::BSTIterator< Key, Value > sedov::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator tmp = *this;
  --(*this);
  return tmp;
}

template < class Key, class Value >
bool sedov::BSTIterator< Key, Value >::operator==(const BSTIterator & other) const
{
  return node_ == other.node_;
}

template < class Key, class Value >
bool sedov::BSTIterator< Key, Value >::operator!=(const BSTIterator & other) const
{
  return node_ != other.node_;
}

#endif
