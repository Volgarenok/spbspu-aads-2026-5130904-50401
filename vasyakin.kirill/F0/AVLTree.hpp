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
}

#endif
