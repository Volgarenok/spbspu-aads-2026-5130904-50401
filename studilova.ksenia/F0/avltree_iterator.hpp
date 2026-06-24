#ifndef AVLTREE_ITERATORS_HPP
#define AVLTREE_ITERATORS_HPP

#include "avltree_node.hpp"

#include <utility>

namespace studilova
{
  template< class Key, class Value >
  class AVLTreeCIt;

  template< class Key, class Value >
  class AVLTreeIt
  {
    private:
      using Node = studilova::detail::AVLNode< Key, Value >;

    public:
      AVLTreeIt();
      AVLTreeIt(Node* node, Node* root);

      std::pair< const Key&, Value& > operator*() const;

      AVLTreeIt& operator++();
      AVLTreeIt operator++(int);
      AVLTreeIt& operator--();
      AVLTreeIt operator--(int);

      bool operator==(const AVLTreeIt& other) const;
      bool operator!=(const AVLTreeIt& other) const;

      operator AVLTreeCIt< Key, Value >() const;

    private:
      Node* node_;
      Node* root_;

      template< class K, class V, class C >
      friend class AVLTree;

      template< class K, class V >
      friend class AVLTreeCIt;
  };

  template< class Key, class Value >
  class AVLTreeCIt
  {
    private:
      using Node = studilova::detail::AVLNode< Key, Value >;

    public:
      AVLTreeCIt();
      AVLTreeCIt(const Node* node, const Node* root);
      AVLTreeCIt(const AVLTreeIt< Key, Value >& other);

      std::pair< const Key&, const Value& > operator*() const;

      AVLTreeCIt& operator++();
      AVLTreeCIt operator++(int);
      AVLTreeCIt& operator--();
      AVLTreeCIt operator--(int);

      bool operator==(const AVLTreeCIt& other) const;
      bool operator!=(const AVLTreeCIt& other) const;

    private:
      const Node* node_;
      const Node* root_;

      template< class K, class V, class C >
      friend class AVLTree;
  };
}

template< class Key, class Value >
studilova::AVLTreeIt< Key, Value >::AVLTreeIt() :
  node_(nullptr),
  root_(nullptr)
{}

template< class Key, class Value >
studilova::AVLTreeIt< Key, Value >::AVLTreeIt(Node* node, Node* root) :
  node_(node),
  root_(root)
{}

template< class Key, class Value >
std::pair< const Key&, Value& > studilova::AVLTreeIt< Key, Value >::operator*() const
{
  return { node_->key_, node_->value_ };
}

template< class Key, class Value >
studilova::AVLTreeIt< Key, Value >& studilova::AVLTreeIt< Key, Value >::operator++()
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
  } else {
    Node* parent = node_->parent_;
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
studilova::AVLTreeIt< Key, Value > studilova::AVLTreeIt< Key, Value >::operator++(int)
{
  AVLTreeIt temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
studilova::AVLTreeIt< Key, Value >& studilova::AVLTreeIt< Key, Value >::operator--()
{
  if (!node_)
  {
    node_ = root_;
    while (node_ && node_->right_)
    {
      node_ = node_->right_;
    }
    return *this;
  }

  if (node_->left_)
  {
    node_ = node_->left_;
    while (node_->right_)
    {
      node_ = node_->right_;
    }
  } else {
    Node* parent = node_->parent_;
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
studilova::AVLTreeIt< Key, Value > studilova::AVLTreeIt< Key, Value >::operator--(int)
{
  AVLTreeIt temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool studilova::AVLTreeIt< Key, Value >::operator==(const AVLTreeIt& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool studilova::AVLTreeIt< Key, Value >::operator!=(const AVLTreeIt& other) const
{
  return !(*this == other);
}

template< class Key, class Value >
studilova::AVLTreeIt< Key, Value >::operator AVLTreeCIt< Key, Value >() const
{
  return AVLTreeCIt< Key, Value >(node_, root_);
}

template< class Key, class Value >
studilova::AVLTreeCIt< Key, Value >::AVLTreeCIt() :
  node_(nullptr),
  root_(nullptr)
{}

template< class Key, class Value >
studilova::AVLTreeCIt< Key, Value >::AVLTreeCIt(const Node* node, const Node* root) :
  node_(node),
  root_(root)
{}

template< class Key, class Value >
studilova::AVLTreeCIt< Key, Value >::AVLTreeCIt(const AVLTreeIt< Key, Value >& other) :
  node_(other.node_),
  root_(other.root_)
{}

template< class Key, class Value >
std::pair< const Key&, const Value& > studilova::AVLTreeCIt< Key, Value >::operator*() const
{
  return { node_->key_, node_->value_ };
}

template< class Key, class Value >
studilova::AVLTreeCIt< Key, Value >& studilova::AVLTreeCIt< Key, Value >::operator++()
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
  } else {
    const Node* parent = node_->parent_;
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
studilova::AVLTreeCIt< Key, Value > studilova::AVLTreeCIt< Key, Value >::operator++(int)
{
  AVLTreeCIt temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
studilova::AVLTreeCIt< Key, Value >& studilova::AVLTreeCIt< Key, Value >::operator--()
{
  if (!node_)
  {
    node_ = root_;
    while (node_ && node_->right_)
    {
      node_ = node_->right_;
    }
    return *this;
  }

  if (node_->left_)
  {
    node_ = node_->left_;
    while (node_->right_)
    {
      node_ = node_->right_;
    }
  } else {
    const Node* parent = node_->parent_;
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
studilova::AVLTreeCIt< Key, Value > studilova::AVLTreeCIt< Key, Value >::operator--(int)
{
  AVLTreeCIt temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool studilova::AVLTreeCIt< Key, Value >::operator==(const AVLTreeCIt& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool studilova::AVLTreeCIt< Key, Value >::operator!=(const AVLTreeCIt& other) const
{
  return !(*this == other);
}

#endif
