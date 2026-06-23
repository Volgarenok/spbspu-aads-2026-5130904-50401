#ifndef BST_ITER_HPP
#define BST_ITER_HPP

#include <cassert>
#include <memory>
#include "bst-node.hpp"

namespace kondrat
{
  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator();

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & rhs) const noexcept;
    bool operator!=(const BSTIterator & rhs) const noexcept;

    std::pair< Key, Value > & operator*();
    std::pair< Key, Value > * operator->();

  private:
    template< class K, class V, class C >
    friend class BSTree;
    BSTIterator(detail::Node< Key, Value > * node, detail::Node< Key, Value > * root) noexcept;
    detail::Node< Key, Value > * getMin(detail::Node< Key, Value > * node) const noexcept;
    detail::Node< Key, Value > * getMax(detail::Node< Key, Value > * node) const noexcept;

    detail::Node< Key, Value > * node_;
    detail::Node< Key, Value > * root_;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator++()
  {
    assert(node_ != nullptr);

    if (node_->right_)
    {
      node_ = getMin(node_->right_);
    }
    else
    {
      detail::Node< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator--()
  {
    if (node_ == nullptr)
    {
      node_ = getMax(root_);
      return *this;
    }

    if (node_->left_)
    {
      node_ = getMax(node_->left_);
    }
    else
    {
      detail::Node< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator temp(*this);
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator & rhs) const noexcept
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  std::pair< Key, Value > & BSTIterator< Key, Value >::operator*()
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value > * BSTIterator< Key, Value >::operator->()
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(detail::Node< Key, Value > * node, detail::Node< Key, Value > * root) noexcept:
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  detail::Node< Key, Value > * BSTIterator< Key, Value >::getMin(detail::Node< Key, Value > * node) const noexcept
  {
    while (node && node->left_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value >
  detail::Node< Key, Value > * BSTIterator< Key, Value >::getMax(detail::Node< Key, Value > * node) const noexcept
  {
    while (node && node->right_)
    {
      node = node->right_;
    }

    return node;
  }
}
#endif
