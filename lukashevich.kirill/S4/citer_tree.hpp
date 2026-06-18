#ifndef CITER_TREE_HPP
#define CITER_TREE_HPP

#include <stdexcept>
#include <utility>

#include "iter_tree.hpp"

namespace lukashevich
{
  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    BSTConstIterator();
    BSTConstIterator(const BSTIterator< Key, Value > & other);

    const std::pair< const Key, Value > & operator*() const;
    const std::pair< const Key, Value > * operator->() const;

    BSTConstIterator & operator++();
    BSTConstIterator operator++(int);

    BSTConstIterator & operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator & other) const;
    bool operator!=(const BSTConstIterator & other) const;

    bool operator==(const BSTIterator< Key, Value > & other) const;
    bool operator!=(const BSTIterator< Key, Value > & other) const;

  private:
    const TreeNodeBase * node_;
    const TreeNodeBase * fakeRoot_;

    BSTConstIterator(
        const TreeNodeBase * node,
        const TreeNodeBase * fakeRoot);

    void checkDereference() const;
    void increment();
    void decrement();

    template< class TreeKey, class TreeValue, class TreeCompare >
    friend class BSTree;

    friend class BSTIterator< Key, Value >;
  };

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator():
    node_(nullptr),
    fakeRoot_(nullptr)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(
      const BSTIterator< Key, Value > & other):
    node_(other.node_),
    fakeRoot_(other.fakeRoot_)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(
      const TreeNodeBase * node,
      const TreeNodeBase * fakeRoot):
    node_(node),
    fakeRoot_(fakeRoot)
  {}

  template< class Key, class Value >
  const std::pair< const Key, Value > &
  BSTConstIterator< Key, Value >::operator*() const
  {
    checkDereference();
    return static_cast< const TreeNode< Key, Value > * >(
        node_)->value_;
  }

  template< class Key, class Value >
  const std::pair< const Key, Value > *
  BSTConstIterator< Key, Value >::operator->() const
  {
    return &operator*();
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > &
  BSTConstIterator< Key, Value >::operator++()
  {
    increment();
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >
  BSTConstIterator< Key, Value >::operator++(int)
  {
    BSTConstIterator result(*this);
    increment();
    return result;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > &
  BSTConstIterator< Key, Value >::operator--()
  {
    decrement();
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >
  BSTConstIterator< Key, Value >::operator--(int)
  {
    BSTConstIterator result(*this);
    decrement();
    return result;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(
      const BSTConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(
      const BSTConstIterator & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(
      const BSTIterator< Key, Value > & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(
      const BSTIterator< Key, Value > & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  void BSTConstIterator< Key, Value >::checkDereference() const
  {
    if (node_ == nullptr || fakeRoot_ == nullptr)
    {
      throw std::logic_error("invalid iterator");
    }

    if (node_ == fakeRoot_)
    {
      throw std::out_of_range("cant end iterator");
    }
  }

  template< class Key, class Value >
  void BSTConstIterator< Key, Value >::increment()
  {
    if (node_ == nullptr || node_ == fakeRoot_)
    {
      return;
    }

    if (node_->right_ != nullptr)
    {
      node_ = node_->right_;

      while (node_->left_ != nullptr)
      {
        node_ = node_->left_;
      }

      return;
    }

    const TreeNodeBase * parent = node_->parent_;

    while (parent != fakeRoot_ && node_ == parent->right_)
    {
      node_ = parent;
      parent = parent->parent_;
    }

    node_ = parent;
  }

  template< class Key, class Value >
  void BSTConstIterator< Key, Value >::decrement()
  {
    if (node_ == nullptr || fakeRoot_ == nullptr)
    {
      return;
    }

    if (node_ == fakeRoot_)
    {
      node_ = fakeRoot_->parent_;

      if (node_ == fakeRoot_)
      {
        return;
      }

      while (node_->right_ != nullptr)
      {
        node_ = node_->right_;
      }

      return;
    }

    if (node_->left_ != nullptr)
    {
      node_ = node_->left_;

      while (node_->right_ != nullptr)
      {
        node_ = node_->right_;
      }

      return;
    }

    const TreeNodeBase * parent = node_->parent_;

    while (parent != fakeRoot_ && node_ == parent->left_)
    {
      node_ = parent;
      parent = parent->parent_;
    }

    node_ = parent;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(
      const BSTConstIterator< Key, Value > & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(
      const BSTConstIterator< Key, Value > & other) const
  {
    return !(*this == other);
  }
}

#endif
