#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP
#include <stdexcept>
#include <utility>

#include "treeNode.hpp"

namespace zhuravleva
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value >
  class BSTIterator
  {
    friend class BSTConstIterator< Key, Value >;
  public:
    BSTIterator() noexcept;
    explicit BSTIterator(TreeNode< Key, Value >* node) noexcept;
    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;
    BSTIterator& operator++();
    BSTIterator operator++(int);

  private:
    TreeNode< Key, Value >* current_;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator() noexcept:
    current_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(TreeNode< Key, Value >* node) noexcept:
    current_(node)
  {}

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return current_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->() const
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    return &(current_->data_);
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator& other) const noexcept
  {
    return current_ == other.current_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const noexcept
  {
    return current_ != other.current_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
  {
    if (!current_)
    {
      throw std::runtime_error("null iterator");
    }
    if (current_->right_)
    {
      current_ = current_->right_;
      while (current_->left_)
      {
        current_ = current_->left_;
      }
    }
    else
    {
      TreeNode< Key, Value >* parent = current_->parent_;
      while (parent && current_ == parent->right_)
      {
        current_ = parent;
        parent = parent->parent_;
      }
      current_ = parent;
    }
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator< Key, Value > temp(*this);
    ++(*this);
    return temp;
  }
}

#endif
