#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP
#include <utility>
#include "treenode.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator
  {
  private:
    using Node = vasyakin::Node< Key, Value >;
    Node* node_;
    Node* fake_leaf_;

    template< class K, class V, class C >
    friend class BSTree;

  public:
    BSTIterator();
    explicit BSTIterator(Node* node, Node* fake_leaf);

    std::pair< Key, Value > operator*() const;

    BSTIterator& operator++();
    BSTIterator operator++(int);

    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

    operator BSTConstIterator< Key, Value >() const;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  private:
    using Node = vasyakin::Node< Key, Value >;
    const Node* node_;
    const Node* fake_leaf_;

    template< class K, class V, class C >
    friend class BSTree;

  public:
    BSTConstIterator();
    explicit BSTConstIterator(const Node* node, const Node* fake_leaf);

    const std::pair< Key, Value > operator*() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator& other) const;
    bool operator!=(const BSTConstIterator& other) const;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    fake_leaf_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node* node, Node* fake_leaf):
    node_(node),
    fake_leaf_(fake_leaf)
  {}

  template< class Key, class Value >
  std::pair< Key, Value > BSTIterator< Key, Value >::operator*() const
  {
    return {node_->key_, node_->value_};
  }

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
      Node* parent = node_->parent_;

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
    BSTIterator< Key, Value > temp = *this;
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
      Node* parent = node_->parent_;

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
    BSTIterator< Key, Value > temp = *this;
    --(*this);
    return temp;
  }
}

#endif
