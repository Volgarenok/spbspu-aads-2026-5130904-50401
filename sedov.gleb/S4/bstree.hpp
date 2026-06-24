#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include "bstiterators.hpp"
#include "treenode.hpp"

namespace sedov
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree & other);
    BSTree(BSTree && other) noexcept;
    ~BSTree();

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other) noexcept;

    bool empty() const;
    size_t size() const;
    void swap(BSTree & other) noexcept;

    void clear() noexcept;

    const Value & at(const Key & k) const;
    Value & at(const Key & k);

    template < class K, class V >
    std::pair< iterator, bool > insert(K && k, V && v);

    Value & operator[](const Key & k);
    size_t erase(const Key & k);
    iterator find(const Key & k);
    const_iterator find(const Key & k) const;
    size_t count(const Key & k) const;

    iterator begin();
    iterator end() noexcept;
    const_iterator begin() const;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
    size_t height() const;
    size_t height(const_iterator it) const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

  private:
    using Node = detail::TreeNode< Key, Value >;
    Node * root_;
    size_t size_;
    Compare comp_;

    Node * clone(Node * src, Node * parent);
    void clearImpl(Node * node) noexcept;

    Node * findNode(const Key & k) const;
    Node * fallLeft(Node * node) const;
    size_t calcHeight(Node * node) const;

    friend class BSTConstIterator< Key, Value >;
    friend class BSTIterator< Key, Value >;
  };
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0),
  comp_()
{
  Node::initFakeLeaf();
  root_ = Node::fakeLeaf;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
  root_(nullptr),
  size_(0),
  comp_(other.comp_)
{
  Node::initFakeLeaf();
  root_ = Node::fakeLeaf;
  root_ = clone(other.root_, Node::fakeLeaf);
  size_ = other.size_;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::BSTree(BSTree && other) noexcept:
  root_(other.root_),
  size_(other.size_),
  comp_(std::move(other.comp_))
{
  Node::initFakeLeaf();
  other.root_ = Node::fakeLeaf;
  other.size_ = 0;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare > & sedov::BSTree< Key, Value, Compare >::operator=(const BSTree & other)
{
  if (this != std::addressof(other))
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template < class Key, class Value, class Compare >
sedov::BSTree< Key, Value, Compare > & sedov::BSTree< Key, Value, Compare >::operator=(BSTree && other) noexcept
{
  if (this != std::addressof(other))
  {
    BSTree temp(std::move(other));
    swap(temp);
  }
  return *this;
}

template < class Key, class Value, class Compare >
bool sedov::BSTree< Key, Value, Compare >::empty() const
{
  return size_ == 0;
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::size() const
{
  return size_;
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::clearImpl(Node * node) noexcept
{
  if (node->isFake())
  {
    return;
  }
  clearImpl(node->left);
  clearImpl(node->right);
  delete node;
}

template < class Key, class Value, class Compare >
void sedov::BSTree< Key, Value, Compare >::clear() noexcept
{
  clearImpl(root_);
  root_ = Node::fakeLeaf;
  size_ = 0;
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::Node * sedov::BSTree< Key, Value, Compare >::clone(Node * src,
  Node * parent)
{
  if (src->isFake())
  {
    return Node::fakeLeaf;
  }
  Node * n = new Node(src->data.first, src->data.second, parent);
  try
  {
    n->left = clone(src->left, n);
    n->right = clone(src->right, n);
  }
  catch (...)
  {
    delete n;
    throw;
  }
  return n;
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::Node *
  sedov::BSTree< Key, Value, Compare >::findNode(const Key & k) const
{
  Node * cur = root_;
  while (!cur->isFake())
  {
    if (comp_(k, cur->data.first))
    {
      cur = cur->left;
    }
    else if (comp_(cur->data.first, k))
    {
      cur = cur->right;
    }
    else
    {
      return cur;
    }
  }
  return nullptr;
}

template < class Key, class Value, class Compare >
const Value & sedov::BSTree< Key, Value, Compare >::at(const Key & k) const
{
  Node * n = findNode(k);
  if (n == nullptr)
  {
    throw std::out_of_range("Key not found");
  }
  return n->data.second;
}

template < class Key, class Value, class Compare >
Value & sedov::BSTree< Key, Value, Compare >::at(const Key & k)
{
  Node * n = findNode(k);
  if (n == nullptr)
  {
    throw std::out_of_range("Key not found");
  }
  return n->data.second;
}

template < class Key, class Value, class Compare >
template < class K, class V >
std::pair< typename sedov::BSTree< Key, Value, Compare >::iterator, bool >
  sedov::BSTree< Key, Value, Compare >::insert(K && k, V && v)
{
  Node * parent = nullptr;
  Node * cur = root_;
  while (!cur->isFake())
  {
    if (!comp_(cur->data.first, k) && !comp_(k, cur->data.first))
    {
      cur->data.second = std::forward< V >(v);
      return std::make_pair(iterator(cur), false);
    }
    if (comp_(k, cur->data.first))
    {
      parent = cur;
      cur = cur->left;
    }
    else
    {
      parent = cur;
      cur = cur->right;
    }
  }
  Node * newNode = new Node(std::forward< K >(k), std::forward< V >(v), Node::fakeLeaf);
  if (parent != nullptr)
  {
    if (comp_(k, parent->data.first))
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    newNode->parent = parent;
  }
  else
  {
    root_ = newNode;
  }
  ++size_;
  return std::make_pair(iterator(newNode), true);
}

template < class Key, class Value, class Compare >
Value & sedov::BSTree< Key, Value, Compare >::operator[](const Key & k)
{
  Node * n = findNode(k);
  if (n != nullptr)
  {
    return n->data.second;
  }
  std::pair< iterator, bool > result = insert(k, Value{});
  return (*result.first).second;
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::erase(const Key & k)
{
  Node * node = findNode(k);
  if (node == nullptr)
  {
    return 0;
  }
  Value res = std::move(node->data.second);
  if (!node->left->isFake() && !node->right->isFake())
  {
    Node * succ = fallLeft(node->right);
    node->data.first = std::move(succ->data.first);
    node->data.second = std::move(succ->data.second);
    node = succ;
  }
  Node * child = (!node->left->isFake()) ? node->left : node->right;
  if (child->isFake())
  {
    child = Node::fakeLeaf;
  }
  else
  {
    child->parent = node->parent;
  }
  if (node->parent->isFake())
  {
    root_ = child;
  }
  else if (node->parent->left == node)
  {
    node->parent->left = child;
  }
  else
  {
    node->parent->right = child;
  }
  delete node;
  --size_;
  return 1;
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::iterator sedov::BSTree< Key, Value, Compare >::find(const Key & k)
{
  Node * n = findNode(k);
  return (n != nullptr) ? iterator(n) : end();
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::find(const Key & k) const
{
  Node * n = findNode(k);
  return (n != nullptr) ? const_iterator(n) : end();
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::count(const Key & k) const
{
  return (findNode(k) != nullptr) ? 1 : 0;
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::Node * sedov::BSTree< Key, Value, Compare >::fallLeft(Node * node) const
{
  while (!node->left->isFake())
  {
    node = node->left;
  }
  return node;
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::iterator sedov::BSTree< Key, Value, Compare >::begin()
{
  if (root_->isFake())
  {
    return iterator(nullptr);
  }
  return iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::iterator sedov::BSTree< Key, Value, Compare >::end() noexcept
{
  return iterator(nullptr);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator sedov::BSTree< Key, Value, Compare >::begin() const
{
  if (root_->isFake())
  {
    return const_iterator(nullptr);
  }
  return const_iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::end() const noexcept
{
  return const_iterator(nullptr);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  if (root_->isFake())
  {
    return const_iterator(nullptr);
  }
  return const_iterator(fallLeft(root_));
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return const_iterator(nullptr);
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::calcHeight(Node * node) const
{
  if (node->isFake())
  {
    return 0;
  }
  size_t l = calcHeight(node->left);
  size_t r = calcHeight(node->right);
  return 1 + ((l > r) ? l : r);
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::height() const
{
  return calcHeight(root_);
}

template < class Key, class Value, class Compare >
size_t sedov::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return calcHeight(it.node_);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  Node * x = it.node_;
  if (x->isFake() || x->right->isFake())
  {
    return it;
  }
  Node * y = x->right;
  x->right = y->left;
  if (!y->left->isFake())
  {
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent->isFake())
  {
    root_ = y;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = y;
  }
  else
  {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
  return const_iterator(y);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  Node * y = it.node_;
  if (y->isFake() || y->left->isFake())
  {
    return it;
  }
  Node * x = y->left;
  y->left = x->right;
  if (!x->right->isFake())
  {
    x->right->parent = y;
  }
  x->parent = y->parent;
  if (y->parent->isFake())
  {
    root_ = x;
  }
  else if (y == y->parent->left)
  {
    y->parent->left = x;
  }
  else
  {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
  return const_iterator(x);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  Node * node = it.node_;
  if (node->isFake() || node->left->isFake() || node->left->right->isFake())
  {
    return it;
  }
  rotateRight(const_iterator(node->left));
  return rotateLeft(it);
}

template < class Key, class Value, class Compare >
typename sedov::BSTree< Key, Value, Compare >::const_iterator
  sedov::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  Node * node = it.node_;
  if (node->isFake() || node->right->isFake() || node->right->left->isFake())
  {
    return it;
  }
  rotateLeft(const_iterator(node->right));
  return rotateRight(it);
}

#endif
