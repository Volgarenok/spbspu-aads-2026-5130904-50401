#ifndef ITER_TREE_HPP
#define ITER_TREE_HPP

#include <stdexcept>
#include <utility>
#include <memory>


#include "node_tree.hpp"

namespace lukashevich
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator
  {
    template< class, class, class >
    friend class BSTree;

    friend class BSTConstIterator< Key, Value >;

  public:
    BSTIterator();

    std::pair< const Key, Value > & operator*() const;
    std::pair< const Key, Value > * operator->() const;

    BSTIterator & operator++();
    BSTIterator operator++(int);

    BSTIterator & operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator & other) const;
    bool operator!=(const BSTIterator & other) const;

    bool operator==(
        const BSTConstIterator< Key, Value > & other) const;
    bool operator!=(
        const BSTConstIterator< Key, Value > & other) const;

  private:
    TreeNodeBase * node_;
    TreeNodeBase * fakeRoot_;

    BSTIterator(TreeNodeBase * node, TreeNodeBase * fakeRoot);

    void checkDereference() const;
    void increment();
    void decrement();
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    fakeRoot_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(
      TreeNodeBase * node,
      TreeNodeBase * fakeRoot):
    node_(node),
    fakeRoot_(fakeRoot)
  {}

  template< class Key, class Value >
  std::pair< const Key, Value > &
  BSTIterator< Key, Value >::operator*() const
  {
    checkDereference();
    return static_cast< TreeNode< Key, Value > * >(node_)->value_;
  }

  template< class Key, class Value >
  std::pair< const Key, Value > *
  BSTIterator< Key, Value >::operator->() const
  {
    checkDereference();
    return std::addressof(
        static_cast< TreeNode< Key, Value > * >(node_)->value_);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > &
  BSTIterator< Key, Value >::operator++()
  {
    increment();
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >
  BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator result(*this);
    increment();
    return result;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > &
  BSTIterator< Key, Value >::operator--()
  {
    decrement();
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >
  BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator result(*this);
    decrement();
    return result;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(
      const BSTIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(
      const BSTIterator & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  void BSTIterator< Key, Value >::checkDereference() const
  {
    if (node_ == nullptr || fakeRoot_ == nullptr)
    {
      throw std::logic_error("Invalid tree iterator");
    }

    if (node_ == fakeRoot_)
    {
      throw std::out_of_range("Cannot dereference end iterator");
    }
  }

  template< class Key, class Value >
  void BSTIterator< Key, Value >::increment()
  {
    if (node_ == nullptr || fakeRoot_ == nullptr)
    {
      throw std::logic_error("Invalid tree iterator");
    }

    if (node_ == fakeRoot_)
    {
      throw std::out_of_range("Cannot increment end iterator");
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

    TreeNodeBase * parent = node_->parent_;

    while (parent != fakeRoot_ && node_ == parent->right_)
    {
      node_ = parent;
      parent = parent->parent_;
    }

    node_ = parent;
  }

  template< class Key, class Value >
  void BSTIterator< Key, Value >::decrement()
  {
    if (node_ == nullptr || fakeRoot_ == nullptr)
    {
      throw std::logic_error("Invalid tree iterator");
    }

    if (node_ == fakeRoot_)
    {
      if (fakeRoot_->right_ == fakeRoot_)
      {
        throw std::out_of_range(
            "Cannot decrement iterator of empty tree");
      }

      node_ = fakeRoot_->right_;
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

    TreeNodeBase * parent = node_->parent_;

    while (parent != fakeRoot_ && node_ == parent->left_)
    {
      node_ = parent;
      parent = parent->parent_;
    }

    if (parent == fakeRoot_)
    {
      throw std::out_of_range("Cannot decrement begin iterator");
    }

    node_ = parent;
  }
}

#endif
