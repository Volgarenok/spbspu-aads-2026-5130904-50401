#ifndef BSTREE_ITERATORS_HPP
#define BSTREE_ITERATORS_HPP

#include "bstree_node.hpp"

#include <utility>

namespace studilova
{
  template< class Key, class Value >
  class BSTreeCIt;

  template< class Key, class Value >
  class BSTreeIt
  {
    private:
      using Node = studilova::Node< Key, Value >;

    public:
      BSTreeIt();
      BSTreeIt(Node* node, Node* root);

      std::pair< const Key&, Value& > operator*() const;

      BSTreeIt& operator++();
      BSTreeIt operator++(int);
      BSTreeIt& operator--();
      BSTreeIt operator--(int);

      bool operator==(const BSTreeIt& other) const;
      bool operator!=(const BSTreeIt& other) const;

      operator BSTreeCIt< Key, Value >() const;

    private:
      Node* node_;
      Node* root_;

      template< class K, class V, class C >
      friend class BSTree;

      template< class K, class V >
      friend class BSTreeCIt;
  };

  template< class Key, class Value >
  class BSTreeCIt
  {
    private:
      using Node = studilova::Node< Key, Value >;

    public:
      BSTreeCIt();
      BSTreeCIt(const Node* node, const Node* root);
      BSTreeCIt(const BSTreeIt< Key, Value >& other);

      std::pair< const Key&, const Value& > operator*() const;

      BSTreeCIt& operator++();
      BSTreeCIt operator++(int);
      BSTreeCIt& operator--();
      BSTreeCIt operator--(int);

      bool operator==(const BSTreeCIt& other) const;
      bool operator!=(const BSTreeCIt& other) const;

    private:
      const Node* node_;
      const Node* root_;

      template< class K, class V, class C >
      friend class BSTree;
  };
}

template< class Key, class Value >
studilova::BSTreeIt< Key, Value >::BSTreeIt() :
  node_(nullptr),
  root_(nullptr)
{}

template< class Key, class Value >
studilova::BSTreeIt< Key, Value >::BSTreeIt(Node* node, Node* root) :
  node_(node),
  root_(root)
{}

template< class Key, class Value >
std::pair< const Key&, Value& > studilova::BSTreeIt< Key, Value >::operator*() const
{
  return { node_->key_, node_->value_ };
}

template< class Key, class Value >
studilova::BSTreeIt< Key, Value >& studilova::BSTreeIt< Key, Value >::operator++()
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
studilova::BSTreeIt< Key, Value > studilova::BSTreeIt< Key, Value >::operator++(int)
{
  BSTreeIt temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
studilova::BSTreeIt< Key, Value >& studilova::BSTreeIt< Key, Value >::operator--()
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
studilova::BSTreeIt< Key, Value > studilova::BSTreeIt< Key, Value >::operator--(int)
{
  BSTreeIt temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool studilova::BSTreeIt< Key, Value >::operator==(const BSTreeIt& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool studilova::BSTreeIt< Key, Value >::operator!=(const BSTreeIt& other) const
{
  return !(*this == other);
}

template< class Key, class Value >
studilova::BSTreeIt< Key, Value >::operator BSTreeCIt< Key, Value >() const
{
  return BSTreeCIt< Key, Value >(node_, root_);
}

template< class Key, class Value >
studilova::BSTreeCIt< Key, Value >::BSTreeCIt() :
  node_(nullptr),
  root_(nullptr)
{}

template< class Key, class Value >
studilova::BSTreeCIt< Key, Value >::BSTreeCIt(const Node* node, const Node* root) :
  node_(node),
  root_(root)
{}

template< class Key, class Value >
studilova::BSTreeCIt< Key, Value >::BSTreeCIt(const BSTreeIt< Key, Value >& other) :
  node_(other.node_),
  root_(other.root_)
{}

template< class Key, class Value >
std::pair< const Key&, const Value& > studilova::BSTreeCIt< Key, Value >::operator*() const
{
  return { node_->key_, node_->value_ };
}

template< class Key, class Value >
studilova::BSTreeCIt< Key, Value >& studilova::BSTreeCIt< Key, Value >::operator++()
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
studilova::BSTreeCIt< Key, Value > studilova::BSTreeCIt< Key, Value >::operator++(int)
{
  BSTreeCIt temp(*this);
  ++(*this);
  return temp;
}

template< class Key, class Value >
studilova::BSTreeCIt< Key, Value >& studilova::BSTreeCIt< Key, Value >::operator--()
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
studilova::BSTreeCIt< Key, Value > studilova::BSTreeCIt< Key, Value >::operator--(int)
{
  BSTreeCIt temp(*this);
  --(*this);
  return temp;
}

template< class Key, class Value >
bool studilova::BSTreeCIt< Key, Value >::operator==(const BSTreeCIt& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool studilova::BSTreeCIt< Key, Value >::operator!=(const BSTreeCIt& other) const
{
  return !(*this == other);
}

#endif
