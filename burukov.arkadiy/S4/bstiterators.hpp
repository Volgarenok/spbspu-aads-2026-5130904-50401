#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP

#include <utility>
#include "treenode.hpp"

namespace burukov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  namespace detail
  {
    template< class Key, class Value >
    class BSTIterator
    {
    public:
      using Node = TreeNode< Key, Value >;

      explicit BSTIterator(Node* node = nullptr);

      std::pair< const Key, Value > operator*() const;

      BSTIterator& operator++();
      BSTIterator operator++(int);
      BSTIterator& operator--();
      BSTIterator operator--(int);

      bool operator==(const BSTIterator& other) const;
      bool operator!=(const BSTIterator& other) const;

      Node* getNode() const { return node_; }

    private:
      Node* node_;

      template< class K, class V, class C >
      friend class BSTree;
    };
  }
}

template< class Key, class Value >
burukov::detail::BSTIterator< Key, Value >::BSTIterator(Node* node) :
  node_(node)
{}

template< class Key, class Value >
std::pair< const Key, Value >
burukov::detail::BSTIterator< Key, Value >::operator*() const
{
  return {node_->key_, node_->value_};
}

template< class Key, class Value >
burukov::detail::BSTIterator< Key, Value >&
burukov::detail::BSTIterator< Key, Value >::operator++()
{
  if (node_ == nullptr || node_->isFake())
  {
    node_ = nullptr;
    return *this;
  }

  if (!node_->right_->isFake())
  {
    node_ = node_->right_;
    while (node_ != nullptr && !node_->left_->isFake())
    {
      node_ = node_->left_;
    }
  }
  else
  {
    Node* p = node_->parent_;
    while (p != nullptr && !p->isFake() && node_ == p->right_)
    {
      node_ = p;
      p = p->parent_;
    }
    node_ = p;
    if (node_ != nullptr && node_->isFake())
    {
      node_ = nullptr;
    }
  }
  return *this;
}

template< class Key, class Value >
burukov::detail::BSTIterator< Key, Value >
burukov::detail::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator tmp = *this;
  ++(*this);
  return tmp;
}

template< class Key, class Value >
burukov::detail::BSTIterator< Key, Value >&
burukov::detail::BSTIterator< Key, Value >::operator--()
{
  if (node_ == nullptr)
  {
    return *this;
  }

  if (!node_->left_->isFake())
  {
    node_ = node_->left_;
    while (node_ != nullptr && !node_->right_->isFake())
    {
      node_ = node_->right_;
    }
  }
  else
  {
    Node* p = node_->parent_;
    while (p != nullptr && !p->isFake() && node_ == p->left_)
    {
      node_ = p;
      p = p->parent_;
    }
    node_ = p;
    if (node_ != nullptr && node_->isFake())
    {
      node_ = nullptr;
    }
  }
  return *this;
}

template< class Key, class Value >
burukov::detail::BSTIterator< Key, Value >
burukov::detail::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator tmp = *this;
  --(*this);
  return tmp;
}

template< class Key, class Value >
bool burukov::detail::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool burukov::detail::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const
{
  return node_ != other.node_;
}

#endif
