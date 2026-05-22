#ifndef BSTITERS_HPP
#define BSTITERS_HPP

#include <utility>
#include "TNode.hpp"

namespace petrov
{
  template< class Key, class Value >
  struct BSTCIterator;

  template <class, class, class>
  struct BSTree;

  template< class Key, class Value >
  struct BSTIterator
  {
    using TNode = detail::TNode< Key, Value >;
    friend struct BSTCIterator<Key, Value>;
    template< class K, class V, class C >
    friend struct BSTree;

    BSTIterator();
    explicit BSTIterator(TNode* node, TNode* fake_leaf);

    BSTIterator& operator++();
    BSTIterator operator++(int);

    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

    std::pair< Key, Value > operator*() const;

  private:
    TNode* node_;
    TNode* fake_leaf_;
  };

  template< class Key, class Value >
  struct BSTCIterator
  {
    using TNode = detail::TNode< Key, Value >;
    template< class K, class V, class C >
    friend struct BSTree;

    BSTCIterator();
    explicit BSTCIterator(const TNode* node, const TNode* fake_leaf);
    BSTCIterator(const BSTIterator<Key, Value>& other);

    BSTCIterator& operator++();
    BSTCIterator operator++(int);

    BSTCIterator& operator--();
    BSTCIterator operator--(int);

    bool operator==(const BSTCIterator& other) const;
    bool operator!=(const BSTCIterator& other) const;

    std::pair< Key, Value > operator*() const;

    private:
      const TNode* node_;
      const TNode* fake_leaf_;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    fake_leaf_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(TNode* node, TNode* fake_leaf):
    node_(node),
    fake_leaf_(fake_leaf)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
  {
    if (node_->right_ != fake_leaf_)
    {
      node_ = node_->right_;

      while (node_->left_ != fake_leaf_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      TNode* parent = node_->parent_;

      while (parent != nullptr && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent ? parent : fake_leaf_;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator--()
  {
    if (node_ == fake_leaf_)
    {
      return *this;
    }

    if (node_->left_ != fake_leaf_)
    {
      node_ = node_->left_;

      while (node_->right_ != fake_leaf_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      TNode* parent = node_->parent_;

      while (parent != nullptr && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent ? parent : fake_leaf_;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator temp = *this;
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator& other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  std::pair< Key, Value > BSTIterator< Key, Value >::operator*() const
  {
    return {node_->key_, node_->value_};
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator():
    node_(nullptr),
    fake_leaf_(nullptr)
  {}

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(const TNode* node, const TNode* fake_leaf):
    node_(node),
    fake_leaf_(fake_leaf)
  {}

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator++()
  {
    if (node_->right_ != fake_leaf_)
    {
      node_ = node_->right_;

      while (node_->left_ != fake_leaf_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      const TNode* parent = node_->parent_;

      while (parent != nullptr && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent ? parent : fake_leaf_;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value > BSTCIterator< Key, Value >::operator++(int)
  {
    BSTCIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >& BSTCIterator< Key, Value >::operator--()
  {
    if (node_ == fake_leaf_)
    {
      return *this;
    }

    if (node_->left_ != fake_leaf_)
    {
      node_ = node_->left_;

      while (node_->right_ != fake_leaf_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      const TNode* parent = node_->parent_;

      while (parent != nullptr && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent ? parent : fake_leaf_;
    }

    return *this;
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value > BSTCIterator< Key, Value >::operator--(int)
  {
    BSTCIterator temp = *this;
    --(*this);
    return temp;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator==(const BSTCIterator& other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool BSTCIterator< Key, Value >::operator!=(const BSTCIterator& other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  std::pair< Key, Value > BSTCIterator< Key, Value >::operator*() const
  {
    return {node_->key_, node_->value_};
  }

  template< class Key, class Value >
  BSTCIterator< Key, Value >::BSTCIterator(const BSTIterator< Key, Value >& other) :
    node_(other.node_),
    fake_leaf_(other.fake_leaf_)
  {}
}

#endif
