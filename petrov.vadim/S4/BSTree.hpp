#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <stdexcept>
#include <algorithm>
#include "TNode.hpp"
#include "BSTIters.hpp"

namespace petrov
{
  template < class Key, class Value, class Compare = std::less< Key > >
  struct BSTree
  {
    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;
    ~BSTree();

    void push(const Key& k, const Value& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    Value drop(Key k);

    using CIterator = BSTCIterator< Key, Value >;
    using Iterator = BSTIterator< Key, Value >;

    CIterator rotateLeft(CIterator it);
    CIterator rotateRight(CIterator it);

    CIterator rotateLargeLeft(CIterator it);
    CIterator rotateLargeRight(CIterator it);

    size_t height(CIterator it);
    size_t height();

    Iterator begin();
    Iterator end();
    CIterator begin() const;
    CIterator end() const;
    CIterator cbegin() const;
    CIterator cend() const;

    bool empty() const;

    using TNode = detail::TNode< Key, Value >;
    private:
      TNode* root_;
      TNode* fake_leaf_;
      Compare comp_;

      TNode* NodeCopy(const TNode* src, TNode* parent, const TNode* src_fake_leaf);
      void swap(BSTree& other) noexcept;
      void clear(TNode* node);
      TNode* findNode(const Key& k);
      const TNode* findNode(const Key& k) const;
      const TNode* minNode(const TNode* root) const;
      const TNode* maxNode(const TNode* root) const;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr),
    fake_leaf_(new TNode()),
    comp_()
  {
    fake_leaf_->left_ = fake_leaf_;
    fake_leaf_->right_ = fake_leaf_;
    fake_leaf_->parent_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
    root_(nullptr),
    comp_(other.comp_)
  {
    fake_leaf_ = new TNode();
    fake_leaf_->left_ = fake_leaf_;
    fake_leaf_->right_ = fake_leaf_;
    fake_leaf_->parent_ = nullptr;

    if (other.root_ != nullptr && other.root_ != other.fake_leaf_)
    {
      root_ = NodeCopy(other.root_, nullptr, other.fake_leaf_);
    }
  }

  template< class Key, class Value, class Compare  >
  BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
    root_(std::move(other.root_)),
    fake_leaf_(std::move(other.fake_leaf_)),
    comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.fake_leaf_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree& other)
  {
    if (this != std::addressof(other))
    {
      BSTree< Key, Value, Compare > cpy = other;
      swap(cpy);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
  {
    if (this != std::addressof(other))
    {
      BSTree< Key, Value, Compare > cpy(std::move(other));
      swap(cpy);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear(root_);
    delete fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(comp_, other.comp_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(TNode* node)
  {
    if (node == nullptr || node == fake_leaf_)
    {
      return;
    }
    clear(node->left_);
    clear(node->right_);
    delete node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::TNode*
  BSTree< Key, Value, Compare >::NodeCopy(const TNode* src, TNode* parent, const TNode* src_fake_leaf)
  {
    if (src == src_fake_leaf || src == nullptr)
    {
      return fake_leaf_;
    }
    TNode* node = new TNode(src->key_, src->value_);
    node->parent_ = parent;
    node->left_ = NodeCopy(src->left_, node, src_fake_leaf);
    node->right_ = NodeCopy(src->right_, node, src_fake_leaf);
    return node;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return root_ == nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    if (root_ == nullptr)
    {
      root_ = new TNode(k, v);
      root_->left_ = fake_leaf_;
      root_->right_ = fake_leaf_;
      root_->parent_ = nullptr;
      return;
    }

    TNode* curr = root_;
    TNode* parent = nullptr;

    while (curr != fake_leaf_)
    {
      if (!comp_(k, curr->key_) && !comp_(curr->key_, k))
      {
        curr->value_ = v;
        return;
      }

      if (comp_(k, curr->key_))
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

    TNode* nnode = new TNode(k, v);
    nnode->left_ = fake_leaf_;
    nnode->right_ = fake_leaf_;
    nnode->parent_ = parent;

    if (comp_(k, parent->key_))
      parent->left_ = nnode;
    else
      parent->right_ = nnode;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::get(const Key& k)
  {
    return const_cast< Value& >(static_cast< const BSTree& >(*this).get(k));
  }

  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::get(const Key& k) const
  {
    return findNode(k)->value_;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::TNode*
  BSTree< Key, Value, Compare >::findNode(const Key& k)
  {
    if (root_ == nullptr)
    {
      throw std::out_of_range("Tree has not this key");
    }

    TNode* curr = root_;
    while (curr != fake_leaf_)
    {
      if (!comp_(k, curr->key_) && !comp_(curr->key_, k))
      {
        return curr;
      }

      if (comp_(k, curr->key_))
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
  const typename BSTree< Key, Value, Compare >::TNode*
  BSTree< Key, Value, Compare >::findNode(const Key& k) const
  {
    if (root_ == nullptr)
    {
      throw std::out_of_range("Tree has not this key");
    }

    const TNode* curr = root_;
    while (curr != fake_leaf_)
    {
      if (!comp_(k, curr->key_) && !comp_(curr->key_, k))
      {
        return curr;
      }

      if (comp_(k, curr->key_))
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
  const typename BSTree< Key, Value, Compare >::TNode*
  BSTree< Key, Value, Compare >::minNode(const TNode* root) const
  {
    const TNode* curr = root;

    while (curr->left_ != fake_leaf_)
    {
      curr = curr->left_;
    }
    return curr;
  }

  template< class Key, class Value, class Compare >
  const typename BSTree< Key, Value, Compare >::TNode*
  BSTree< Key, Value, Compare >::maxNode(const TNode* root) const
  {
    const TNode* curr = root;

    while (curr->right_ != fake_leaf_)
    {
      curr = curr->right_;
    }
    return curr;
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(Key k)
  {
    TNode* curr = findNode(k);
    Value res = curr->value_;

    if (curr->left_ == fake_leaf_ && curr->right_ == fake_leaf_)
    {
      if (!curr->parent_)
      {
        root_ = nullptr;
      }
      else if (curr->parent_->left_ == curr)
      {
        curr->parent_->left_ = fake_leaf_;
      }
      else if (curr->parent_->right_ == curr)
      {
        curr->parent_->right_ = fake_leaf_;
      }
      delete curr;
      return res;
    }
    else if (curr->left_ == fake_leaf_)
    {
      curr->right_->parent_ = curr->parent_;

      if (!curr->parent_)
      {
        root_ = curr->right_;
      }
      else if (curr->parent_->left_ == curr)
      {
        curr->parent_->left_ = curr->right_;
      }
      else if (curr->parent_->right_ == curr)
      {
        curr->parent_->right_ = curr->right_;
      }
      delete curr;
      return res;
    }
    else if (curr->right_ == fake_leaf_)
    {
      curr->left_->parent_ = curr->parent_;

      if (!curr->parent_)
      {
        root_ = curr->left_;
      }
      else if (curr->parent_->left_ == curr)
      {
        curr->parent_->left_ = curr->left_;
      }
      else if (curr->parent_->right_ == curr)
      {
        curr->parent_->right_ = curr->left_;
      }
      delete curr;
      return res;
    }
    else
    {
      TNode* min_in_right = const_cast< TNode* >(minNode(curr->right_));

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
      return res;
    }
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Iterator
  BSTree< Key, Value, Compare >::begin()
  {
    if (empty())
    {
      return end();
    }
    return Iterator(const_cast< TNode* >(minNode(root_)), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Iterator
  BSTree< Key, Value, Compare >::end()
  {
    return Iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::begin() const
  {
    if (empty())
    {
      return end();
    }
    return CIterator(minNode(root_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::end() const
  {
    return CIterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::cbegin() const
  {
    if (empty())
    {
      return end();
    }
    return CIterator(minNode(root_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::cend() const
  {
    return CIterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(CIterator it)
  {
    if (it.node_ == fake_leaf_)
    {
      return 0;
    }
    size_t left_h = height(CIterator(it.node_->left_, fake_leaf_));
    size_t right_h = height(CIterator(it.node_->right_, fake_leaf_));
    return 1 + std::max(left_h, right_h);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height()
  {
    if (empty())
    {
      return 0;
    }
    return height(CIterator(root_, fake_leaf_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::rotateLeft(CIterator it)
  {
    TNode* n = const_cast< TNode* >(it.node_);

    if (n == fake_leaf_ || n->parent_ == nullptr || n->parent_->left_ == n)
    {
      throw std::invalid_argument("rotateLeft: invalid node");
    }

    TNode* p = n->parent_;

    p->right_ = n->left_;
    if (n->left_ != fake_leaf_)
    {
      n->left_->parent_ = p;
    }

    n->parent_ = p->parent_;
    if (p->parent_ == nullptr)
    {
      root_ = n;
    }
    else if (p->parent_->left_ == p)
    {
      p->parent_->left_ = n;
    }
    else
    {
      p->parent_->right_ = n;
    }

    n->left_ = p;
    p->parent_ = n;

    return CIterator(n, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::rotateRight(CIterator it)
  {
    TNode* n = const_cast< TNode* >(it.node_);

    if (n == fake_leaf_ || n->parent_ == nullptr || n->parent_->right_ == n)
    {
      throw std::invalid_argument("rotateRight: invalid node or no parent");
    }

    TNode* p = n->parent_;

    p->left_ = n->right_;
    if (n->right_ != fake_leaf_)
    {
      n->right_->parent_ = p;
    }

    n->parent_ = p->parent_;
    if (p->parent_ == nullptr)
    {
      root_ = n;
    }
    else if (p->parent_->right_ == p)
    {
      p->parent_->right_ = n;
    }
    else
    {
      p->parent_->left_ = n;
    }

    n->right_ = p;
    p->parent_ = n;

    return CIterator(n, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::rotateLargeLeft(CIterator it)
  {
    TNode* node = const_cast< TNode* >(it.node_);
    if (node == fake_leaf_ || node->parent_ == nullptr || node->parent_->parent_ == nullptr)
    {
      throw std::invalid_argument("No parent");
    }

    rotateRight(it);
    return rotateLeft(it);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::CIterator
  BSTree< Key, Value, Compare >::rotateLargeRight(CIterator it)
  {
    TNode* node = const_cast< TNode* >(it.node_);
    if (node == fake_leaf_ || node->parent_ == nullptr || node->parent_->parent_ == nullptr)
    {
      throw std::invalid_argument("No parent");
    }

    rotateLeft(it);
    return rotateRight(it);
  }
}

#endif
