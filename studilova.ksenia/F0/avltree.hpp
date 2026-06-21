#ifndef AVLTree_HPP
#define AVLTree_HPP

#include "avltree_node.hpp"
#include "avltree_iterator.hpp"

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace studilova
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
    public:
      using It = studilova::AVLTreeIt< Key, Value >;
      using CIt = studilova::AVLTreeCIt< Key, Value >;

      AVLTree();
      explicit AVLTree(const Compare& cmp);
      ~AVLTree();

      AVLTree(const AVLTree& other);
      AVLTree(AVLTree&& other) noexcept;
      AVLTree& operator=(const AVLTree& other);
      AVLTree& operator=(AVLTree&& other) noexcept;

      bool empty() const;

      void push(const Key& key, const Value& value);
      Value& get(const Key& key);
      const Value& get(const Key& key) const;

      Value drop(const Key& key);

      It begin();
      It end();

      CIt begin() const;
      CIt end() const;
      CIt cbegin() const;
      CIt cend() const;

      CIt rotateLeft(CIt it);
      CIt rotateRight(CIt it);
      CIt rotateLargeLeft(CIt it);
      CIt rotateLargeRight(CIt it);

      size_t height() const;
      size_t height(CIt it) const;

    private:
      using Node = studilova::detail::AVLNode< Key, Value >;

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

      size_t getNodeHeight(const Node* node) const noexcept;
      int getBalance(const Node* node) const;
      void updateNodeHeight(Node* node);

      Node* balance(Node* node);
      void rebalanceFrom(Node* node);

      void swap(AVLTree& other) noexcept;
  };
}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >::AVLTree():
  root_(nullptr),
  cmp_(Compare())
{}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >::AVLTree(const Compare& cmp):
  root_(nullptr),
  cmp_(cmp)
{}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >::~AVLTree()
{
  clear(root_);
}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& other):
  root_(nullptr),
  cmp_(other.cmp_)
{
  root_ = clone(other.root_, nullptr);
}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& other) noexcept:
  root_(other.root_),
  cmp_(other.cmp_)
{
  other.root_ = nullptr;
}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >& studilova::AVLTree< Key, Value, Compare >::operator=(const AVLTree& other)
{
  if (this != &other)
  {
    AVLTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
studilova::AVLTree< Key, Value, Compare >& studilova::AVLTree< Key, Value, Compare >::operator=(AVLTree&& other) noexcept
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
bool studilova::AVLTree< Key, Value, Compare >::empty() const
{
  return root_ == nullptr;
}

template< class Key, class Value, class Compare >
void studilova::AVLTree< Key, Value, Compare >::push(const Key& key, const Value& value)
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
    }
    else
    {
      current = current->right_;
    }
  }

  Node* new_node = new Node(key, value);
  new_node->parent_ = parent;

  if (cmp_(key, parent->key_))
  {
    parent->left_ = new_node;
  }
  else
  {
    parent->right_ = new_node;
  }
}

template< class Key, class Value, class Compare >
Value& studilova::AVLTree< Key, Value, Compare >::get(const Key& key)
{
  return findNode(key)->value_;
}

template< class Key, class Value, class Compare >
const Value& studilova::AVLTree< Key, Value, Compare >::get(const Key& key) const
{
  return findNode(key)->value_;
}

template< class Key, class Value, class Compare >
Value studilova::AVLTree< Key, Value, Compare >::drop(const Key& key)
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
typename studilova::AVLTree< Key, Value, Compare >::It studilova::AVLTree< Key, Value, Compare >::begin()
{
  return It(getMin(root_), root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::It studilova::AVLTree< Key, Value, Compare >::end()
{
  return It(nullptr, root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::begin() const
{
  return cbegin();
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::end() const
{
  return cend();
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::cbegin() const
{
  return CIt(getMin(root_), root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::cend() const
{
  return CIt(nullptr, root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::rotateLeft(CIt it)
{
  Node* old_root = const_cast< Node* >(it.node_);

  if (!old_root || !old_root->right_)
  {
    throw std::logic_error("left rotation is impossible");
  }

  Node* new_root = old_root->right_;
  Node* moved_subtree = new_root->left_;

  old_root->right_ = moved_subtree;
  if (moved_subtree)
  {
    moved_subtree->parent_ = old_root;
  }

  new_root->parent_ = old_root->parent_;

  if (!old_root->parent_)
  {
    root_ = new_root;
  }
  else if (old_root == old_root->parent_->left_)
  {
    old_root->parent_->left_ = new_root;
  }
  else
  {
    old_root->parent_->right_ = new_root;
  }

  new_root->left_ = old_root;
  old_root->parent_ = new_root;

  return CIt(new_root, root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::rotateRight(CIt it)
{
  Node* old_root = const_cast< Node* >(it.node_);

  if (!old_root || !old_root->left_)
  {
    throw std::logic_error("right rotation is impossible");
  }

  Node* new_root = old_root->left_;
  Node* moved_subtree = new_root->right_;

  old_root->left_ = moved_subtree;
  if (moved_subtree)
  {
    moved_subtree->parent_ = old_root;
  }

  new_root->parent_ = old_root->parent_;

  if (!old_root->parent_)
  {
    root_ = new_root;
  }
  else if (old_root == old_root->parent_->left_)
  {
    old_root->parent_->left_ = new_root;
  }
  else
  {
    old_root->parent_->right_ = new_root;
  }

  new_root->right_ = old_root;
  old_root->parent_ = new_root;

  return CIt(new_root, root_);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::rotateLargeLeft(CIt it)
{
  Node* old_root = const_cast< Node* >(it.node_);

  if (!old_root || !old_root->right_ || !old_root->right_->left_)
  {
    throw std::logic_error("large left rotation is impossible");
  }

  rotateRight(CIt(old_root->right_, root_));
  return rotateLeft(CIt(old_root, root_));
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::CIt studilova::AVLTree< Key, Value, Compare >::rotateLargeRight(CIt it)
{
  Node* old_root = const_cast< Node* >(it.node_);

  if (!old_root || !old_root->left_ || !old_root->left_->right_)
  {
    throw std::logic_error("large right rotation is impossible");
  }

  rotateLeft(CIt(old_root->left_, root_));
  return rotateRight(CIt(old_root, root_));
}

template< class Key, class Value, class Compare >
size_t studilova::AVLTree< Key, Value, Compare >::height() const
{
  return calcHeight(root_);
}

template< class Key, class Value, class Compare >
size_t studilova::AVLTree< Key, Value, Compare >::height(CIt it) const
{
  return calcHeight(it.node_);
}

template< class Key, class Value, class Compare >
void studilova::AVLTree< Key, Value, Compare >::clear(Node* node)
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
typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::clone(const Node* node, Node* parent)
{
  if (!node)
  {
    return nullptr;
  }

  Node* copy = new Node(node->key_, node->value_);
  copy->parent_ = parent;
  copy->height_ = node->height_;
  copy->left_ = clone(node->left_, copy);
  copy->right_ = clone(node->right_, copy);

  return copy;
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::findNode(const Key& key)
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
    }
    else
    {
      current = current->right_;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Compare >
const typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::findNode(const Key& key) const
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
    }
    else
    {
      current = current->right_;
    }
  }
  throw std::out_of_range("key not found");
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::getMin(Node* node) const
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
const typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::getMin(const Node* node) const
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
typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::getMax(Node* node) const
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
void studilova::AVLTree< Key, Value, Compare >::replaceNode(Node* old_node, Node* new_node)
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
  }
  else
  {
    old_node->parent_->right_ = new_node;
  }
}

template< class Key, class Value, class Compare >
size_t studilova::AVLTree< Key, Value, Compare >::calcHeight(const Node* node) const
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
size_t studilova::AVLTree< Key, Value, Compare >::getNodeHeight(const Node* node) const noexcept
{
  if (!node)
  {
    return 0;
  }

  return node->height_;
}

template< class Key, class Value, class Compare >
int studilova::AVLTree< Key, Value, Compare >::getBalance(const Node* node) const
{
  if (!node)
  {
    return 0;
  }

  return static_cast< int >(getNodeHeight(node->left_)) - static_cast< int >(getNodeHeight(node->right_));
}

template< class Key, class Value, class Compare >
void studilova::AVLTree< Key, Value, Compare >::updateNodeHeight(Node* node)
{
  if (!node)
  {
    return;
  }

  size_t left_height = getNodeHeight(node->left_);
  size_t right_height = getNodeHeight(node->right_);

  node->height_ = 1 + (left_height > right_height ? left_height : right_height);
}

template< class Key, class Value, class Compare >
typename studilova::AVLTree< Key, Value, Compare >::Node* studilova::AVLTree< Key, Value, Compare >::balance(Node* node)
{
  if (!node)
  {
    return nullptr;
  }

  updateNodeHeight(node);

  int balance = getBalance(node);

  if (balance > 1)
  {
    if (getBalance(node->left_) < 0)
    {
      return const_cast< Node* >(rotateLargeRight(CIt(node, root_)).node_);
    }

    return const_cast< Node* >(rotateRight(CIt(node, root_)).node_);
  }

  if (balance < -1)
  {
    if (getBalance(node->right_) > 0)
    {
      return const_cast< Node* >(rotateLargeLeft(CIt(node, root_)).node_);
    }

    return const_cast< Node* >(rotateLeft(CIt(node, root_)).node_);
  }

  return node;
}

template< class Key, class Value, class Compare >
void studilova::AVLTree< Key, Value, Compare >::rebalanceFrom(Node* node)
{
  while (node)
  {
    Node* parent = node->parent_;

    Node* new_root = balance(node);

    if (!parent)
    {
      root_ = new_root;
    }
    node = parent;
  }

  if (root_)
  {
    root_->parent_ = nullptr;
  }
}

template< class Key, class Value, class Compare >
void studilova::AVLTree< Key, Value, Compare >::swap(AVLTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(cmp_, other.cmp_);
}

#endif
