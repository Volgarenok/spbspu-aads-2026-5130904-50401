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
    template<class K, class V, class C>
    friend class BSTree;

  public:
    BSTIterator();

    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);

    BSTIterator(detail::BSTNode< Key, Value >*);

    ~BSTIterator() = default;

    BSTIterator< Key, Value >& operator=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value >& operator=(BSTIterator< Key, Value >&&);

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    BSTIterator< Key, Value >& operator++();
    BSTIterator< Key, Value >& operator--();

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
  private:
    detail::BSTNode< Key, Value >* node_;

    detail::BSTNode< Key, Value >* getNode();
  };

  template< class Key, class Value >
  class BSTCIterator
  {
  public:
    BSTCIterator();

    BSTCIterator(const BSTCIterator< Key, Value >&);
    BSTCIterator(BSTCIterator< Key, Value >&&);

    BSTCIterator(const detail::BSTNode< Key, Value >*);

    ~BSTCIterator() = default;

    BSTCIterator< Key, Value >& operator=(const BSTCIterator< Key, Value >&);
    BSTCIterator< Key, Value >& operator=(BSTCIterator< Key, Value >&&);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    BSTCIterator< Key, Value >& operator++();
    BSTCIterator< Key, Value >& operator--();

    bool operator==(const BSTCIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTCIterator< Key, Value >&) const noexcept;
  private:
    const detail::BSTNode< Key, Value >* node_;

    const detail::BSTNode< Key, Value >* getNode();
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(const BSTIterator< Key, Value >& other):
    node_(other.node_)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(BSTIterator< Key, Value >&& other):
    node_(other.node_)
  {
    other.node_ = nullptr;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(detail::BSTNode< Key, Value >* nodePtr):
    node_(nodePtr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator=(const BSTIterator< Key, Value >& other)
  {
    node_ = other.node_;
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator=(BSTIterator< Key, Value >&& other)
  {
    node_ = other.node_;
    other.node_ = nullptr;
    return *this;
  }

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*() const
  {
    return node_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->() const
  {
    return &(node_->data_);
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
  detail::BSTNode< Key, Value>* BSTIterator< Key, Value >::getNode()
  {
    return node_;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator():
    node_(nullptr)
  {}

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(const BSTCIterator< Key, Value >& other):
    node_(other.node_)
  {}

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(BSTCIterator< Key, Value >&& other):
    node_(other.node_)
  {
    other.node_ = nullptr;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(const detail::BSTNode< Key, Value >* nodePtr):
    node_(nodePtr)
  {}

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator=(const BSTCIterator< Key, Value >& other)
  {
    node_ = other.node_;
    return *this;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator=(BSTCIterator< Key, Value >&& other)
  {
    node_ = other.node_;
    other.node_ = nullptr;
    return *this;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& BSTCIterator< Key, Value >::operator*() const
  {
    return node_->data_;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* BSTCIterator< Key, Value >::operator->() const
  {
    return &(node_->data_);
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
