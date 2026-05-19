#ifndef BSTREE_CONST_ITERATOR
#define BSTREE_CONST_ITERATOR
#include <functional>
#include <cstddef>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BSTreeCIter {
  public:
    using node_t = Node< Key, Value >;

    BSTreeCIter();
    BSTreeCIter< Key, Value >& operator++();
    BSTreeCIter< Key, Value > operator++(int);

    BSTreeCIter< Key, Value >& operator--();
    BSTreeCIter< Key, Value > operator--(int);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator!=(const BSTreeCIter< Key, Value >& other) const;
    bool operator==(const BSTreeCIter< Key, Value >& other) const;
  private:
    node_t* node_;
    node_t* fakeLeaf_;
    BSTreeCIter(node_t* node, node_t* fakeLeaf);
  };
}



template< class Key, class Value >
malashenko::BSTreeCIter< Key, Value >::BSTreeCIter():
  node_(nullptr),
  fakeLeaf_(nullptr)
{}

template< class Key, class Value >
malashenko::BSTreeCIter< Key, Value >::BSTreeCIter(node_t* node, node_t* fakeLeaf):
  node_(node),
  fakeLeaf_(fakeLeaf)
{}


template< class Key, class Value >
malashenko::BSTreeCIter< Key, Value >& malashenko::BSTreeCIter< Key, Value >::operator++()
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
malashenko::BSTreeCIter< Key, Value > malashenko::BSTreeCIter< Key, Value >::operator++(int)
{
  BSTreeCIter< Key, Value > tmp{*this};
  ++(*this);
  return tmp;
}

template< class Key, class Value >
malashenko::BSTreeCIter< Key, Value >& malashenko::BSTreeCIter< Key, Value >::operator--()
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
malashenko::BSTreeCIter< Key, Value > malashenko::BSTreeCIter< Key, Value >::operator--(int)
{
  BSTreeCIter< Key, Value > tmp{*this};
  --(*this);
  return tmp;
}

template< class Key, class Value >
const std::pair< Key, Value >& malashenko::BSTreeCIter< Key, Value >::operator*() const
{
  assert(node_);
  return {node_->key_, node_->value_};
}

template< class Key, class Value >
const std::pair< Key, Value >* malashenko::BSTreeCIter< Key, Value >::operator->() const
{
  assert(node_);
  return std::addressof(node_->key_, node_->value_);
}

template< class Key, class Value >
bool malashenko::BSTreeCIter< Key, Value >::operator==(const BSTreeCIter< Key, Value >& other) const
{
  return node_ == other.node_;
}

template< class Key, class Value >
bool malashenko::BSTreeCIter< Key, Value >::operator!=(const BSTreeCIter< Key, Value >& other) const
{
  return !(node_ == other.node_);
}
#endif