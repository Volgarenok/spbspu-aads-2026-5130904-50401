#ifndef BSTREE_HPP
#define BSTREE_HPP

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
    ~BSTree();

    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

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

    Value& get(const Key& key);
    const Value& get(const Key& key) const;

    void drop(const Key& key);
    void clear() noexcept;
    size_t height() const noexcept;
    size_t height(constIterator it) const noexcept;

    constIterator rotateLeft(constIterator it) noexcept;
    constIterator rotateRight(constIterator it) noexcept;
    constIterator rotateLargeLeft(constIterator it) noexcept;
    constIterator rotateLargeRight(constIterator it) noexcept;

  private:
    detail::TreeNode< Key, Value >* root_;
    size_t size_;
    Compare compare_;
    void clear(detail::TreeNode< Key, Value >* node) noexcept;
    detail::TreeNode< Key, Value >* copy(const detail::TreeNode< Key, Value >* node, detail::TreeNode< Key, Value >* parent);
    size_t height(detail::TreeNode< Key, Value >* node) const noexcept;
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
  root_(other.root_),
  size_(other.size_),
  compare_(other.compare_)
{
  other.root_ = nullptr;
  other.size_ = 0;
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >&
zhuravleva::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != &other)
  {
    zhuravleva::detail::TreeNode< Key, Value >* newRoot = copy(other.root_, nullptr);
    clear();
    root_ = newRoot;
    size_ = other.size_;
    compare_ = other.compare_;
  }
  return *this;
}

template< class Key, class Value, class Compare >
zhuravleva::BSTree< Key, Value, Compare >&
zhuravleva::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != &other)
  {
    clear();
    root_ = other.root_;
    size_ = other.size_;
    compare_ = other.compare_;
    other.root_ = nullptr;
    other.size_ = 0;
  }
  return *this;
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
zhuravleva::BSTIterator< Key, Value > zhuravleva::BSTree< Key, Value, Compare >::begin() noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return zhuravleva::BSTIterator< Key, Value >(nullptr);
  }
  while (current->left)
  {
    current = current->left;
  }
  return zhuravleva::BSTIterator< Key, Value >(current);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTIterator< Key, Value > zhuravleva::BSTree< Key, Value, Compare >::end() noexcept
{
  return zhuravleva::BSTIterator< Key, Value >(nullptr);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value > zhuravleva::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  const zhuravleva::detail::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return zhuravleva::BSTConstIterator< Key, Value >(nullptr);
  }
  while (current->left)
  {
    current = current->left;
  }
  return zhuravleva::BSTConstIterator< Key, Value >(current);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value > zhuravleva::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return zhuravleva::BSTConstIterator< Key, Value >(nullptr);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::find(const Key& key) noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* node = root_;
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
      return zhuravleva::BSTIterator< Key, Value >(node);
    }
  }
  return end();
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::find(const Key& key) const noexcept
{
  const zhuravleva::detail::TreeNode< Key, Value >* node = root_;
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
      return zhuravleva::BSTConstIterator< Key, Value >(node);
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
    root_ = new zhuravleva::detail::TreeNode< Key, Value >(key, value);
    size_++;
    return;
  }
  zhuravleva::detail::TreeNode< Key, Value >* node = root_;
  zhuravleva::detail::TreeNode< Key, Value >* parent = nullptr;
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
  zhuravleva::detail::TreeNode< Key, Value >* newNode = new zhuravleva::detail::TreeNode< Key, Value >(key, value, parent);
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
Value& zhuravleva::BSTree< Key, Value, Compare >::get(const Key& key)
{
  zhuravleva::BSTIterator< Key, Value > it = find(key);
  if (it == end())
  {
    throw std::runtime_error("key not found");
  }
  return it->second;
}

template< class Key, class Value, class Compare >
const Value& zhuravleva::BSTree< Key, Value, Compare >::get(const Key& key) const
{
  zhuravleva::BSTConstIterator< Key, Value > it = find(key);
  if (it == cend())
  {
    throw std::runtime_error("key not found");
  }
  return it->second;
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::drop(const Key& key)
{
  zhuravleva::detail::TreeNode< Key, Value >* node = root_;
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
    throw std::runtime_error("key not found");
  }
  if (node->left && node->right)
  {
    zhuravleva::detail::TreeNode< Key, Value >* successor = node->right;
    while (successor->left)
    {
      successor = successor->left;
    }
    node->data = successor->data;
    node = successor;
  }
  zhuravleva::detail::TreeNode< Key, Value >* child = node->left;
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
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::clear() noexcept
{
  clear(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height() const noexcept
{
  return height(root_);
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height(constIterator it) const noexcept
{
  return height(const_cast< zhuravleva::detail::TreeNode< Key, Value >* >(it.current_));
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateLeft(constIterator it) noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* node = const_cast< zhuravleva::detail::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent)
  {
    return cend();
  }
  zhuravleva::detail::TreeNode< Key, Value >* parent = node->parent;
  if (parent->right != node)
  {
    return cend();
  }
  zhuravleva::detail::TreeNode< Key, Value >* middle = node->left;
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
  return zhuravleva::BSTConstIterator< Key, Value >(node);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateRight(constIterator it) noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* node = const_cast< zhuravleva::detail::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent)
  {
    return cend();
  }
  zhuravleva::detail::TreeNode< Key, Value >* parent = node->parent;
  if (parent->left != node)
  {
    return cend();
  }
  zhuravleva::detail::TreeNode< Key, Value >* middle = node->right;
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

  return zhuravleva::BSTConstIterator< Key, Value >(node);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateLargeLeft(constIterator it) noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* node = const_cast< zhuravleva::detail::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent || !node->parent->parent)
  {
    return cend();
  }
  if (node != node->parent->left)
  {
    return cend();
  }
  if (node->parent != node->parent->parent->right)
  {
    return cend();
  }
  rotateRight(it);
  return rotateLeft(zhuravleva::BSTConstIterator< Key, Value >(node));
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateLargeRight(constIterator it) noexcept
{
  zhuravleva::detail::TreeNode< Key, Value >* node = const_cast< zhuravleva::detail::TreeNode< Key, Value >* >(it.current_);

  if (!node || !node->parent || !node->parent->parent)
  {
    return cend();
  }
  if (node != node->parent->right)
  {
    return cend();
  }
  if (node->parent != node->parent->parent->left)
  {
    return cend();
  }
  rotateLeft(it);
  return rotateRight(zhuravleva::BSTConstIterator< Key, Value >(node));
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::clear(zhuravleva::detail::TreeNode< Key, Value >* node) noexcept
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
  (const zhuravleva::detail::TreeNode< Key, Value >* node, zhuravleva::detail::TreeNode< Key, Value >* parent)
{
  if (!node)
  {
    return nullptr;
  }
  zhuravleva::detail::TreeNode< Key, Value >* newNode = new zhuravleva::detail::TreeNode< Key, Value >
    (node->data.first, node->data.second, parent);
  newNode->left = copy(node->left, newNode);
  newNode->right = copy(node->right, newNode);
  return newNode;
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height(zhuravleva::detail::TreeNode< Key, Value >* node) const noexcept
{
  if (!node)
  {
    return 0;
  }
  size_t leftHeight = height(node->left);
  size_t rightHeight = height(node->right);
  size_t maxHeight = leftHeight;
  if (rightHeight > maxHeight)
  {
    maxHeight = rightHeight;
  }
  return maxHeight + 1;
}

#endif
