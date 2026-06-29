#ifndef BSTIT_HPP
#define BSTIT_HPP

#include <utility>
#include "BSTNode.hpp"

namespace donkeev
{
  template< class Key, class Value >
  detail::BSTNode< Key, Value >* fallLeft(detail::BSTNode< Key, Value >*);

  template< class Key, class Value >
  const detail::BSTNode< Key, Value >* fallLeft(const detail::BSTNode< Key, Value >*);

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* fallRight(detail::BSTNode< Key, Value >*);

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* getNextParent(detail::BSTNode< Key, Value >*);

  template< class Key, class Value >
  const detail::BSTNode< Key, Value >* getNextParent(const detail::BSTNode< Key, Value >*);

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator() = default;

    BSTIterator(const BSTIterator< Key, Value >&) = default;
    BSTIterator(BSTIterator< Key, Value >&&) = default;

    ~BSTIterator() = default;

    BSTIterator< Key, Value >& operator=(const BSTIterator< Key, Value >&) = default;
    BSTIterator< Key, Value >& operator=(BSTIterator< Key, Value >&&) = default;

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();
    BSTIterator< Key, Value >& operator++();
    BSTIterator< Key, Value >& operator--();

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
    bool operator==(std::nullptr_t) const noexcept;
    bool operator!=(std::nullptr_t) const noexcept;
  private:
    template< class K, class V, class C >
    friend class BSTree;

    detail::BSTNode< Key, Value >* node_;

    BSTIterator(detail::BSTNode< Key, Value >*);

    detail::BSTNode< Key, Value >* getNode();
  };

  template< class Key, class Value >
  class BSTCIterator
  {
  public:
    BSTCIterator() = default;

    BSTCIterator(const BSTCIterator< Key, Value >&) = default;
    BSTCIterator(BSTCIterator< Key, Value >&&) = default;

    ~BSTCIterator() = default;

    BSTCIterator< Key, Value >& operator=(const BSTCIterator< Key, Value >&) = default;
    BSTCIterator< Key, Value >& operator=(BSTCIterator< Key, Value >&&) = default;

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    BSTCIterator< Key, Value >& operator++();
    BSTCIterator< Key, Value >& operator--();

    bool operator==(const BSTCIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTCIterator< Key, Value >&) const noexcept;
    bool operator==(std::nullptr_t) const noexcept;
    bool operator!=(std::nullptr_t) const noexcept;
  private:
    template< class K, class V, class C >
    friend class BSTree;
    const detail::BSTNode< Key, Value >* node_;

    BSTCIterator(const detail::BSTNode< Key, Value >*);

    const detail::BSTNode< Key, Value >* getNode();
  };

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*()
  {
    return node_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->()
  {
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
  {
    if (!node_)
    {
      return *this;
    }

    if (node_->right_)
    {
      node_ = node_->right_;
      node_ = fallLeft(node_);
    }
    else
    {
      node_ = getNextParent(node_);
    }

    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator--()
  {
    if (!node_)
    {
      return *this;
    }

    if (node_->left_)
    {
      node_ = node_->left_;
      node_ = fallRight(node_);
    }
    else
    {
      node_ = getPrevParent(node_);
    }

    return *this;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator< Key, Value >& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator< Key, Value >& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(std::nullptr_t) const noexcept
  {
    return node_ == nullptr;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(std::nullptr_t) const noexcept
  {
    return node_ != nullptr;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(detail::BSTNode< Key, Value >* nodePtr):
    node_(nodePtr)
  {}

  template< class Key, class Value >
  detail::BSTNode< Key, Value>* BSTIterator< Key, Value >::getNode()
  {
    return node_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& BSTCIterator< Key, Value >::operator*() const
  {
    return node_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* BSTCIterator< Key, Value >::operator->() const
  {
    return std::addressof(node_->data_);
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator++()
  {
    if (!node_)
    {
      return *this;
    }

    if (node_->right_)
    {
      node_ = node_->right_;
      node_ = fallLeft(node_);
    }
    else
    {
      node_ = getNextParent(node_);
    }

    return *this;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator--()
  {
    if (!node_)
    {
      return *this;
    }

    if (node_->left_)
    {
      node_ = node_->left_;
      node_ = fallRight(node_);
    }
    else
    {
      node_ = getPrevParent(node_);
    }

    return *this;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator==(const BSTCIterator< Key, Value >& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator!=(const BSTCIterator< Key, Value >& other) const noexcept
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator==(std::nullptr_t) const noexcept
  {
    return node_ == nullptr;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator!=(std::nullptr_t) const noexcept
  {
    return node_ != nullptr;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(const detail::BSTNode< Key, Value >* nodePtr):
    node_(nodePtr)
  {}

  template< class Key, class Value >
  const detail::BSTNode< Key, Value>* BSTCIterator< Key, Value >::getNode()
  {
    return node_;
  }

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* fallLeft(detail::BSTNode< Key, Value >* nodePtr)
  {
    if (!nodePtr)
    {
      return nullptr;
    }

    while (nodePtr->left_)
    {
      nodePtr = nodePtr->left_;
    }

    return nodePtr;
  }

  template< class Key, class Value >
  const detail::BSTNode< Key, Value >* fallLeft(const detail::BSTNode< Key, Value >* nodePtr)
  {
    if (!nodePtr)
    {
      return nullptr;
    }

    while (nodePtr->left_)
    {
      nodePtr = nodePtr->left_;
    }

    return nodePtr;
  }

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* fallRight(detail::BSTNode< Key, Value >* nodePtr)
  {
    if (!nodePtr)
    {
      return nullptr;
    }

    while (nodePtr->right_)
    {
      nodePtr = nodePtr->right_;
    }

    return nodePtr;
  }

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* getNextParent(detail::BSTNode< Key, Value >* nodePtr)
  {
    while (nodePtr->parent_ && nodePtr == nodePtr->parent_->right_)
    {
      nodePtr = nodePtr->parent_;
    }

    nodePtr = nodePtr->parent_;
    return nodePtr;
  }

  template< class Key, class Value >
  const detail::BSTNode< Key, Value >* getNextParent(const detail::BSTNode< Key, Value >* nodePtr)
  {
    while (nodePtr->parent_ && nodePtr == nodePtr->parent_->right_)
    {
      nodePtr = nodePtr->parent_;
    }

    nodePtr = nodePtr->parent_;
    return nodePtr;
  }

  template< class Key, class Value >
  detail::BSTNode< Key, Value >* getPrevParent(detail::BSTNode< Key, Value >* nodePtr)
  {
    while (nodePtr->parent_ && nodePtr == nodePtr->parent_->left_)
    {
      nodePtr = nodePtr->parent_;
    }

    nodePtr = nodePtr->parent_;
    return nodePtr;
  }

  template< class Key, class Value >
  const detail::BSTNode< Key, Value >* getPrevParent(const detail::BSTNode< Key, Value >* nodePtr)
  {
    while (nodePtr->parent_ && nodePtr == nodePtr->parent_->left_)
    {
      nodePtr = nodePtr->parent_;
    }

    nodePtr = nodePtr->parent_;
    return nodePtr;
  }

}

#endif
