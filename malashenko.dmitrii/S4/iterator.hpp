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
    BSTreeIter< Key, Value > operator+(size_t s);

    BSTreeIter< Key, Value >& operator--();
    BSTreeIter< Key, Value > operator--(int);
    BSTreeIter< Key, Value > operator-(size_t s);

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
  node_t* next = node_;
  if (next->right_)
  {
    next = next->right_;
    next = next->minimum(fakeLeaf_);
  }
  else
  {
    node_t* parent = next->parent;
    while (parent && parent->right_ == next)
    {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  return {next, fakeLeaf_};
}


template< class Key, class Value >
malashenko::BSTreeIter< Key, Value > malashenko::BSTreeIter< Key, Value >::operator++(int)
{
  BSTreeIter< Key, Value > tmp(*this)
  ++(*this);
  return tmp;
}

template< class Key, class Value >
malashenko::BSTreeIter< Key, Value >& malashenko::BSTreeIter< Key, Value >::operator--()
{
  node_t* next = node_;
  if (next->left_)
  {
    next = next->left_;
    next = next->maximum(fakeLeaf_);
  }
  else
  {
    node_t* parent = next->parent;
    while (parent && parent->left_ == next)
    {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  return {next, fakeLeaf_};
}

template< class Key, class Value >
malashenko::BSTreeIter< Key, Value > malashenko::BSTreeIter< Key, Value >::operator--(int)
{
  BSTreeIter< Key, Value > tmp(*this)
  --(*this);
  return tmp;
}




#endif