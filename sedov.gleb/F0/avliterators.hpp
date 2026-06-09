#ifndef AVLITERATORS_HPP
#define AVLITERATORS_HPP
#include <utility>
#include "avltreenode.hpp"

namespace sedov
{
  template< class Key, class Value, class Compare >
  class AVLTree;

  template< class Key, class Value >
  class AVLConstIterator
  {
  public:
    using valueType = std::pair< const Key, Value >;

    explicit AVLConstIterator(AVLTreeNode< Key, Value > * node = nullptr);
    valueType operator*() const;

    AVLConstIterator & operator++() noexcept;
    AVLConstIterator operator++(int) noexcept;
    AVLConstIterator & operator--() noexcept;
    AVLConstIterator operator--(int) noexcept;
    bool operator==(const AVLConstIterator & other) const noexcept;
    bool operator!=(const AVLConstIterator & other) const noexcept;

  private:
    AVLTreeNode< Key, Value > * node_;

    template< class Key, class Value, class Compare >
    friend class AVLTree;
  };

  template< class Key, class Value >
  class AVLIterator
  {
  public:
    using valueType = std::pair<const Key, Value>;

    explicit AVLIterator(AVLTreeNode< Key, Value > * node = nullptr);
    valueType operator*() const;
    
    AVLIterator & operator++() noexcept;
    AVLIterator operator++(int) noexcept;
    AVLIterator & operator--() noexcept;
    AVLIterator operator--(int) noexcept;
    bool operator==(const AVLIterator & other) const noexcept;
    bool operator!=(const AVLIterator & other) const noexcept;

  private:
    AVLTreeNode< Key, Value > * node_;
    template< class Key, class Value, class Compare >
    friend class AVLTree;
  };

  template < class Key, class Value >
  AVLConstIterator< Key, Value >::AVLConstIterator(AVLTreeNode< Key, Value > * node):
    node_(node)
  {}

  template < class Key, class Value >
  std::pair< const Key, Value > AVLConstIterator< Key, Value >::operator*() const
  {
    return std::pair< const Key, Value >(node_->key_, node_->value_);
  }

  template < class Key, class Value >
  AVLConstIterator< Key, Value > & AVLConstIterator< Key, Value >::operator++() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      AVLTreeNode< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = node_->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template < class Key, class Value >
  AVLConstIterator< Key, Value > AVLConstIterator< Key, Value >::operator++(int) noexcept
  {
    AVLConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template < class Key, class Value >
  AVLConstIterator< Key, Value > & AVLConstIterator< Key, Value >::operator--() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      AVLTreeNode< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = node_->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template < class Key, class Value >
  AVLConstIterator< Key, Value > AVLConstIterator< Key, Value >::operator--(int) noexcept
  {
    AVLConstIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template < class Key, class Value >
  bool AVLConstIterator< Key, Value >::operator==(const AVLConstIterator & other) const noexcept
  {
    return node_ == other.node_;
  }

  template < class Key, class Value >
  bool AVLConstIterator< Key, Value >::operator!=(const AVLConstIterator & other) const noexcept
  {
    return node_ != other.node_;
  }

  template < class Key, class Value >
  AVLIterator< Key, Value >::AVLIterator(AVLTreeNode< Key, Value > * node):
    node_(node)
  {}

  template < class Key, class Value >
  std::pair< const Key, Value > AVLIterator< Key, Value >::operator*() const
  {
    return std::pair< const Key, Value >(node_->key_, node_->value_);
  }

  template < class Key, class Value >
  AVLIterator< Key, Value > & AVLIterator< Key, Value >::operator++() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      AVLTreeNode< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = node_->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template < class Key, class Value >
  AVLIterator< Key, Value > AVLIterator< Key, Value >::operator++(int) noexcept
  {
    AVLIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template < class Key, class Value >
  AVLIterator< Key, Value > & AVLIterator< Key, Value >::operator--() noexcept
  {
    if (!node_)
    {
      return *this;
    }
    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      AVLTreeNode< Key, Value > * parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = node_->parent_;
      }
      node_ = parent;
    }
    return *this;
  }

  template < class Key, class Value >
  AVLIterator< Key, Value > AVLIterator< Key, Value >::operator--(int) noexcept
  {
    AVLIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template < class Key, class Value >
  bool AVLIterator< Key, Value >::operator==(const AVLIterator & other) const noexcept
  {
    return node_ == other.node_;
  }

  template < class Key, class Value >
  bool AVLIterator< Key, Value >::operator!=(const AVLIterator & other) const noexcept
  {
    return node_ != other.node_;
  }
}

#endif
