#ifndef CBST_ITER_HPP
#define CBST_ITER_HPP

#include <cassert>
#include <memory>
#include "bst-node.hpp"

namespace kondrat
{
  template< class Key, class Value >
  class CBSTIterator
  {
  public:
    CBSTIterator();

    CBSTIterator & operator++();
    CBSTIterator operator++(int);
    CBSTIterator & operator--();
    CBSTIterator operator--(int);

    bool operator==(const CBSTIterator & rhs) const noexcept;
    bool operator!=(const CBSTIterator & rhs) const noexcept;

    const std::pair< Key, Value > & operator*() const;
    const std::pair< Key, Value > * operator->() const;

  private:
    template< class K, class V, class C >
    friend class BSTree;
    CBSTIterator(detail::Node< Key, Value > * node, detail::Node< Key, Value > * root) noexcept;
    detail::Node< Key, Value > * getMin(detail::Node< Key, Value > * node) const noexcept;
    detail::Node< Key, Value > * getMax(detail::Node< Key, Value > * node) const noexcept;

    detail::Node< Key, Value > * node_;
    detail::Node< Key, Value > * root_;
  };

  template< class Key, class Value >
  CBSTIterator< Key, Value >::CBSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  CBSTIterator< Key, Value > & CBSTIterator< Key, Value >::operator++()
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
  CBSTIterator< Key, Value > CBSTIterator< Key, Value >::operator++(int)
  {
    CBSTIterator temp(*this);
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value > & CBSTIterator< Key, Value >::operator--()
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
  CBSTIterator< Key, Value > CBSTIterator< Key, Value >::operator--(int)
  {
    CBSTIterator temp(*this);
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool CBSTIterator< Key, Value >::operator==(const CBSTIterator & rhs) const noexcept
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool CBSTIterator< Key, Value >::operator!=(const CBSTIterator & rhs) const noexcept
  {
    return node_ != rhs.node_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value > & CBSTIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value > * CBSTIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  CBSTIterator< Key, Value >::CBSTIterator(detail::Node< Key, Value > * node, detail::Node< Key, Value > * root) noexcept:
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  detail::Node< Key, Value > * CBSTIterator< Key, Value >::getMin(detail::Node< Key, Value > * node) const noexcept
  {
    while (node && node->left_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value >
  detail::Node< Key, Value > * CBSTIterator< Key, Value >::getMax(detail::Node< Key, Value > * node) const noexcept
  {
    while (node && node->right_)
    {
      node = node->right_;
    }

    return node;
  }
}

#endif
