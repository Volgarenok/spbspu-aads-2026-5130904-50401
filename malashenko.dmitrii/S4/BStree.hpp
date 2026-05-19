#ifndef BSTREE
#define BSTREE
#include <iostream>
#include <cstddef>
#include <utility>
#include "const-iterator.hpp"
#include "node.hpp"
namespace malashenko
{

  template< class Key, class Value, class Compare >
  class BSTree {
  public:
    using const_iterator = malashenko::BSTreeCIter< Key, Value >;

    BSTree();
    ~BSTree();

    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;

    Value drop(const Key& k);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    size_t height(const_iterator it);
    size_t height();

    void swap(BSTree& other) noexcept;
  private:
    malashenko::Node< Key, Value >* fakeLeaf_;
    malashenko::Node< Key, Value >* root_;

    void clear();
    malashenko::Node< Key, Value >* cloneNode(const_iterator it);
  };
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree():
  fakeLeaf_(reinterpret_cast< Node< Key, Value >* >(::operator new(sizeof(Node< Key, Value >))))
{
  fakeLeaf_->left_ = fakeLeaf_;
  fakeLeaf_->right_ = fakeLeaf_;
  fakeLeaf_->parent_ = nullptr;
  root_ = fakeLeaf_;
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::~BSTree()
{
  clear(); // CLEAR FUNC
  ::operator delete(fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  BSTree()
{
  // CLONE NODE
}



template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  fakeLeaf_(std::exchange(fakeLeaf_, nullptr)),
  root_(std::exchange(fakeLeaf_, nullptr))
{}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >& malashenko::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  BSTree< Key, Value, Compare > tmp(other);
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >& malashenko::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  assert(this == &other);

  BSTree< Key, Value, Compare > tmp(std::forward(other));
  swap(other);
  return *this;
}

template< class Key, class Value, class Compare >
void malashenko::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  swap(root_, other.root_);
  swap(fakeLeaf_, other.fakeLeaf_);
}

template< class Key, class Value, class Compare >
void malashenko::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  Node< Key, Value >* tmp = root_;
  Node< Key, Value >* parent = nullptr;
  Compare cmp;
  while (tmp != fakeLeaf_)
  {
    if (!cmp(k, tmp->key_) && !cmp(tmp->key_, k))
    {
      tmp->value_ =  v;
      return;
    }
    parent = tmp;
    tmp = cmp(k, tmp->key_) ? tmp->left_ : tmp->right_;
  }
  Node< Key, Value > newNode = Node< Key, Value >(k, v);
  newNode->left_ = fakeLeaf_;
  newNode->right_ = fakeLeaf_;
  newNode->parent_ = parent;

  if (!parent)
  {
    root_ = newNode;
  }
  else
  {
    if (parent->left_ == tmp)
    {
      parent->left_ = newNode;
    }
    else
    {
      parent->right_ = newNode;
    }
  }
}

template< class Key, class Value, class Compare >
Value& malashenko::BSTree< Key, Value, Compare >::get(const Key& k)
{
  BSTree< Key, Value, Compare > cthis = this;
  return const_cast< Value& >(cthis->get(k));
}

template< class Key, class Value, class Compare >
const Value& malashenko::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  malashenko::Node< Key, Value >* tmp = root_;
  while (tmp != fakeLeaf_)
  {
    if (!cmp(k, tmp->key_) && !cmp(tmp->key_, k))
    {
      return tmp->value_;
    }
    tmp = cmp(k, tmp->key_) ? tmp->left_ : tmp->right_;
  }

  throw std::invalid_argument("There is no Value with that Key");
}


template< class Key, class Value, class Compare >
size_t malashenko::BSTree< Key, Value, Compare >::height()
{
  if (root_ == fakeLeaf_)
  {
    return 0;
  }
  if (root_->left_ == fakeLeaf_ && root_->right_ == fakeLeaf_)
  {
    return 1;
  }
  return std::max(height(root_->left_), height(root_->right_)) + 1;
}



#endif