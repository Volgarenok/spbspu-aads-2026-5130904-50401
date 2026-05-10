#ifndef BSTTREE_HPP
#define BSTTREE_HPP
#include "treenode.hpp"
#include "bstiterators.hpp"
#include <functional>
#include <stdexcept>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <memory>

namespace vasyakin
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:

    BSTree(const Compare& cmp = Compare());
    ~BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    Value drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const;

  private:
    using Node = ::vasyakin::Node< Key, Value >;

    Node* root_;
    Node* fake_leaf_;
    Compare cmp_;

    void clear(Node* node);
    void swap(BSTree& other) noexcept;
    Node* cloneNode(const Node* src, Node* parent, const Node* src_fake_leaf);
    Node* findNode(const Key& k);
    const Node* findNode(const Key& k) const;
    size_t calcHeight(const Node* node) const;
    Node* fallLeft(Node* node) const;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const Compare& cmp):
    cmp_(cmp)
  {
    fake_leaf_ = new Node(Key{}, Value{});
    fake_leaf_->left_ = fake_leaf_;
    fake_leaf_->right_ = fake_leaf_;
    fake_leaf_->parent_ = nullptr;
    root_ = fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear(root_);
    delete fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
    cmp_(other.cmp_)
  {
    fake_leaf_ = new Node(Key{}, Value{});
    fake_leaf_->left_ = fake_leaf_;
    fake_leaf_->right_ = fake_leaf_;
    fake_leaf_->parent_ = nullptr;

    root_ = cloneNode(other.root_, nullptr, other.fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
    root_(std::move(other.root_)),
    fake_leaf_(std::move(other.fake_leaf_)),
    cmp_(std::move(other.cmp_))
  {
    other.root_ = nullptr;
    other.fake_leaf_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree& other)
  {
    if (this == std::addressof(other))
    {
      return *this;
    }

    BSTree< Key, Value, Compare > cpy = other;
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
  {
    if (this == std::addressof(other))
    {
      return *this;
    }

    BSTree< Key, Value, Compare > cpy(std::move(other));
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(Node* node)
  {
    if (node == fake_leaf_)
    {
      return;
    }

    clear(node->left_);
    clear(node->right_);
    delete node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node*
  BSTree< Key, Value, Compare >::cloneNode(const Node* src, Node* parent, const Node* src_fake_leaf)
  {
    if (src == src_fake_leaf)
    {
      return fake_leaf_;
    }

    Node* new_node = new Node(src->key_, src->value_);
    new_node->parent_ = parent;
    new_node->left_ = cloneNode(src->left_, new_node, src_fake_leaf);
    new_node->right_ = cloneNode(src->right_, new_node, src_fake_leaf);

    return new_node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
  {
    std::swap(cmp_, other.cmp_);
    std::swap(root_, other.root_);
    std::swap(fake_leaf_, other.fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node*
  BSTree< Key, Value, Compare >::findNode(const Key& k)
  {
    Node* curr = root_;

    while (curr != fake_leaf_)
    {
      if (!cmp_(k, curr->key_) && !cmp_(curr->key_, k))
      {
        return curr;
      }

      if (cmp_(k, curr->key_))
      {
        curr = curr->left_;
      }
      else
      {
        curr = curr->right_;
      }
    }

    throw std::out_of_range("Tree has not this key");
  }

  template< class Key, class Value, class Compare >
  const typename BSTree< Key, Value, Compare >::Node*
  BSTree< Key, Value, Compare >::findNode(const Key& k) const
  {
    Node* curr = root_;

    while (curr != fake_leaf_)
    {
      if (!cmp_(k, curr->key_) && !cmp_(curr->key_, k))
      {
        return curr;
      }

      if (cmp_(k, curr->key_))
      {
        curr = curr->left_;
      }
      else
      {
        curr = curr->right_;
      }
    }

    throw std::out_of_range("Tree has not this key");
  }
}

#endif
