#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP
#include <memory>
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
  public:
    BSTIterator() noexcept;
    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    bool operator==(const BSTIterator& other) const noexcept;
    bool operator!=(const BSTIterator& other) const noexcept;
    BSTIterator& operator++();
    BSTIterator operator++(int);

  private:
    template< class K, class V, class C >
    friend class BSTree;
    friend class BSTConstIterator< Key, Value >;
    explicit BSTIterator(detail::TreeNode< Key, Value >* node) noexcept;
    detail::TreeNode< Key, Value >* current_;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    BSTConstIterator() noexcept;
    BSTConstIterator(const BSTIterator< Key, Value >& other) noexcept;
    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    bool operator==(const BSTConstIterator& other) const noexcept;
    bool operator!=(const BSTConstIterator& other) const noexcept;
    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

  private:
    template< class K, class V, class C >
    friend class BSTree;
    explicit BSTConstIterator(const detail::TreeNode< Key, Value >* node) noexcept;
    const detail::TreeNode< Key, Value >* current_;
  };
}

template< class Key, class Value >
zhuravleva::BSTIterator< Key, Value >::BSTIterator() noexcept:
  current_(nullptr)
{}

template< class Key, class Value >
zhuravleva::BSTIterator< Key, Value >::BSTIterator(
    zhuravleva::detail::TreeNode< Key, Value >* node) noexcept:
  current_(node)
{}

template< class Key, class Value >
std::pair< Key, Value >& zhuravleva::BSTIterator< Key, Value >::operator*()
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  return current_->data;
}

template< class Key, class Value >
std::pair< Key, Value >* zhuravleva::BSTIterator< Key, Value >::operator->()
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  return std::addressof(current_->data);
}

template< class Key, class Value >
bool zhuravleva::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const noexcept
{
  return current_ == other.current_;
}

template< class Key, class Value >
bool zhuravleva::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const noexcept
{
  return current_ != other.current_;
}

template< class Key, class Value >
zhuravleva::BSTIterator< Key, Value >& zhuravleva::BSTIterator< Key, Value >::operator++()
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  if (current_->right)
  {
    current_ = current_->right;
    while (current_->left)
    {
      current_ = current_->left;
    }
  }
  else
  {
    zhuravleva::detail::TreeNode< Key, Value >* parent = current_->parent;
    while (parent && current_ == parent->right)
    {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template< class Key, class Value >
zhuravleva::BSTIterator< Key, Value > zhuravleva::BSTIterator< Key, Value >::operator++(int)
{
  zhuravleva::BSTIterator< Key, Value > temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
zhuravleva::BSTConstIterator< Key, Value >::BSTConstIterator() noexcept:
  current_(nullptr)
{}

template< class Key, class Value >
zhuravleva::BSTConstIterator< Key, Value >::BSTConstIterator(const zhuravleva::detail::TreeNode< Key, Value >* node) noexcept:
  current_(node)
{}

template< class Key, class Value >
zhuravleva::BSTConstIterator< Key, Value >::BSTConstIterator(const zhuravleva::BSTIterator< Key, Value >& other) noexcept:
  current_(other.current_)
{}

template< class Key, class Value >
const std::pair< Key, Value >& zhuravleva::BSTConstIterator< Key, Value >::operator*() const
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  return current_->data;
}

template< class Key, class Value >
const std::pair< Key, Value >* zhuravleva::BSTConstIterator< Key, Value >::operator->() const
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  return std::addressof(current_->data);
}

template< class Key, class Value >
bool zhuravleva::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& other) const noexcept
{
  return current_ == other.current_;
}

template< class Key, class Value >
bool zhuravleva::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& other) const noexcept
{
  return current_ != other.current_;
}

template< class Key, class Value >
zhuravleva::BSTConstIterator< Key, Value >& zhuravleva::BSTConstIterator< Key, Value >::operator++()
{
  if (!current_)
  {
    throw std::runtime_error("null iterator");
  }
  if (current_->right)
  {
    current_ = current_->right;
    while (current_->left)
    {
      current_ = current_->left;
    }
  }
  else
  {
    const zhuravleva::detail::TreeNode< Key, Value >* parent = current_->parent;
    while (parent && current_ == parent->right)
    {
      current_ = parent;
      parent = parent->parent;
    }
    current_ = parent;
  }
  return *this;
}

template< class Key, class Value >
zhuravleva::BSTConstIterator< Key, Value > zhuravleva::BSTConstIterator< Key, Value >::operator++(int)
{
  zhuravleva::BSTConstIterator< Key, Value > temp(*this);
  ++(*this);
  return temp;
}

#endif
