#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <memory>
#include <stdexcept>
#include <utility>
#include "bst-node.hpp"
#include "bst-iter.hpp"
#include "cbst-iter.hpp"

namespace kondrat
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = CBSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree & other);
    BSTree(BSTree && other) noexcept;
    ~BSTree();

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other) noexcept;

    void push(const Key & key, const Value & value);
    void push(const Key & key, Value && value);
    size_t erase(const Key & key);
    void clear() noexcept;
    void swap(BSTree & other) noexcept;

    Value & at(const Key & key);
    const Value & at(const Key & key) const;
    Value & operator[](const Key & key);
    bool contains(const Key & key) const;
    size_t height() const;
    size_t height(const_iterator it) const;
    size_t size() const noexcept;
    bool empty() const noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

  private:
    using node_t = detail::Node< Key, Value >;

    node_t * findNode(const Key & key) const;
    node_t * getMin(node_t * node) const noexcept;
    node_t * getMax(node_t * node) const noexcept;
    void transplant(node_t * oldNode, node_t * newNode) noexcept;
    size_t getHeight(node_t * node) const;
    node_t * clone(const node_t * other, node_t * parent);
    void clear(node_t * node) noexcept;

    template< class V >
    void pushImpl(const Key & key, V && value);

    node_t * root_;
    size_t size_;
    Compare comp_;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr),
    size_(0),
    comp_()
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
    root_(nullptr),
    size_(other.size_),
    comp_(other.comp_)
  {
    root_ = clone(other.root_, nullptr);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree && other) noexcept:
    root_(std::exchange(other.root_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    comp_(std::move(other.comp_))
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(const BSTree & other)
  {
    if (this != std::addressof(other))
    {
      BSTree temp(other);
      swap(temp);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(BSTree && other) noexcept
  {
    if (this != std::addressof(other))
    {
      BSTree temp(std::move(other));
      swap(temp);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key & key, const Value & value)
  {
    pushImpl(key, value);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key & key, Value && value)
  {
    pushImpl(key, std::move(value));
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::erase(const Key & key)
  {
    node_t * node = findNode(key);

    if (node == nullptr)
    {
      return 0;
    }

    if (node->left_ == nullptr)
    {
      transplant(node, node->right_);
    }
    else if (node->right_ == nullptr)
    {
      transplant(node, node->left_);
    }
    else
    {
      node_t * next = getMin(node->right_);

      if (next->parent_ != node)
      {
        transplant(next, next->right_);
        next->right_ = node->right_;
        next->right_->parent_ = next;
      }

      transplant(node, next);
      next->left_ = node->left_;
      next->left_->parent_ = next;
    }

    delete node;
    --size_;

    return 1;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear() noexcept
  {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::at(const Key & key)
  {
    node_t * node = findNode(key);

    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::at(const Key & key) const
  {
    node_t * node = findNode(key);

    if (node == nullptr)
    {
      throw std::logic_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::operator[](const Key & key)
  {
    node_t * node = findNode(key);

    if (node == nullptr)
    {
      push(key, Value{});
      node = findNode(key);
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return getHeight(root_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return getHeight(it.node_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::begin()
  {
    if (root_ == nullptr)
    {
      return end();
    }

    return iterator(getMin(root_), root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::end()
  {
    return iterator(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::begin() const
  {
    if (root_ == nullptr)
    {
      return end();
    }

    return const_iterator(getMin(root_), root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    node_t * x = it.node_;

    if (x == nullptr || x->right_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    node_t * y = x->right_;

    x->right_ = y->left_;

    if (y->left_ != nullptr)
    {
      y->left_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == nullptr)
    {
      root_ = y;
    }
    else if (x == x->parent_->left_)
    {
      x->parent_->left_ = y;
    }
    else
    {
      x->parent_->right_ = y;
    }

    y->left_ = x;
    x->parent_ = y;

    return const_iterator(y, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    node_t * x = it.node_;

    if (x == nullptr || x->left_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    node_t * y = x->left_;

    x->left_ = y->right_;

    if (y->right_ != nullptr)
    {
      y->right_->parent_ = x;
    }

    y->parent_ = x->parent_;

    if (x->parent_ == nullptr)
    {
      root_ = y;
    }
    else if (x == x->parent_->left_)
    {
      x->parent_->left_ = y;
    }
    else
    {
      x->parent_->right_ = y;
    }

    y->right_ = x;
    x->parent_ = y;

    return const_iterator(y, root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    node_t * x = it.node_;

    if (x == nullptr || x->right_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    rotateRight(const_iterator(x->right_, root_));
    return rotateLeft(const_iterator(x, root_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    node_t * x = it.node_;

    if (x == nullptr || x->left_ == nullptr)
    {
      throw std::logic_error("invalid rotation");
    }

    rotateLeft(const_iterator(x->left_, root_));
    return rotateRight(const_iterator(x, root_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::node_t *
  BSTree< Key, Value, Compare >::findNode(const Key & key) const
  {
    node_t * current = root_;

    while (current != nullptr)
    {
      if (comp_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (comp_(current->data_.first, key))
      {
        current = current->right_;
      }
      else
      {
        return current;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::node_t *
  BSTree< Key, Value, Compare >::getMin(node_t * node) const noexcept
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    while (node->left_ != nullptr)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::node_t *
  BSTree< Key, Value, Compare >::getMax(node_t * node) const noexcept
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    while (node->right_ != nullptr)
    {
      node = node->right_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::transplant(node_t * oldNode, node_t * newNode) noexcept
  {
    if (oldNode->parent_ == nullptr)
    {
      root_ = newNode;
    }
    else if (oldNode == oldNode->parent_->left_)
    {
      oldNode->parent_->left_ = newNode;
    }
    else
    {
      oldNode->parent_->right_ = newNode;
    }

    if (newNode != nullptr)
    {
      newNode->parent_ = oldNode->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::getHeight(node_t * node) const
  {
    if (node == nullptr)
    {
      return 0;
    }

    size_t leftHeight = getHeight(node->left_);
    size_t rightHeight = getHeight(node->right_);

    return std::max(leftHeight, rightHeight) + 1;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::node_t *
  BSTree< Key, Value, Compare >::clone(const node_t * other, node_t * parent)
  {
    if (other == nullptr)
    {
      return nullptr;
    }

    node_t * node = new node_t(other->data_.first, other->data_.second);
    node->parent_ = parent;

    try
    {
      node->left_ = clone(other->left_, node);
      node->right_ = clone(other->right_, node);
    }
    catch (...)
    {
      clear(node);
      throw;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(node_t * node) noexcept
  {
    if (node == nullptr)
    {
      return;
    }

    clear(node->left_);
    clear(node->right_);

    delete node;
  }

  template< class Key, class Value, class Compare >
  template< class V >
  void BSTree< Key, Value, Compare >::pushImpl(const Key & key, V && value)
  {
    node_t * parent = nullptr;
    node_t * current = root_;

    while (current != nullptr)
    {
      parent = current;

      if (comp_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (comp_(current->data_.first, key))
      {
        current = current->right_;
      }
      else
      {
        current->data_.second = std::forward< V >(value);
        return;
      }
    }

    std::unique_ptr< node_t > node(new node_t(key, std::forward< V >(value)));
    node->parent_ = parent;

    if (parent == nullptr)
    {
      root_ = node.release();
    }
    else if (comp_(key, parent->data_.first))
    {
      parent->left_ = node.release();
    }
    else
    {
      parent->right_ = node.release();
    }

    ++size_;
  }
}

#endif
