#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <stdexcept>
#include <cstddef>
#include <functional>
#include <algorithm>
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

    std::pair< iterator, bool > insert(const Key& key, const Value& value);
    std::pair< iterator, bool > insert(Key&& key, Value&& value);

    size_t erase(const Key& key);
    void clear() noexcept;

    iterator find(const Key& key) noexcept;
    const_iterator find(const Key& key) const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    size_t count(const Key& key) const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    void swap(AVLTree& other) noexcept;

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

    template< class K, class V >
    std::pair< iterator, bool > insertImpl(K&& key, V&& value);

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

    Node* new_node = new Node(src->value_.first, src->value_.second);

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
      if (cmp_(key, curr->value_.first))
      {
        curr = curr->left_;
      }
      else if (cmp_(curr->value_.first, key))
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
    return const_cast< Value& >(static_cast< const AVLTree* >(this)->at(key));
  }

  template< class Key, class Value, class Compare >
  const Value& AVLTree< Key, Value, Compare >::at(const Key& key) const
  {
    const Node* node = findNode(key);
    if (!node)
    {
      throw std::out_of_range("AVLTree::at: key not found");
    }

    return node->value_.second;
  }

  template< class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::count(const Key& key) const noexcept
  {
    return findNode(key) != nullptr ? 1 : 0;
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
      if (cmp_(curr->value_.first, key))
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
      if (!cmp_(key, curr->value_.first))
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

  template< class Key, class Value, class Compare >
  std::pair< typename AVLTree< Key, Value, Compare >::iterator,
    typename AVLTree< Key, Value, Compare >::iterator >
    AVLTree< Key, Value, Compare >::equal_range(const Key& key) noexcept
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLTree< Key, Value, Compare >::const_iterator,
    typename AVLTree< Key, Value, Compare >::const_iterator >
    AVLTree< Key, Value, Compare >::equal_range(const Key& key) const noexcept
  {
    return {lower_bound(key), upper_bound(key)};
  }

  template< class Key, class Value, class Compare >
  int AVLTree< Key, Value, Compare >::getHeight(const Node* node) const noexcept
  {
    return node ? node->height_ : 0;
  }

  template< class Key, class Value, class Compare >
  int AVLTree< Key, Value, Compare >::getBalance(const Node* node) const noexcept
  {
    return node ? getHeight(node->left_) - getHeight(node->right_) : 0;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::updateHeight(Node* node) noexcept
  {
    if (node)
    {
      node->height_ = 1 +
        std::max(getHeight(node->left_), getHeight(node->right_));
    }
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
    AVLTree< Key, Value, Compare >::rotateLeft(Node* node) noexcept
  {
    if (!node)
    {
      return node;
    }

    Node* new_root = node->right_;
    node->right_ = new_root->left_;

    if (new_root->left_)
    {
      new_root->left_->parent_ = node;
    }

    new_root->parent_ = node->parent_;
    if (!node->parent_)
    {
      root_ = new_root;
    }
    else if (node == node->parent_->left_)
    {
      node->parent_->left_ = new_root;
    }
    else
    {
      node->parent_->right_ = new_root;
    }

    new_root->left_ = node;
    node->parent_ = new_root;

    updateHeight(node);
    updateHeight(new_root);

    return new_root;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
    AVLTree< Key, Value, Compare >::rotateLargeLeft(Node* node) noexcept
  {
    if (!node)
    {
      return node;
    }

    node->right_ = rotateRight(node->right_);
    return rotateLeft(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
    AVLTree< Key, Value, Compare >::rotateRight(Node* node) noexcept
  {
    if (!node)
    {
      return node;
    }

    Node* new_root = node->left_;
    node->left_ = new_root->right_;

    if (new_root->right_)
    {
      new_root->right_->parent_ = node;
    }

    new_root->parent_ = node->parent_;
    if (!node->parent_)
    {
      root_ = new_root;
    }
    else if (node == node->parent_->right_)
    {
      node->parent_->right_ = new_root;
    }
    else
    {
      node->parent_->left_ = new_root;
    }

    new_root->right_ = node;
    node->parent_ = new_root;

    updateHeight(node);
    updateHeight(new_root);

    return new_root;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
    AVLTree< Key, Value, Compare >::rotateLargeRight(Node* node) noexcept
  {
    if (!node)
    {
      return node;
    }

    node->left_ = rotateLeft(node->left_);
    return rotateRight(node);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
    AVLTree< Key, Value, Compare >::balance(Node* node) noexcept
  {
    if (!node)
    {
      return node;
    }

    updateHeight(node);
    int bal = getBalance(node);

    if (bal > 1)
    {
      if (getBalance(node->left_) < 0)
      {
        return rotateLargeRight(node);
      }

      return rotateRight(node);
    }

    if (bal < -1)
    {
      if (getBalance(node->right_) > 0)
      {
        return rotateLargeLeft(node);
      }

      return rotateLeft(node);
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::rebalanceUp(Node* from) noexcept
  {
    Node* node = from;
    while (node)
    {
      Node* parent = node->parent_;
      bool was_left = parent && parent->left_ == node;

      int old_height = node->height_;

      node = balance(node);
      if (old_height == node->height_)
      {
        break;
      }

      if (parent)
      {
        if (was_left)
        {
          parent->left_ = node;
        }
        else
        {
          parent->right_ = node;
        }
      }
      else
      {
        root_ = node;
        break;
      }

      node = parent;
    }

    if (root_)
    {
      root_->parent_ = nullptr;
    }
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLTree< Key, Value, Compare >::iterator, bool >
    AVLTree< Key, Value, Compare >::insert(const Key& key, const Value& value)
  {
    return insertImpl(key, value);
  }

  template< class Key, class Value, class Compare >
  std::pair< typename AVLTree< Key, Value, Compare >::iterator, bool >
    AVLTree< Key, Value, Compare >::insert(Key&& key, Value&& value)
  {
    return insertImpl(std::forward< Key >(key), std::forward< Value >(value));
  }

  template< class Key, class Value, class Compare >
  template< class K, class V >
  std::pair< typename AVLTree< Key, Value, Compare >::iterator, bool >
    AVLTree< Key, Value, Compare >::insertImpl(K&& key, V&& value)
  {
    Node* curr = root_;
    Node* parent = nullptr;
    bool go_left = false;

    while (curr)
    {
      parent = curr;

      if (cmp_(key, curr->value_.first))
      {
        curr = curr->left_;
        go_left = true;
      }
      else if (cmp_(curr->value_.first, key))
      {
        curr = curr->right_;
        go_left = false;
      }
      else
      {
        return {iterator(curr), false};
      }
    }

    Node* new_node = new Node(std::forward< K >(key), std::forward< V >(value));
    ++size_;
    new_node->parent_ = parent;

    if (parent != nullptr)
    {
      if (go_left)
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

    rebalanceUp(new_node);

    return {iterator(new_node), true};
  }

  template< class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::erase(const Key& key)
  {
    Node* curr = root_;
    Node* rebalance_from = nullptr;
    bool found = false;

    while (curr != nullptr)
    {
      if (!cmp_(key, curr->value_.first) && !cmp_(curr->value_.first, key))
      {
        found = true;

        if (curr->left_ == nullptr && curr->right_ == nullptr)
        {
          rebalance_from = curr->parent_;

          if (curr->parent_)
          {
            if (curr->parent_->left_ == curr)
            {
              curr->parent_->left_ = nullptr;
            }
            else
            {
              curr->parent_->right_ = nullptr;
            }
          }
          else
          {
            root_ = nullptr;
          }
        }
        else if (curr->left_ == nullptr)
        {
          rebalance_from = curr->parent_;

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
        else if (curr->right_ == nullptr)
        {
          rebalance_from = curr->parent_;

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

          while (min_in_right->left_ != nullptr)
          {
            min_in_right = min_in_right->left_;
          }

          const_cast< Key& >(curr->value_.first) = std::move(
            const_cast< Key& >(min_in_right->value_.first));
          curr->value_.second = std::move(min_in_right->value_.second);

          if (min_in_right->right_ != nullptr)
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

          rebalance_from = min_in_right->parent_;

          delete min_in_right;
          --size_;
          break;
        }

        delete curr;
        --size_;
        break;
      }

      if (cmp_(key, curr->value_.first))
      {
        curr = curr->left_;
      }
      else
      {
        curr = curr->right_;
      }
    }

    if (!found)
    {
      return 0;
    }

    rebalanceUp(rebalance_from);

    return 1;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator
    AVLTree< Key, Value, Compare >::begin() noexcept
  {
    return iterator(fallLeft(root_));
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::iterator
    AVLTree< Key, Value, Compare >::end() noexcept
  {
    return iterator();
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
    AVLTree< Key, Value, Compare >::begin() const noexcept
  {
    return const_iterator(fallLeft(root_));
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
    AVLTree< Key, Value, Compare >::end() const noexcept
  {
    return const_iterator();
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
    AVLTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::const_iterator
    AVLTree< Key, Value, Compare >::cend() const noexcept
  {
    return end();
  }
}

#endif
