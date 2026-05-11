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

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    Node* parent = nullptr;
    Node* curr = root_;

    while (curr != fake_leaf_)
    {
      if (!cmp_(curr->key_, k) && !cmp_(k, curr->key_))
      {
        curr->value_ = v;
        return;
      }

      if (cmp_(k, curr->key_))
      {
        parent = curr;
        curr = curr->left_;
      }
      else
      {
        parent = curr;
        curr = curr->right_;
      }
    }

    Node* new_node = new Node(k, v);
    new_node->parent_ = parent;
    new_node->left_ = fake_leaf_;
    new_node->right_ = fake_leaf_;

    if (parent != nullptr)
    {
      if (cmp_(k, parent->key_))
      {
        parent->left_ = new_node;
      }
      else
      {
        parent->right_ = new_node;
      }
    }
    else
    {
      root_ = new_node;
    }
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::get(const Key& k)
  {
    return const_cast< Node* >(findNode(k))->value_;
  }

  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::get(const Key& k) const
  {
    return const_cast< Node* >(findNode(k))->value_;
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(Key k)
  {
    Node* curr = root_;

    while (curr != fake_leaf_)
    {
      if (!cmp_(k, curr->key_) && !cmp_(curr->key_, k))
      {

        Value val = curr->value_;

        if (curr->left_ == fake_leaf_ && curr->right_ == fake_leaf_)
        {
          if (curr->parent_)
          {
            if (curr->parent_->left_ == curr)
            {
              curr->parent_->left_ = fake_leaf_;
            }
            else
            {
              curr->parent_->right_ = fake_leaf_;
            }
          }
          else
          {
            root_ = fake_leaf_;
          }
        }
        else if (curr->left_ == fake_leaf_)
        {
          curr->right_->parent_ = curr->parent_;

          if (curr->parent_)
          {
            if (curr->parent_->left_ == curr)
            {
              curr->parent_->left_ = curr->right_;
            }
            else
            {
              curr->parent_->right_ = curr->right_;
            }
          }
          else
          {
            root_ = curr->right_;
          }
        }
        else if (curr->right_ == fake_leaf_)
        {
          curr->left_->parent_ = curr->parent_;

          if (curr->parent_)
          {
            if (curr->parent_->left_ == curr)
            {
              curr->parent_->left_ = curr->left_;
            }
            else
            {
              curr->parent_->right_ = curr->left_;
            }
          }
          else
          {
            root_ = curr->left_;
          }
        }
        else
        {
          Node* min_in_right = curr->right_;

          while (min_in_right->left_ != fake_leaf_)
          {
            min_in_right = min_in_right->left_;
          }

          curr->key_ = min_in_right->key_;
          curr->value_ = min_in_right->value_;

          if (min_in_right->right_ != fake_leaf_)
          {
            min_in_right->right_->parent_ = min_in_right->parent_;
          }

          if (min_in_right->parent_->left_ == min_in_right)
          {
            min_in_right->parent_->left_ = min_in_right->right_;
          }
          else
          {
            min_in_right->parent_->right_ = min_in_right->right_;
          }

          delete min_in_right;
          return val;
        }

        delete curr;
        return val;
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
  size_t BSTree< Key, Value, Compare >::calcHeight(const Node* node) const
  {
    if (node == fake_leaf_)
    {
      return 0;
    }

    size_t left_h = calcHeight(node->left_);
    size_t right_h = calcHeight(node->right_);
    size_t height = std::max(left_h, right_h) + 1;

    return height;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it)
  {
    return calcHeight(it.node_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height()
  {
    return calcHeight(root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node*
  BSTree< Key, Value, Compare >::fallLeft(Node* node) const
  {
    while (node != fake_leaf_ && node->left_ != fake_leaf_)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator
  BSTree< Key, Value, Compare >::begin()
  {
    return iterator(fallLeft(root_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator
  BSTree< Key, Value, Compare >::end()
  {
    return iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::begin() const
  {
    return const_iterator(fallLeft(root_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::cbegin() const
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::cend() const
  {
    return end();
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return root_ == fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    Node* x = const_cast< Node* >(it.node_);

    if (x == fake_leaf_ || x->parent_ == nullptr)
    {
      throw std::invalid_argument("rotateLeft: invalid node or no parent");
    }

    Node* y = x->parent_;

    y->right_ = x->left_;
    if (x->left_ != fake_leaf_)
    {
      x->left_->parent_ = y;
    }

    x->parent_ = y->parent_;
    if (y->parent_ == nullptr)
    {
      root_ = x;
    }
    else if (y->parent_->left_ == y)
    {
      y->parent_->left_ = x;
    }
    else
    {
      y->parent_->right_ = x;
    }

    x->left_ = y;
    y->parent_ = x;

    return const_iterator(x, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    Node* x = const_cast< Node* >(it.node_);

    if (x == fake_leaf_ || x->parent_ == nullptr)
    {
      throw std::invalid_argument("rotateRight: invalid node or no parent");
    }

    Node* y = x->parent_;

    y->left_ = x->right_;
    if (x->right_ != fake_leaf_)
    {
      x->right_->parent_ = y;
    }

    x->parent_ = y->parent_;
    if (y->parent_ == nullptr)
    {
      root_ = x;
    }
    else if (y->parent_->right_ == y)
    {
      y->parent_->right_ = x;
    }
    else
    {
      y->parent_->left_ = x;
    }

    x->right_ = y;
    y->parent_ = x;

    return const_iterator(x, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    Node* node = const_cast< Node* >(it.node_);
    if (node == nullptr || node->parent_ == nullptr || node->parent_->parent_ == nullptr)
    {
      throw std::invalid_argument("Can not do rotate. No parent");
    }

    rotateRight(it);
    return rotateLeft(const_iterator(node, fake_leaf_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    Node* node = const_cast< Node* >(it.node_);
    if (node == nullptr || node->parent_ == nullptr || node->parent_->parent_ == nullptr)
    {
      throw std::invalid_argument("Can not do rotate. No parent");
    }

    rotateLeft(it);
    return rotateRight(const_iterator(node, fake_leaf_));
  }
}

#endif
