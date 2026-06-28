#ifndef MY_TREE_HPP
#define MY_TREE_HPP

#include "my_citer_tree.hpp"
#include "my_iter_tree.hpp"
#include "my_node_tree.hpp"
#include <cstddef>

namespace zubarev
{

  template< class Key, class Value, class Compare >
  class BSTree
  {
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;

  private:
    using Node = BSTreeNode< Key, Value >;
    Node* fake_root_;
    Compare comp_;

    size_t height_implement(Node* node) const
    {
      if (!node || node == fake_root_) {
        return 0;
      }
      return 1 + std::max(height_implement(node->right_), height_implement(node->left_));
    }

    void destroy(Node* node)
    {
      if (!node || node == fake_root_) {
        return;
      }

      destroy(node->left_);
      destroy(node->right_);

      node->left_ = node->right_ = node->parent_ = nullptr;
      delete node;
    }
    Node* clone(Node* node, Node* parent)
    {
      if (!node || node == fake_root_) {
        return nullptr;
      }
      Node* new_node = new Node(node->data_.first, node->data_.second);
      new_node->parent_ = parent;
      new_node->left_ = clone(node->left_, new_node);
      new_node->right_ = clone(node->right_, new_node);

      return new_node;
    }

    Node* find_node(const Key& k) const
    {
      Node* cur = fake_root_->parent_;
      while (cur && cur != fake_root_) {
        if (comp_(k, cur->data_.first)) {
          cur = cur->left_;
        } else if (comp_(cur->data_.first, k)) {
          cur = cur->right_;
        } else {
          return cur;
        }
      }

      return nullptr;
    }

    Node* fall_left(Node* node) const
    {
      if (!node || node == fake_root_) {
        return fake_root_;
      }
      while (node->left_) {
        node = node->left_;
      }
      return node;
    }

    Node* fall_right(Node* node) const
    {
      if (!node || node == fake_root_) {
        return fake_root_;
      }

      while (node->right_) {
        node = node->right_;
      }
      return node;
    }

  public:
    BSTree();
    ~BSTree();
    BSTree(const BSTree& table);
    BSTree(BSTree&& table) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    Value& operator[](Key k);
    const Value& operator[](Key id) const;
    Value& at(Key id);
    const Value& at(Key id) const;
    bool contains(Key k) const noexcept;

    BSTIterator< Key, Value > begin();
    BSTIterator< Key, Value > end();

    BSTConstIterator< Key, Value > cbegin() const;
    BSTConstIterator< Key, Value > cend() const;

    BSTConstIterator< Key, Value > begin() const;
    BSTConstIterator< Key, Value > end() const;

    void push(Key k, Value v);
    Value get(Key k);
    Value drop(Key k);

    void swap(BSTree& rhs) noexcept;

    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;
    bool empty() const;
  };
  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return fake_root_->parent_ == fake_root_;
  }
  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(Key k) const noexcept
  {
    if (find_node(k) == nullptr) {
      return false;
    }
    return true;
  }
  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree& rhs) noexcept
  {
    std::swap(fake_root_, rhs.fake_root_);
    std::swap(comp_, rhs.comp_);
  }
  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree()
  {

    fake_root_ = new Node(Key{}, Value{});
    fake_root_->left_ = fake_root_->right_ = fake_root_->parent_ = fake_root_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    if (fake_root_) {

      destroy(fake_root_->parent_);

      delete fake_root_;

      fake_root_ = nullptr;
    }
  }

  template< class Key, class Value, class Compare >

  BSTree< Key, Value, Compare >::BSTree(const BSTree& tree):
    comp_(tree.comp_)
  {
    fake_root_ = new Node(Key{}, Value{});
    fake_root_->left_ = fake_root_->right_ = fake_root_->parent_ = fake_root_;

    if (tree.fake_root_->parent_ && tree.fake_root_->parent_ != tree.fake_root_) {
      fake_root_->parent_ = clone(tree.fake_root_->parent_, fake_root_);
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept
  {
    fake_root_ = new Node(Key{}, Value{});
    fake_root_->left_ = fake_root_->right_ = fake_root_->parent_ = fake_root_;

    if (other.fake_root_->parent_ != other.fake_root_) {
      delete fake_root_;
      swap(other);

      other.fake_root_ = new Node(Key{}, Value{});
      other.fake_root_->left_ = other.fake_root_->right_ = other.fake_root_->parent_ = other.fake_root_;
    }
  }
  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree& rhs)
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }
    BSTree cpy(rhs);
    swap(cpy);
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree&& rhs) noexcept
  {
    if (this == std::addressof(rhs)) {
      return *this;
    }

    if (fake_root_) {
      destroy(fake_root_->parent_);
      delete fake_root_;
    }

    swap(rhs);

    rhs.fake_root_ = new Node(Key{}, Value{});
    rhs.fake_root_->left_ = rhs.fake_root_->right_ = rhs.fake_root_->parent_ = rhs.fake_root_;

    return *this;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](Key k)
  {

    Node* node = find_node(k);

    if (!node) {

      push(k, Value{});
      node = find_node(k);
    }

    return node->data_.second;
  }
  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::operator[](Key k) const
  {
    return at(k);
  }
  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::at(Key id)
  {
    Node* node = find_node(id);
    if (node) {
      return node->data_.second;
    } else {
      throw std::out_of_range("BSTree: key out of range");
    }
  }
  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::at(Key id) const
  {
    const Node* node = find_node(id);
    if (node) {
      return node->data_.second;
    } else {
      throw std::out_of_range("BSTree: key out of range");
    }
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::begin()
  {
    return iterator(fall_left(fake_root_->parent_));
  }
  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::end()
  {
    return iterator(fake_root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cbegin() const
  {
    return const_iterator(fall_left(fake_root_->parent_));
  }
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cend() const
  {
    return const_iterator(fake_root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::begin() const
  {
    return const_iterator(fall_left(fake_root_->parent_));
  }
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(fake_root_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key k, Value v)
  {
    Node* cur = fake_root_->parent_;
    Node* parent = fake_root_;
    while (cur && cur != fake_root_) {
      parent = cur;
      if (comp_(k, cur->data_.first)) {
        cur = cur->left_;

      } else if (comp_(cur->data_.first, k)) {
        cur = cur->right_;
      } else {
        throw std::logic_error("Key already exists");
        return;
      }
    }

    Node* tmp = new Node(k, v);
    tmp->parent_ = parent;
    if (parent == fake_root_) {
      fake_root_->parent_ = tmp;
    } else if (comp_(k, parent->data_.first)) {
      parent->left_ = tmp;
    } else {
      parent->right_ = tmp;
    }
  }
  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::get(Key k)
  {
    Node* tmp = find_node(k);
    if (!tmp || tmp == fake_root_) {
      throw std::out_of_range("Key not found");
    }
    return tmp->data_.second;
  }
  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(Key k)
  {
    Node* find = find_node(k);
    if (!find || find == fake_root_) {
      throw std::out_of_range("Key not found");
    }
    Value saved_val = find->data_.second;

    if (find->left_ && find->right_) {
      Node* tmp = find->right_;
      tmp = fall_left(tmp);
      find->data_ = tmp->data_;
      find = tmp;
    }

    Node* child = find->left_ ? find->left_ : find->right_;

    if (find->parent_ != fake_root_) {
      if (find->parent_->left_ == find) {
        find->parent_->left_ = child;
      } else {
        find->parent_->right_ = child;
      }
    } else {
      fake_root_->parent_ = child;
    }
    delete find;

    return saved_val;
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    Node* z = (*it);
    if (!z || z == fake_root_ || !z->right_ || z->right_ == fake_root_) {
      throw std::invalid_argument("rotateRight: no left child");
    }
    Node* y = z->right_;
    Node* z_parent = z->parent_;
    Node* y_child = y->left_;

    y->left_ = z;
    z->right_ = y_child;

    if (y_child && y_child != fake_root_) {
      y_child->parent_ = z;
    }
    y->parent_ = z_parent;
    z->parent_ = y;

    if (z_parent == fake_root_) {
      fake_root_->parent_ = y;
    } else if (z_parent->right_ == (*it)) {
      z_parent->right_ = y;
    } else {
      z_parent->left_ = y;
    }
    return const_iterator(y);
  }
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    Node* z = (*it);
    if (!z || z == fake_root_ || !z->left_ || z->left_ == fake_root_) {
      throw std::invalid_argument("rotateRight: no left child");
    }
    Node* y = z->left_;
    Node* z_parent = z->parent_;
    Node* y_child = y->right_;

    y->right_ = z;
    z->left_ = y_child;

    if (y_child && y_child != fake_root_) {
      y_child->parent_ = z;
    }
    y->parent_ = z_parent;
    z->parent_ = y;

    if (z_parent == fake_root_) {
      fake_root_->parent_ = y;
    } else if (z_parent->right_ == (*it)) {
      z_parent->right_ = y;
    } else {
      z_parent->left_ = y;
    }
    return const_iterator(y);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    Node* y = (*it)->left_;
    rotateRight(const_iterator(y));
    return rotateLeft(it);
  }
  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    Node* y = (*it)->left_;
    rotateLeft(const_iterator(y));
    return rotateRight(it);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return height_implement(*it);
  }
  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return height_implement(fake_root_->parent_);
  }
}

#endif
