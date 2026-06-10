#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <iostream>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include "avliterators.hpp"
#include "avltreenode.hpp"
#include "list.hpp"

namespace sedov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
      using valueType = std::pair< const Key, Value >;
      using iter = AVLiter< Key, Value >;
      using constIter = AVLConstiter< Key, Value >;

      AVLTree() noexcept;
      AVLTree(const AVLTree & h);
      AVLTree(AVLTree && h) noexcept;
      ~AVLTree();

      AVLTree& operator=(const AVLTree & h);
      AVLTree& operator=(AVLTree && h) noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;
      void swap(AVLTree & h) noexcept;
      void clear() noexcept;

      bool find(const Key & k, Value & outValue) const;
      const Value & at(const Key & k) const;
      Value & at(const Key & k);
      bool contains(const Key & k) const;

      bool insert(const Key & k, const Value & v);
      bool insert(Key && k, Value && v);
      bool erase(const Key & k);
      Value drop(const Key & k);

      void push(const Key & k, const Value & v);
      void push(Key && k, Value && v);

      iter begin() noexcept;
      iter end() noexcept;
      constIter begin() const noexcept;
      constIter end() const noexcept;
      constIter cbegin() const noexcept;
      constIter cend() const noexcept;

      size_t height() const noexcept;
      void collectInRange(const Key & from, const Key & to, List< valueType > & result) const;
      void getAll(List< valueType > & result) const;

  private:
      AVLTreeNode< Key, Value > * root_;
      size_t size_;
      Compare comp_;

      AVLTreeNode< Key, Value > * clone(AVLTreeNode< Key, Value > * src, AVLTreeNode< Key, Value > * parent);
      void clearImpl(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * fallLeft(AVLTreeNode< Key, Value > * node) const noexcept;
      AVLTreeNode< Key, Value > * fallRight(AVLTreeNode< Key, Value > * node) const noexcept;

      void updateHeight(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * balanceNode(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * rotateLeft(AVLTreeNode< Key, Value > * x) noexcept;
      AVLTreeNode< Key, Value > * rotateRight(AVLTreeNode< Key, Value > * y) noexcept;
      AVLTreeNode< Key, Value > * rotateLargeLeft(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * rotateLargeRight(AVLTreeNode< Key, Value > * node) noexcept;

      std::pair< AVLTreeNode< Key, Value > *, bool > insertNode(AVLTreeNode< Key, Value > * node, const Key & k,
        const Value & v);
      std::pair< AVLTreeNode< Key, Value > *, bool > insertNode(AVLTreeNode< Key, Value > * node, Key && k,
        Value && v);
      AVLTreeNode< Key, Value > * removeNode(AVLTreeNode< Key, Value > * node, const Key & k, bool & found) noexcept;
      const AVLTreeNode< Key, Value > * findNode(const Key & k) const noexcept;

      void collectInRangeImpl(AVLTreeNode< Key, Value > * node, const Key & from, const Key & to,
        List< valueType > & result) const;
      void getAllImpl(AVLTreeNode< Key, Value > * node, List< valueType > & result) const;
  };

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree() noexcept:
    root_(nullptr),
    size_(0),
    comp_()
  {}

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree & h):
    root_(nullptr),
    size_(0),
    comp_(h.comp_)
  {
    root_ = clone(h.root_, nullptr);
    size_ = h.size_;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree && h) noexcept:
    root_(h.root_),
    size_(h.size_),
    comp_(std::move(h.comp_))
  {
    h.root_ = nullptr;
    h.size_ = 0;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    clear();
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare > & AVLTree< Key, Value, Compare >::operator=(const AVLTree & h)
  {
    if (this != std::addressof(h))
    {
      AVLTree temp(h);
      swap(temp);
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare > & AVLTree< Key, Value, Compare >::operator=(AVLTree && h) noexcept
  {
    if (this != std::addressof(h))
    {
      clear();
      root_ = h.root_;
      size_ = h.size_;
      comp_ = std::move(h.comp_);
      h.root_ = nullptr;
      h.size_ = 0;
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template < class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::swap(AVLTree & h) noexcept
  {
    std::swap(root_, h.root_);
    std::swap(size_, h.size_);
    std::swap(comp_, h.comp_);
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::clear() noexcept
  {
    clearImpl(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::find(const Key & k, Value & outValue) const
  {
    AVLTreeNode< Key, Value > * node = findNode(k);
    if (node)
    {
      outValue = node->value_;
      return true;
    }
    return false;
  }

  template < class Key, class Value, class Compare >
  const Value & AVLTree< Key, Value, Compare >::at(const Key & k) const
  {
    AVLTreeNode< Key, Value > * node = findNode(k);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->value_;
  }

  template < class Key, class Value, class Compare >
  Value & AVLTree< Key, Value, Compare >::at(const Key & k)
  {
    AVLTreeNode< Key, Value > * node = findNode(k);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->value_;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::contains(const Key & k) const
  {
    return findNode(k) != nullptr;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::insert(const Key & k, const Value & v)
  {
    auto res = insertNode(root_, k, v);
    root_ = res.first;
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    return res.second;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::insert(Key && k, Value && v)
  {
    auto res = insertNode(root_, std::move(k), std::move(v));
    root_ = res.first;
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    return res.second;
  }

  template < class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::erase(const Key & k)
  {
    if (!findNode(k))
    {
      return false;
    }
    bool found = false;
    root_ = removeNode(root_, k, found);
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    if (found)
    {
      --size_;
    }
    return found;
  }

  template < class Key, class Value, class Compare >
  Value AVLTree< Key, Value, Compare >::drop(const Key & k)
  {
    if (!findNode(k))
    {
      throw std::out_of_range("Key not found");
    }
    Value result;
    find(k, result);
    bool found = false;
    root_ = removeNode(root_, k, found);
    if (root_)
    {
      root_->parent_ = nullptr;
    }
    return result;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(const Key & k, const Value & v)
  {
    insert(k, v);
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(Key && k, Value && v)
  {
    insert(std::move(k), std::move(v));
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::iter AVLTree< Key, Value, Compare >::begin() noexcept
  {
    if (!root_)
    {
      return iter(nullptr);
    }
    return iter(fallLeft(root_));
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::iter AVLTree< Key, Value, Compare >::end() noexcept
  {
    return iter(nullptr);
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::constIter AVLTree< Key, Value, Compare >::begin() const noexcept
  {
    if (!root_)
    {
      return constIter(nullptr);
    }
    return constIter(fallLeft(root_));
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::constIter AVLTree< Key, Value, Compare >::end() const noexcept
  {
    return constIter(nullptr);
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::constIter AVLTree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (!root_)
    {
      return constIter(nullptr);
    }
    return constIter(fallLeft(root_));
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::constIter AVLTree< Key, Value, Compare >::cend() const noexcept
  {
    return constIter(nullptr);
  }

  template < class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::height() const noexcept
  {
    return root_ ? root_->height_ : 0;
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::collectInRange(const Key & from, const Key & to,
    List< std::pair< const Key, Value > > & result) const
  {
    collectInRangeImpl(root_, from, to, result);
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::getAll(List< std::pair< const Key, Value > > & result) const
  {
    getAllImpl(root_, result);
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::clone(AVLTreeNode< Key, Value > * src,
    AVLTreeNode< Key, Value > * parent)
  {
    if (!src)
    {
      return nullptr;
    }
    AVLTreeNode< Key, Value > * newNode = new AVLTreeNode< Key, Value >(src->key_, src->value_, parent);
    newNode->left_ = clone(src->left_, newNode);
    newNode->right_ = clone(src->right_, newNode);
    newNode->height_ = src->height_;
    return newNode;
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::clearImpl(AVLTreeNode< Key, Value > * node) noexcept
  {
    if (!node)
    {
      return;
    }
    clearImpl(node->left_);
    clearImpl(node->right_);
    delete node;
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::fallLeft(AVLTreeNode< Key, Value > * node) const noexcept
  {
    while (node && node->left_)
    {
      node = node->left_;
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > *
    AVLTree< Key, Value, Compare >::fallRight(AVLTreeNode< Key, Value > * node) const noexcept
  {
    while (node && node->right_)
    {
      node = node->right_;
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::updateHeight(AVLTreeNode< Key, Value > * node) noexcept
  {
    if (node)
    {
      node->updateHeight();
    }
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::balanceNode(AVLTreeNode< Key, Value > * node) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    updateHeight(node);
    int bf = node->balanceFactor();
    if (bf > 1)
    {
      if (node->left_ && node->left_->balanceFactor() < 0)
      {
        return rotateLargeLeft(node);
      }
      return rotateRight(node);
    }
    if (bf < -1)
    {
      if (node->right_ && node->right_->balanceFactor() > 0)
      {
        return rotateLargeRight(node);
      }
      return rotateLeft(node);
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::rotateLeft(AVLTreeNode< Key, Value > * x) noexcept
  {
    AVLTreeNode< Key, Value > * y = x->right_;
    if (!y)
    {
      return x;
    }
    x->right_ = y->left_;
    if (y->left_)
    {
      y->left_->parent_ = x;
    }
    y->parent_ = x->parent_;
    if (!x->parent_)
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
    updateHeight(x);
    updateHeight(y);
    return y;
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::rotateRight(AVLTreeNode< Key, Value > * y) noexcept
  {
    AVLTreeNode< Key, Value > * x = y->left_;
    if (!x)
    {
      return y;
    }
    y->left_ = x->right_;
    if (x->right_)
    {
      x->right_->parent_ = y;
    }
    x->parent_ = y->parent_;
    if (!y->parent_)
    {
      root_ = x;
    }
    else if (y == y->parent_->left_)
    {
      y->parent_->left_ = x;
    }
    else
    {
      y->parent_->right_ = x;
    }
    x->right_ = y;
    y->parent_ = x;
    updateHeight(y);
    updateHeight(x);
    return x;
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > *
    AVLTree< Key, Value, Compare >::rotateLargeLeft(AVLTreeNode< Key, Value > * node) noexcept
  {
    node->left_ = rotateLeft(node->left_);
    if (node->left_)
    {
      node->left_->parent_ = node;
    }
    return rotateRight(node);
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > *
    AVLTree< Key, Value, Compare >::rotateLargeRight(AVLTreeNode< Key, Value > * node) noexcept
  {
    node->right_ = rotateRight(node->right_);
    if (node->right_)
    {
      node->right_->parent_ = node;
    }
    return rotateLeft(node);
  }

  template < class Key, class Value, class Compare >
  std::pair< AVLTreeNode< Key, Value > *, bool > 
    AVLTree< Key, Value, Compare >::insertNode(AVLTreeNode< Key, Value > * node, const Key & k, const Value & v)
  {
    if (!node)
    {
      ++size_;
      return std::make_pair(new AVLTreeNode< Key, Value >(k, v, nullptr), true);
    }
    if (comp_(k, node->key_))
    {
      auto res = insertNode(node->left_, k, v);
      node->left_ = res.first;
      if (node->left_)
      {
        node->left_->parent_ = node;
      }
    }
    else if (comp_(node->key_, k))
    {
      auto res = insertNode(node->right_, k, v);
      node->right_ = res.first;
      if (node->right_)
      {
        node->right_->parent_ = node;
      }
    }
    else
    {
      node->value_ = v;
      return std::make_pair(node, false);
    }
    return std::make_pair(balanceNode(node), true);
  }

  template < class Key, class Value, class Compare >
  std::pair< AVLTreeNode< Key, Value > *, bool>
    AVLTree< Key, Value, Compare >::insertNode(AVLTreeNode< Key, Value > * node, Key && k, Value && v)
  {
    if (!node)
    {
      ++size_;
      return std::make_pair(new AVLTreeNode< Key, Value >(std::move(k), std::move(v), nullptr), true);
    }
    if (comp_(k, node->key_))
    {
      auto res = insertNode(node->left_, std::move(k), std::move(v));
      node->left_ = res.first;
      if (node->left_)
      {
        node->left_->parent_ = node;
      }
    }
    else if (comp_(node->key_, k))
    {
      auto res = insertNode(node->right_, std::move(k), std::move(v));
      node->right_ = res.first;
      if (node->right_)
      {
        node->right_->parent_ = node;
      }
    }
    else
    {
      node->value_ = std::move(v);
      return std::make_pair(node, false);
    }
    return std::make_pair(balanceNode(node), true);
  }

  template < class Key, class Value, class Compare >
  AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::removeNode(AVLTreeNode< Key, Value > * node,
    const Key & k, bool & found) noexcept
  {
    if (!node)
    {
      return nullptr;
    }
    if (comp_(k, node->key_))
    {
      node->left_ = removeNode(node->left_, k, found);
      if (node->left_)
      {
        node->left_->parent_ = node;
      }
    }
    else if (comp_(node->key_, k))
    {
      node->right_ = removeNode(node->right_, k, found);
      if (node->right_)
      {
        node->right_->parent_ = node;
      }
    }
    else
    {
      found = true;
      if (!node->left_ || !node->right_)
      {
        AVLTreeNode< Key, Value > * child = node->left_ ? node->left_ : node->right_;
        if (child)
        {
          child->parent_ = node->parent_;
        }
        delete node;
        return child;
      }
      else
      {
        AVLTreeNode< Key, Value > * succ = fallLeft(node->right_);
        node->key_ = succ->key_;
        node->value_ = std::move(succ->value_);
        node->right_ = removeNode(node->right_, succ->key_, found);
        if (node->right_)
        {
          node->right_->parent_ = node;
        }
      }
    }
    return balanceNode(node);
  }

  template < class Key, class Value, class Compare >
  const AVLTreeNode< Key, Value > * AVLTree< Key, Value, Compare >::findNode(const Key & k) const noexcept
  {
    AVLTreeNode< Key, Value > * cur = root_;
    while (cur)
    {
      if (comp_(k, cur->key_))
      {
        cur = cur->left_;
      }
      else if (comp_(cur->key_, k))
      {
        cur = cur->right_;
      }
      else
      {
        return cur;
      }
    }
    return nullptr;
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::collectInRangeImpl(AVLTreeNode< Key, Value > * node, const Key & from,
    const Key & to, List< std::pair< const Key, Value > > & result) const
  {
    if (!node)
    {
      return;
    }
    if (!comp_(node->key_, from))
    {
      collectInRangeImpl(node->left_, from, to, result);
    }
    if (!comp_(node->key_, from) && !comp_(to, node->key_))
    {
      result.pushBack(std::pair< const Key, Value >(node->key_, node->value_));
    }
    if (!comp_(to, node->key_))
    {
      collectInRangeImpl(node->right_, from, to, result);
    }
  }

  template < class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::getAllImpl(AVLTreeNode< Key, Value > * node,
    List< std::pair< const Key, Value > > & result) const
  {
    if (!node)
    {
      return;
    }
    getAllImpl(node->left_, result);
    result.pushBack(std::pair< const Key, Value >(node->key_, node->value_));
    getAllImpl(node->right_, result);
  }
}

#endif
