#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <algorithm>
#include <functional>
#include <cstddef>
#include <stdexcept>
#include <utility>
#include "treeNode.hpp"
#include "treeIterators.hpp"

namespace zhuravleva
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using valueType = std::pair< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using constIterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;
    Value& operator[](const Key& key);

    bool empty() const noexcept;
    size_t size() const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    constIterator cbegin() const noexcept;
    constIterator cend() const noexcept;

    iterator find(const Key& key) noexcept;
    constIterator find(const Key& key) const noexcept;

    bool contains(const Key& key) const noexcept;
    void push(const Key& key, const Value& value);
    void push(const Key& key, Value&& value);


    Value& at(const Key& key);
    const Value& at(const Key& key) const;

    Value& get(const Key& key);
    const Value& get(const Key& key) const;

    size_t drop(const Key& key);
    void clear() noexcept;
    void swap(BSTree& other) noexcept;
    size_t height() const noexcept;
    size_t height(constIterator it) const noexcept;

    iterator rotateLeft(iterator it) noexcept;
    iterator rotateRight(iterator it) noexcept;
    iterator rotateLargeLeft(iterator it) noexcept;
    iterator rotateLargeRight(iterator it) noexcept;

  private:
    detail::TreeNode< Key, Value >* root_;
    size_t size_;
    Compare compare_;
    void clear(detail::TreeNode< Key, Value >* node) noexcept;
    detail::TreeNode< Key, Value >* copy(const detail::TreeNode< Key, Value >* node, detail::TreeNode< Key, Value >* parent);
    size_t height(const detail::TreeNode< Key, Value >* node) const noexcept;
  };
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0),
  compare_(Compare())
{}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(nullptr),
  size_(other.size_),
  compare_(other.compare_)
{
  root_ = copy(other.root_, nullptr);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(std::exchange(other.root_, nullptr)),
  size_(std::exchange(other.size_, 0)),
  compare_(other.compare_)
{}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >&
    zhuravleva::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  BSTree temp(other);
  swap(temp);
  return *this;
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >&
    zhuravleva::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  BSTree temp(std::move(other));
  swap(temp);
  return *this;
}

template< class Key, class Value, class Compare >
Value& zhuravleva::BSTree< Key, Value, Compare >::operator[](const Key& key)
{
  iterator it = find(key);
  if (it == end())
  {
    push(key, Value());
    it = find(key);
  }
  return it->second;
}

template< class Key, class Value, class Compare >
bool zhuravleva::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::begin() noexcept
{
  detail::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return iterator(nullptr);
  }
  while (current->left)
  {
    current = current->left;
  }
  return iterator(current);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::constIterator
zhuravleva::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  const detail::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return constIterator(nullptr);
  }
  while (current->left)
  {
    current = current->left;
  }
  return constIterator(current);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::constIterator
zhuravleva::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return constIterator(nullptr);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::find(const Key& key) noexcept
{
  detail::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data.first))
    {
      node = node->left;
    }
    else if (compare_(node->data.first, key))
    {
      node = node->right;
    }
    else
    {
      return iterator(node);
    }
  }
  return end();
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::constIterator
zhuravleva::BSTree< Key, Value, Compare >::find(const Key& key) const noexcept
{
  const detail::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data.first))
    {
      node = node->left;
    }
    else if (compare_(node->data.first, key))
    {
      node = node->right;
    }
    else
    {
      return constIterator(node);
    }
  }
  return cend();
}

template< class Key, class Value, class Compare >
bool zhuravleva::BSTree< Key, Value, Compare >::contains(const Key& key) const noexcept
{
  return find(key) != cend();
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::push(const Key& key, const Value& value)
{
  if (!root_)
  {
    root_ = new detail::TreeNode< Key, Value >(key, value);
    size_++;
    return;
  }
  detail::TreeNode< Key, Value >* node = root_;
  detail::TreeNode< Key, Value >* parent = nullptr;
  while (node)
  {
    parent = node;
    if (compare_(key, node->data.first))
    {
      node = node->left;
    }
    else if (compare_(node->data.first, key))
    {
      node = node->right;
    }
    else
    {
      node->data.second = value;
      return;
    }
  }
  detail::TreeNode< Key, Value >* newNode = new detail::TreeNode< Key, Value >(key, value, parent);
  if (compare_(key, parent->data.first))
  {
    parent->left = newNode;
  }
  else
  {
    parent->right = newNode;
  }
  size_++;
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::push(const Key& key, Value&& value)
{
  if (!root_)
  {
    root_ = new detail::TreeNode< Key, Value >(key, std::move(value));
    ++size_;
    return;
  }
  detail::TreeNode< Key, Value >* node = root_;
  detail::TreeNode< Key, Value >* parent = nullptr;
  while (node)
  {
    parent = node;
    if (compare_(key, node->data.first))
    {
      node = node->left;
    }
    else if (compare_(node->data.first, key))
    {
      node = node->right;
    }
    else
    {
      node->data.second = std::move(value);
      return;
    }
  }
  detail::TreeNode< Key, Value >* newNode =
    new detail::TreeNode< Key, Value >(key, std::move(value), parent);

  if (compare_(key, parent->data.first))
  {
    parent->left = newNode;
  }
  else
  {
    parent->right = newNode;
  }
  ++size_;
}

template< class Key, class Value, class Compare >
Value& zhuravleva::BSTree< Key, Value, Compare >::at(const Key& key)
{
  iterator it = find(key);
  if (it == end())
  {
    throw std::out_of_range("key not found");
  }
  return it->second;
}

template< class Key, class Value, class Compare >
const Value& zhuravleva::BSTree< Key, Value, Compare >::at(const Key& key) const
{
  constIterator it = find(key);
  if (it == cend())
  {
    throw std::out_of_range("key not found");
  }
  return it->second;
}

template< class Key, class Value, class Compare >
Value& zhuravleva::BSTree< Key, Value, Compare >::get(const Key& key)
{
  return at(key);
}

template< class Key, class Value, class Compare >
const Value& zhuravleva::BSTree< Key, Value, Compare >::get(const Key& key) const
{
  return at(key);
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::drop(const Key& key)
{
  detail::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data.first))
    {
      node = node->left;
    }
    else if (compare_(node->data.first, key))
    {
      node = node->right;
    }
    else
    {
      break;
    }
  }
  if (!node)
  {
    return 0;
  }
  if (node->left && node->right)
  {
    detail::TreeNode< Key, Value >* successor = node->right;
    while (successor->left)
    {
      successor = successor->left;
    }
    node->data = successor->data;
    node = successor;
  }
  detail::TreeNode< Key, Value >* child = node->left;
  if (!child)
  {
    child = node->right;
  }
  if (child)
  {
    child->parent = node->parent;
  }
  if (!node->parent)
  {
    root_ = child;
  }
  else if (node == node->parent->left)
  {
    node->parent->left = child;
  }
  else
  {
    node->parent->right = child;
  }
  delete node;
  size_--;
  return 1;
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::clear() noexcept
{
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(compare_, other.compare_);
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height() const noexcept
{
  return height(root_);
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height(constIterator it) const noexcept
{
  return height(it.current_);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::rotateLeft(iterator it) noexcept
{
  detail::TreeNode< Key, Value >* node = it.current_;
  if (!node || !node->parent)
  {
    return end();
  }
  detail::TreeNode< Key, Value >* parent = node->parent;
  if (parent->right != node)
  {
    return end();
  }
  detail::TreeNode< Key, Value >* middle = node->left;
  parent->right = middle;
  if (middle)
  {
    middle->parent = parent;
  }
  node->parent = parent->parent;
  if (!parent->parent)
  {
    root_ = node;
  }
  else if (parent == parent->parent->left)
  {
    parent->parent->left = node;
  }
  else
  {
    parent->parent->right = node;
  }
  node->left = parent;
  parent->parent = node;
  return iterator(node);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::rotateRight(iterator it) noexcept
{
  detail::TreeNode< Key, Value >* node = it.current_;
  if (!node || !node->parent)
  {
    return end();
  }
  detail::TreeNode< Key, Value >* parent = node->parent;
  if (parent->left != node)
  {
    return end();
  }
  detail::TreeNode< Key, Value >* middle = node->right;
  parent->left = middle;
  if (middle)
  {
    middle->parent = parent;
  }
  node->parent = parent->parent;
  if (!parent->parent)
  {
    root_ = node;
  }
  else if (parent == parent->parent->left)
  {
    parent->parent->left = node;
  }
  else
  {
    parent->parent->right = node;
  }

  node->right = parent;
  parent->parent = node;
  return iterator(node);
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::rotateLargeLeft(iterator it) noexcept
{
  detail::TreeNode< Key, Value >* node = it.current_;
  if (!node || !node->parent || !node->parent->parent)
  {
    return end();
  }
  if (node != node->parent->left)
  {
    return end();
  }
  if (node->parent != node->parent->parent->right)
  {
    return end();
  }
  rotateRight(it);
  return rotateLeft(iterator(node));
}

template< class Key, class Value, class Compare >
typename zhuravleva::BSTree< Key, Value, Compare >::iterator
zhuravleva::BSTree< Key, Value, Compare >::rotateLargeRight(iterator it) noexcept
{
  detail::TreeNode< Key, Value >* node = it.current_;

  if (!node || !node->parent || !node->parent->parent)
  {
    return end();
  }
  if (node != node->parent->right)
  {
    return end();
  }
  if (node->parent != node->parent->parent->left)
  {
    return end();
  }
  rotateLeft(it);
  return rotateRight(iterator(node));
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::clear(detail::TreeNode< Key, Value >* node) noexcept
{
  if (!node)
  {
    return;
  }
  clear(node->left);
  clear(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
zhuravleva::detail::TreeNode< Key, Value >* zhuravleva::BSTree< Key, Value, Compare >::copy
    (const detail::TreeNode< Key, Value >* node, detail::TreeNode< Key, Value >* parent)
{
  if (!node)
  {
    return nullptr;
  }
  std::unique_ptr< detail::TreeNode< Key, Value > > newNode(
    new detail::TreeNode< Key, Value >(node->data.first, node->data.second, parent));
  try
  {
    newNode->left = copy(node->left, newNode.get());
    newNode->right = copy(node->right, newNode.get());
  }
  catch (...)
  {
    clear(newNode->left);
    clear(newNode->right);
    throw;
  }
  return newNode.release();
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height(const detail::TreeNode< Key, Value >* node) const noexcept
{
  if (!node)
  {
    return 0;
  }
  size_t leftHeight = height(node->left);
  size_t rightHeight = height(node->right);
  return std::max(leftHeight, rightHeight) + 1;
}

#endif
