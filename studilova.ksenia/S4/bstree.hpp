#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "bstree_node.hpp"
#include "bstree_iterators.hpp"

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace studilova
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    public:
      BSTree();
      explicit BSTree(const Compare& cmp);
      ~BSTree();

      BSTree(const BSTree& other);
      BSTree(BSTree&& other) noexcept;
      BSTree& operator=(const BSTree& other);
      BSTree& operator=(BSTree&& other) noexcept;

      bool empty() const;

      void push(const Key& key, const Value& value);
      Value& get(const Key& key);
      const Value& get(const Key& key) const;

      Value drop(const Key& key);

      using It = studilova::BSTreeIt< Key, Value >;
      using CIt = studilova::BSTreeCIt< Key, Value >;

      It begin();
      It end();

      CIt begin() const;
      CIt end() const;
      CIt cbegin() const;
      CIt cend() const;

      size_t height() const;
      size_t height(CIt it) const;

    private:
      using Node = studilova::Node< Key, Value >;

      Node* root_;
      Compare cmp_;

      void clear(Node* node);
      Node* clone(const Node* node, Node* parent);
      Node* findNode(const Key& key);
      const Node* findNode(const Key& key) const;
      Node* getMin(Node* node) const;
      const Node* getMin(const Node* node) const;
      Node* getMax(Node* node) const;
      void replaceNode(Node* old_node, Node* new_node);
      size_t calcHeight(const Node* node) const;
      void swap(BSTree& other) noexcept;
  };
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree() :
  root_(nullptr),
  cmp_(Compare())
{}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree(const Compare& cmp) :
  root_(nullptr),
  cmp_(cmp)
{}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::~BSTree()
{
  clear(root_);
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree(const BSTree& other) :
  root_(nullptr),
  cmp_(other.cmp_)
{
  root_ = clone(other.root_, nullptr);
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept :
  root_(other.root_),
  cmp_(other.cmp_)
{
  other.root_ = nullptr;
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >& studilova::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >& studilova::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != &other)
  {
    clear(root_);
    root_ = other.root_;
    cmp_ = other.cmp_;
    other.root_ = nullptr;
  }
  return *this;
}

template< class Key, class Value, class Compare >
bool studilova::BSTree< Key, Value, Compare >::empty() const
{
  return root_ == nullptr;
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  if (!root_)
  {
    root_ = new Node(key, value);
    return;
  }

  Node* current = root_;
  Node* parent = nullptr;

  while (current)
  {
    parent = current;

    if (!cmp_(key, current->key_) && !cmp_(current->key_, key))
    {
      current->value_ = value;
      return;
    }
    else if (cmp_(key, current->key_))
    {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }

  Node* new_node = new Node(key, value);
  new_node->parent_ = parent;

  if (cmp_(key, parent->key_))
  {
    parent->left_ = new_node;
  } else {
    parent->right_ = new_node;
  }
}

template< class Key, class Value, class Compare >
Value& studilova::BSTree< Key, Value, Compare >::get(const Key& key)
{
  return findNode(key)->value_;
}

template< class Key, class Value, class Compare >
const Value& studilova::BSTree< Key, Value, Compare >::get(const Key& key) const
{
  return findNode(key)->value_;
}

template< class Key, class Value, class Compare >
Value studilova::BSTree< Key, Value, Compare >::drop(const Key& key)
{
  Node* node = findNode(key);
  Value result = node->value_;

  if (node->left_ && node->right_)
  {
    Node* next = getMin(node->right_);

    node->key_ = next->key_;
    node->value_ = next->value_;

    node = next;
  }

  Node* child = node->left_ ? node->left_ : node->right_;
  replaceNode(node, child);

  delete node;
  return result;
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::It studilova::BSTree< Key, Value, Compare >::begin()
{
  return It(getMin(root_), root_);
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::It studilova::BSTree< Key, Value, Compare >::end()
{
  return It(nullptr, root_);
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::CIt studilova::BSTree< Key, Value, Compare >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::CIt studilova::BSTree< Key, Value, Compare >::end() const
{
  return cend();
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::CIt studilova::BSTree< Key, Value, Compare >::cbegin() const
{
  return CIt(getMin(root_), root_);
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::CIt studilova::BSTree< Key, Value, Compare >::cend() const
{
  return CIt(nullptr, root_);
}

template< class Key, class Value, class Compare >
size_t studilova::BSTree< Key, Value, Compare >::height() const
{
  return calcHeight(root_);
}

template< class Key, class Value, class Compare >
size_t studilova::BSTree< Key, Value, Compare >::height(CIt it) const
{
  return calcHeight(it.node_);
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::clear(Node* node)
{
  if (!node)
  {
    return;
  }
  clear(node->left_);
  clear(node->right_);
  delete node;
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::clone(const Node* node, Node* parent)
{
  if (!node)
  {
    return nullptr;
  }

  Node* copy = new Node(node->key_, node->value_);
  copy->parent_ = parent;
  copy->left_ = clone(node->left_, copy);
  copy->right_ = clone(node->right_, copy);

  return copy;
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::findNode(const Key& key)
{
  Node* current = root_;
  while (current)
  {
    if (!cmp_(key, current->key_) && !cmp_(current->key_, key))
    {
      return current;
    }
    else if (cmp_(key, current->key_))
    {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Compare >
const typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::findNode(const Key& key) const
{
  const Node* current = root_;
  while (current)
  {
    if (!cmp_(key, current->key_) && !cmp_(current->key_, key))
    {
      return current;
    }
    else if (cmp_(key, current->key_))
    {
      current = current->left_;
    } else {
      current = current->right_;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare>::getMin(Node* node) const
{
  if (!node)
  {
    return nullptr;
  }

  while (node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
const typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::getMin(const Node* node) const
{
  if (!node)
  {
    return nullptr;
  }

  while (node->left_)
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::getMax(Node* node) const
{
  if (!node)
  {
    return nullptr;
  }

  while (node->right_)
  {
    node = node->right_;
  }

  return node;
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::replaceNode(Node* old_node, Node* new_node)
{
  if (new_node)
  {
    new_node->parent_ = old_node->parent_;
  }

  if (!old_node->parent_)
  {
    root_ = new_node;
  }
  else if (old_node == old_node->parent_->left_)
  {
    old_node->parent_->left_ = new_node;
  } else {
    old_node->parent_->right_ = new_node;
  }
}

template< class Key, class Value, class Compare >
size_t studilova::BSTree< Key, Value, Compare >::calcHeight(const Node* node) const
{
  if (!node)
  {
    return 0;
  }

  size_t left_height = calcHeight(node->left_);
  size_t right_height = calcHeight(node->right_);

  return 1 + (left_height > right_height ? left_height : right_height);
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(cmp_, other.cmp_);
}

#endif
