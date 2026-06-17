#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <stdexcept>
#include <cstddef>
#include <functional>
#include <algorithm>
#include <memory>
#include "node.hpp"
#include "treeIters.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
    using iterator = AVLIterator< Key, Value >;
    using const_iterator = AVLConstIterator< Key, Value >;

    explicit AVLTree(const Compare& cmp = Compare());
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    ~AVLTree() noexcept;
    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    void insert(const Key& key, const Value& value);
    bool remove(const Key& key);
    void clear() noexcept;

    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    bool has(const Key& key) const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;

    iterator lower_bound(const Key& key) noexcept;
    const_iterator lower_bound(const Key& key) const noexcept;

    iterator upper_bound(const Key& key) noexcept;
    const_iterator upper_bound(const Key& key) const noexcept;

    std::pair< iterator, iterator > equal_range(const Key& key) noexcept;
    std::pair< const_iterator, const_iterator > equal_range(const Key& key) const noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;
  private:
    using Node = vasyakin::detail::Node< Key, Value >;
    Node* root_;
    size_t size_;
    Compare cmp_;

    int getHeight(const Node* node) const noexcept;
    int getBalance(const Node* node) const noexcept;
    void updateHeight(Node* node) noexcept;

    Node* rotateLeft(Node* node) noexcept;
    Node* rotateRight(Node* node) noexcept;
    Node* rotateLargeLeft(Node* node) noexcept;
    Node* rotateLargeRight(Node* node) noexcept;

    Node* balance(Node* node) noexcept;
    void rebalanceUp(Node* from) noexcept;

    Node* cloneNode(const Node* src, Node* parent) const;
    void swap(AVLTree& other) noexcept;
    Node* fallLeft(Node* node) const noexcept;
    const Node* findNode(const Key& key) const noexcept;
    static void destroyTree(Node* node) noexcept;
  };

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const Compare& cmp):
    root_(nullptr),
    size_(0),
    cmp_(cmp)
  {}

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& other):
    size_(other.size_),
    cmp_(other.cmp_)
  {
    root_ = cloneNode(other.root_, nullptr);
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& other) noexcept:
    root_(std::exchange(other.root_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    cmp_(std::move(other.cmp_))
  {}

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::~AVLTree() noexcept
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >&
    AVLTree< Key, Value, Compare >::operator=(const AVLTree& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }

    AVLTree< Key, Value, Compare > cpy = other;
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >&
    AVLTree< Key, Value, Compare >::operator=(AVLTree&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }

    swap(other);
    return *this;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::cloneNode(const Node* src, Node* parent) const
  {
    if (src == nullptr)
    {
      return nullptr;
    }

    Node* new_node = new Node(src->key_, src->value_);

    try
    {
      new_node->parent_ = parent;
      new_node->height_ = src->height_;
      new_node->left_ = cloneNode(src->left_, new_node);
      new_node->right_ = cloneNode(src->right_, new_node);
    }
    catch (...)
    {
      destroyTree(new_node->left_);
      destroyTree(new_node->right_);
      delete new_node;
      throw;
    }

    return new_node;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::swap(AVLTree& other) noexcept
  {
    std::swap(other.root_, root_);
    std::swap(other.size_, size_);
    std::swap(other.cmp_, cmp_);
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::clear() noexcept
  {
    destroyTree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::destroyTree(Node* node) noexcept
  {
    if (!node)
    {
      return;
    }

    destroyTree(node->left_);
    destroyTree(node->right_);
    delete node;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::fallLeft(Node* node) const noexcept
  {
    while (node && node->left_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  const typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::findNode(const Key& key) const noexcept
  {
    const Node* curr = root_;
    while (curr)
    {
      if (cmp_(key, curr->key_))
      {
        curr = curr->left_;
      }
      else if (cmp_(curr->key_, key))
      {
        curr = curr->right_;
      }
      else
      {
        return curr;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator
  AVLTree< Key, Value, Compare >::find(const Key& key) noexcept
  {
    return iterator(const_cast< Node* >(findNode(key)));
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
  AVLTree< Key, Value, Compare >::find(const Key& key) const noexcept
  {
    return const_iterator(findNode(key));
  }

  template< class Key, class Value, class Compare >
  Value& AVLTree< Key, Value, Compare >::at(const Key& key)
  {
    Node* node = const_cast< Node* >(findNode(key));
    if (!node)
    {
      throw std::out_of_range("AVLTree::at: key not found");
    }

    return node->value_;
  }

  template< class Key, class Value, class Compare >
  const Value& AVLTree< Key, Value, Compare >::at(const Key& key) const
  {
    const Node* node = findNode(key);
    if (!node)
    {
      throw std::out_of_range("AVLTree::at: key not found");
    }

    return node->value_;
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::has(const Key& key) const noexcept
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator
  AVLTree< Key, Value, Compare >::lower_bound(const Key& key) noexcept
  {
    Node* curr = root_;
    Node* candidate = nullptr;

    while (curr)
    {
      if (cmp_(curr->key_, key))
      {
        curr = curr->right_;
      }
      else
      {
        candidate = curr;
        curr = curr->left_;
      }
    }

    return iterator(candidate);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
  AVLTree< Key, Value, Compare >::lower_bound(const Key& key) const noexcept
  {
    return const_cast< AVLTree* >(this)->lower_bound(key);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator
  AVLTree< Key, Value, Compare >::upper_bound(const Key& key) noexcept
  {
    Node* curr = root_;
    Node* candidate = nullptr;

    while (curr)
    {
      if (!cmp_(key, curr->key_))
      {
        curr = curr->right_;
      }
      else
      {
        candidate = curr;
        curr = curr->left_;
      }
    }

    return iterator(candidate);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
  AVLTree< Key, Value, Compare >::upper_bound(const Key& key) const noexcept
  {
    return const_cast< AVLTree* >(this)->upper_bound(key);
  }
}

#endif
