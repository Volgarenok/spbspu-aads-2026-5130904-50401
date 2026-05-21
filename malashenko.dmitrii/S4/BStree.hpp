#ifndef BSTREE
#define BSTREE
#include <iostream>
#include <cstddef>
#include <utility>
#include "const-iterator.hpp"
#include "iterator.hpp"
#include "node.hpp"
namespace malashenko
{

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree {
  public:
    using cIter = malashenko::BSTreeCIter< Key, Value >;
    using iter = malashenko::BSTreeIter< Key, Value >;
    using node_t = malashenko::Node< Key, Value >;

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

    cIter rotateLeft(cIter it);
    cIter rotateRight(cIter it);
    cIter rotateLargeLeft(cIter it);
    cIter rotateLargeRight(cIter it);

    iter begin();
    cIter begin() const;
    cIter cbegin() const;

    iter end();
    cIter end() const;
    cIter cend() const;

    size_t height(cIter it);
    size_t height();

    void swap(BSTree& other) noexcept;
    bool empty() const;
  private:
    node_t* fakeLeaf_;
    node_t* root_;

    void clear(node_t* root);
    node_t* find(const Key& k) const;
    node_t* cloneNode(node_t* it, node_t* parent, node_t* otherFake);
  };
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree():
  fakeLeaf_(reinterpret_cast< node_t* >(::operator new(sizeof(node_t))))
{
  fakeLeaf_->left_ = fakeLeaf_;
  fakeLeaf_->right_ = fakeLeaf_;
  fakeLeaf_->parent_ = nullptr;
  root_ = fakeLeaf_;
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::~BSTree()
{
  clear(root_);
  ::operator delete(fakeLeaf_);
}

template< class Key, class Value, class Compare >
void malashenko::BSTree< Key, Value, Compare >::clear(node_t* root)
{
  if (!root || root == fakeLeaf_)
  {
    return;
  }

  clear(root->left_);
  clear(root->right_);
  delete root;
}


template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  BSTree()
{
  root_ = cloneNode(other.root_, nullptr, other.fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::Node< Key, Value >* malashenko::BSTree< Key, Value, Compare >::cloneNode(
  node_t* node,
  node_t* parent,
  node_t* otherFake)
{
  if (node == otherFake)
  {
    return fakeLeaf_;
  }

  node_t* newNode = new node_t(node->data_.first, node->data_.second);
  newNode->parent_ = parent;
  newNode->left_ = cloneNode(node->left_, newNode, otherFake);
  newNode->right_ = cloneNode(node->right_, newNode, otherFake);
  return newNode;
}

template <class Key, class Value, class Compare>
Value malashenko::BSTree<Key, Value, Compare>::drop(const Key& k)
{
  node_t* node = find(k);

  if (!node)
  {
    throw std::out_of_range("There is no Value with that Key");
  }

  Value deletedValue = node->data_.second;
  while (node->left_ != fakeLeaf_ && node->right_ != fakeLeaf_)
  {
    Node<Key, Value>* tmpNode = node->right_;

    node->right_ = tmpNode->left_;
    if (tmpNode->left_ != fakeLeaf_)
    {
      tmpNode->left_->parent_ = node;
    }

    tmpNode->parent_ = node->parent_;
    if (node->parent_ == fakeLeaf_)
    {
      root_ = tmpNode;
    }
    else if (node == node->parent_->left_)
    {
      node->parent_->left_ = tmpNode;
    }
    else
    {
      node->parent_->right_ = tmpNode;
    }

    tmpNode->left_ = node;
    node->parent_ = tmpNode;
  }

  Node<Key, Value>* child = node->left_ == fakeLeaf_ ? node->right_ : node->left_;
  Node<Key, Value>* parent = node->parent_;

  if (parent == fakeLeaf_)
  {
    root_ = child;
  }
  else
  {
    if (parent->left_ == node)
    {
      parent->left_ = child;
    }
    else
    {
      parent->right_ = child;
    }
  }

  if (child != fakeLeaf_)
  {
    child->parent_ = parent;
  }

  delete node;
  return deletedValue;
}

template< class Key, class Value, class Compare >
malashenko::Node< Key, Value >* malashenko::BSTree< Key, Value, Compare >::find(const Key& k) const
{
  Compare cmp;
  node_t* tmp = root_;

  while (tmp != fakeLeaf_)
  {
    if (!cmp(k, tmp->data_.first) && !cmp(tmp->data_.first, k))
    {
      return tmp;
    }

    if (cmp(k, tmp->data_.first))
    {
      tmp = tmp->left_;
    }
    else
    {
      tmp = tmp->right_;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  fakeLeaf_(std::exchange(other.fakeLeaf_, nullptr)),
  root_(std::exchange(other.root_, nullptr))
{}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >& malashenko::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  BSTree< Key, Value, Compare > tmp(other);
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Compare >
malashenko::BSTree< Key, Value, Compare >& malashenko::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  assert(this != &other);

  BSTree< Key, Value, Compare > tmp(std::forward< BSTree >(other));
  swap(tmp);
  return *this;
}

template< class Key, class Value, class Compare >
void malashenko::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(fakeLeaf_, other.fakeLeaf_);
}

template< class Key, class Value, class Compare >
void malashenko::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  node_t* tmp = root_;
  node_t* parent = nullptr;
  Compare cmp;
  while (tmp != fakeLeaf_)
  {
    if (!cmp(k, tmp->data_.first) && !cmp(tmp->data_.first, k))
    {
      tmp->data_.second =  v;
      return;
    }
    parent = tmp;
    tmp = cmp(k, tmp->data_.first) ? tmp->left_ : tmp->right_;
  }
  node_t* newNode = new node_t(k, v);
  newNode->left_ = fakeLeaf_;
  newNode->right_ = fakeLeaf_;
  newNode->parent_ = parent ? parent : fakeLeaf_;

  if (!parent)
  {
    root_ = newNode;
  }
  else
  {
    if (cmp(k, parent->data_.first))
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
  const BSTree< Key, Value, Compare >* cthis = this;
  return const_cast< Value& >(cthis->get(k));
}

template< class Key, class Value, class Compare >
const Value& malashenko::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  node_t* node = find(k);
  if (!node)
  {
    throw std::out_of_range("There is no Value with that Key");
  }
  return node->data_.second;
}


template< class Key, class Value, class Compare >
size_t malashenko::BSTree< Key, Value, Compare >::height()
{
  return root_->height(fakeLeaf_);
}

template< class Key, class Value, class Compare >
size_t malashenko::BSTree< Key, Value, Compare >::height(cIter it)
{
  return it.node_->height(fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::rotateLeft(cIter it)
{
  Node<Key, Value>* rotateNode = it.node_;
  if (rotateNode == fakeLeaf_ || rotateNode->right_ == fakeLeaf_)
  {
    return it;
  }

  Node<Key, Value>* rightNode = rotateNode->right_;

  rotateNode->right_ = rightNode->left_;
  if (rightNode->left_ != fakeLeaf_)
  {
    rightNode->left_->parent_ = rotateNode;
  }

  rightNode->parent_ = rotateNode->parent_;
  if (rotateNode->parent_ == fakeLeaf_)
  {
    root_ = rightNode;
  }
  else if (rotateNode == rotateNode->parent_->left_)
  {
    rotateNode->parent_->left_ = rightNode;
  }
  else
  {
    rotateNode->parent_->right_ = rightNode;
  }

  rightNode->left_ = rotateNode;
  rotateNode->parent_ = rightNode;

  return cIter(rightNode, fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::rotateRight(cIter it)
{
  Node<Key, Value>* rotateNode = it.node_;
  if (rotateNode == fakeLeaf_ || rotateNode->left_ == fakeLeaf_)
  {
    return it;
  }

  Node<Key, Value>* leftNode = rotateNode->left_;

  rotateNode->left_ = leftNode->right_;
  if (leftNode->right_ != fakeLeaf_)
  {
    leftNode->right_->parent_ = rotateNode;
  }

  leftNode->parent_ = rotateNode->parent_;
  if (rotateNode->parent_ == fakeLeaf_)
  {
    root_ = leftNode;
  }
  else if (rotateNode == rotateNode->parent_->left_)
  {
    rotateNode->parent_->left_ = leftNode;
  }
  else
  {
    rotateNode->parent_->right_ = leftNode;
  }

  leftNode->right_ = rotateNode;
  rotateNode->parent_ = leftNode;

  return cIter(leftNode, fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::rotateLargeLeft(cIter it)
{
  Node<Key, Value>* rotateNode = it.node_;
  if (rotateNode == fakeLeaf_ || rotateNode->left_ == fakeLeaf_ || rotateNode->left_->right_ == fakeLeaf_)
  {
    return it;
  }

  rotateRight(cIter(rotateNode->left_, fakeLeaf_));
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::rotateLargeRight(cIter it)
{
  Node<Key, Value>* rotateNode = it.node_;
  if (rotateNode == fakeLeaf_ || rotateNode->right_ == fakeLeaf_ || rotateNode->right_->left_ == fakeLeaf_)
  {
    return it;
  }

  rotateLeft(cIter(rotateNode->right_, fakeLeaf_));
  return rotateRight(it);
}


template< class Key, class Value, class Compare >
malashenko::BSTreeIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::begin()
{
  if (empty())
  {
    return end();
  }

  return iter(root_->minimum(fakeLeaf_), fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::begin() const
{
  if (empty())
  {
    return end();
  }
  return cIter(root_->minimum(fakeLeaf_), fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::cbegin() const
{
  if (empty())
  {
    return end();
  }

  return cIter(root_->minimum(fakeLeaf_), fakeLeaf_);
}


template< class Key, class Value, class Compare >
malashenko::BSTreeIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::end()
{

  return iter(fakeLeaf_, fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::end() const
{
  return cIter(fakeLeaf_, fakeLeaf_);
}

template< class Key, class Value, class Compare >
malashenko::BSTreeCIter< Key, Value > malashenko::BSTree< Key, Value, Compare>::cend() const
{
  return cIter(fakeLeaf_, fakeLeaf_);
}

template< class Key, class Value, class Compare >
bool malashenko::BSTree< Key, Value, Compare>::empty() const
{
  return root_ == fakeLeaf_;
}

#endif
