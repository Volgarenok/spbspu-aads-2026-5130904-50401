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
    TreeNode< Key, Value >* root_;
    size_t size_;
    Compare compare_;
    void clear(TreeNode< Key, Value >* node) noexcept;
    TreeNode< Key, Value >* copy(const TreeNode< Key, Value >* node, TreeNode< Key, Value >* parent);
    size_t height(TreeNode< Key, Value >* node) const noexcept;
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
    zhuravleva::TreeNode< Key, Value >* newRoot = copy(other.root_, nullptr);
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
  zhuravleva::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return zhuravleva::BSTIterator< Key, Value >(nullptr);
  }
  while (current->left_)
  {
    current = current->left_;
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
  const zhuravleva::TreeNode< Key, Value >* current = root_;
  if (!current)
  {
    return zhuravleva::BSTConstIterator< Key, Value >(nullptr);
  }
  while (current->left_)
  {
    current = current->left_;
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
  zhuravleva::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data_.first))
    {
      node = node->left_;
    }
    else if (compare_(node->data_.first, key))
    {
      node = node->right_;
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
  const zhuravleva::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data_.first))
    {
      node = node->left_;
    }
    else if (compare_(node->data_.first, key))
    {
      node = node->right_;
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
    root_ = new zhuravleva::TreeNode< Key, Value >(key, value);
    size_++;
    return;
  }
  zhuravleva::TreeNode< Key, Value >* node = root_;
  zhuravleva::TreeNode< Key, Value >* parent = nullptr;
  while (node)
  {
    parent = node;
    if (compare_(key, node->data_.first))
    {
      node = node->left_;
    }
    else if (compare_(node->data_.first, key))
    {
      node = node->right_;
    }
    else
    {
      node->data_.second = value;
      return;
    }
  }
  zhuravleva::TreeNode< Key, Value >* newNode = new zhuravleva::TreeNode< Key, Value >(key, value, parent);
  if (compare_(key, parent->data_.first))
  {
    parent->left_ = newNode;
  }
  else
  {
    parent->right_ = newNode;
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
  zhuravleva::TreeNode< Key, Value >* node = root_;
  while (node)
  {
    if (compare_(key, node->data_.first))
    {
      node = node->left_;
    }
    else if (compare_(node->data_.first, key))
    {
      node = node->right_;
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
  if (node->left_ && node->right_)
  {
    zhuravleva::TreeNode< Key, Value >* successor = node->right_;
    while (successor->left_)
    {
      successor = successor->left_;
    }
    node->data_ = successor->data_;
    node = successor;
  }
  zhuravleva::TreeNode< Key, Value >* child = node->left_;
  if (!child)
  {
    child = node->right_;
  }
  if (child)
  {
    child->parent_ = node->parent_;
  }
  if (!node->parent_)
  {
    root_ = child;
  }
  else if (node == node->parent_->left_)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
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
  return height(const_cast< zhuravleva::TreeNode< Key, Value >* >(it.current_));
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateLeft(constIterator it) noexcept
{
  zhuravleva::TreeNode< Key, Value >* node = const_cast< zhuravleva::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent_)
  {
    return cend();
  }
  zhuravleva::TreeNode< Key, Value >* parent = node->parent_;
  if (parent->right_ != node)
  {
    return cend();
  }
  zhuravleva::TreeNode< Key, Value >* middle = node->left_;
  parent->right_ = middle;
  if (middle)
  {
    middle->parent_ = parent;
  }
  node->parent_ = parent->parent_;
  if (!parent->parent_)
  {
    root_ = node;
  }
  else if (parent == parent->parent_->left_)
  {
    parent->parent_->left_ = node;
  }
  else
  {
    parent->parent_->right_ = node;
  }
  node->left_ = parent;
  parent->parent_ = node;
  return zhuravleva::BSTConstIterator< Key, Value >(node);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateRight(constIterator it) noexcept
{
  zhuravleva::TreeNode< Key, Value >* node = const_cast< zhuravleva::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent_)
  {
    return cend();
  }
  zhuravleva::TreeNode< Key, Value >* parent = node->parent_;
  if (parent->left_ != node)
  {
    return cend();
  }
  zhuravleva::TreeNode< Key, Value >* middle = node->right_;
  parent->left_ = middle;
  if (middle)
  {
    middle->parent_ = parent;
  }
  node->parent_ = parent->parent_;
  if (!parent->parent_)
  {
    root_ = node;
  }
  else if (parent == parent->parent_->left_)
  {
    parent->parent_->left_ = node;
  }
  else
  {
    parent->parent_->right_ = node;
  }

  node->right_ = parent;
  parent->parent_ = node;

  return zhuravleva::BSTConstIterator< Key, Value >(node);
}

template< class Key, class Value, class Compare >
zhuravleva::BSTConstIterator< Key, Value >
zhuravleva::BSTree< Key, Value, Compare >::rotateLargeLeft(constIterator it) noexcept
{
  zhuravleva::TreeNode< Key, Value >* node = const_cast< zhuravleva::TreeNode< Key, Value >* >(it.current_);
  if (!node || !node->parent_ || !node->parent_->parent_)
  {
    return cend();
  }
  if (node != node->parent_->left_)
  {
    return cend();
  }
  if (node->parent_ != node->parent_->parent_->right_)
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
  zhuravleva::TreeNode< Key, Value >* node = const_cast< zhuravleva::TreeNode< Key, Value >* >(it.current_);

  if (!node || !node->parent_ || !node->parent_->parent_)
  {
    return cend();
  }
  if (node != node->parent_->right_)
  {
    return cend();
  }
  if (node->parent_ != node->parent_->parent_->left_)
  {
    return cend();
  }
  rotateLeft(it);
  return rotateRight(zhuravleva::BSTConstIterator< Key, Value >(node));
}

template< class Key, class Value, class Compare >
void zhuravleva::BSTree< Key, Value, Compare >::clear(zhuravleva::TreeNode< Key, Value >* node) noexcept
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
zhuravleva::TreeNode< Key, Value >* zhuravleva::BSTree< Key, Value, Compare >::copy
  (const zhuravleva::TreeNode< Key, Value >* node, zhuravleva::TreeNode< Key, Value >* parent)
{
  if (!node)
  {
    return nullptr;
  }
  zhuravleva::TreeNode< Key, Value >* newNode = new zhuravleva::TreeNode< Key, Value >
    (node->data_.first, node->data_.second, parent);
  newNode->left_ = copy(node->left_, newNode);
  newNode->right_ = copy(node->right_, newNode);
  return newNode;
}

template< class Key, class Value, class Compare >
size_t zhuravleva::BSTree< Key, Value, Compare >::height(zhuravleva::TreeNode< Key, Value >* node) const noexcept
{
  if (!node)
  {
    return 0;
  }
  size_t leftHeight = height(node->left_);
  size_t rightHeight = height(node->right_);
  size_t maxHeight = leftHeight;
  if (rightHeight > maxHeight)
  {
    maxHeight = rightHeight;
  }
  return maxHeight + 1;
}

#endif
