#ifndef BSTREE_ITERATOR
#define BSTREE_ITERATOR
#include <functional>
#include <cstddef>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BSTreeIter {
  public:
    using node_t = Node< Key, Value >;
    BSTreeIter();
    BSTreeIter< Key, Value >& operator++();
    BSTreeIter< Key, Value > operator++(int);

    BSTreeIter< Key, Value >& operator--();
    BSTreeIter< Key, Value > operator--(int);

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

    bool operator!=(const BSTreeIter< Key, Value >& other) const;
    bool operator==(const BSTreeIter< Key, Value >& other) const;
  private:
    node_t* node_;
    node_t* fakeLeaf_;
    BSTreeIter(node_t* node, node_t* fakeLeaf);
  };
}


template< class Key, class Value >
malashenko::BSTreeIter< Key, Value >::BSTreeIter():
  node_(nullptr),
  fakeLeaf_(nullptr)
{}

template< class Key, class Value >
malashenko::BSTreeIter< Key, Value >::BSTreeIter(node_t* node, node_t* fakeLeaf):
  node_(node),
  fakeLeaf_(fakeLeaf)
{}


template< class Key, class Value >
malashenko::BSTreeIter< Key, Value >& malashenko::BSTreeIter< Key, Value >::operator++()
{
  if (node_->right_)
  {
    node_ = node_->right_;
    node_ = node_->minimum(fakeLeaf_);
  }
  else
  {
    node_t* parent = node_->parent;
    while (parent && parent->right_ == node_)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
}


template< class Key, class Value >
malashenko::BSTreeIter< Key, Value > malashenko::BSTreeIter< Key, Value >::operator++(int)
{
  BSTreeIter< Key, Value > tmp{*this};
  ++(*this);
  return tmp;
}

template< class Key, class Value >
malashenko::BSTreeIter< Key, Value >& malashenko::BSTreeIter< Key, Value >::operator--()
{
  if (node_->left_)
  {
    node_ = node_->left_;
    node_ = node_->maximum(fakeLeaf_);
  }
  else
  {
    node_t* parent = node_->parent;
    while (parent && parent->left_ == node_)
    {
      node_ = parent;
      parent = node_->parent;
    }
    node_ = parent;
  }
  return *this;
}

template< class Key, class Value >
malashenko::BSTreeIter< Key, Value > malashenko::BSTreeIter< Key, Value >::operator--(int)
{
  BSTreeIter< Key, Value > tmp{*this};
  --(*this);
  return tmp;
}

template< class Key, class Value >
std::pair< Key, Value >& malashenko::BSTreeIter< Key, Value >::operator*()
{
  assert(node_);
  return {node_->key_, node_->value_};
}

template< class Key, class Value >
std::pair< Key, Value >* malashenko::BSTreeIter< Key, Value >::operator->()
{
  assert(node_);
  return std::addressof(node_->key_, node_->value_);
}

template< class Key, class Value >
bool malashenko::BSTreeIter< Key, Value >::operator==(const BSTreeIter< Key, Value >& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool malashenko::BSTreeIter< Key, Value >::operator!=(const BSTreeIter< Key, Value >& other) const
{
  return !(node_ == other.node_);
}




#endif