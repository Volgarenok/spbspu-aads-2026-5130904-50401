#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <memory>
#include <utility>
#include <stdexcept>
#include <functional>

namespace hvostov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      std::pair< const Key, Value > data;
      Node< Key, Value >* parent;
      Node< Key, Value >* left;
      Node< Key, Value >* right;
      size_t height = 0;

      Node() = default;
      Node(const Key& k, const Value& v, Node* p);
      Node(const Key& k, Value&& v, Node* p);
      ~Node() = default;
    };
  }

  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree {
  public:
    using node_t = detail::Node< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;
    ~BSTree();

    void push(const Key& k, const Value& v);
    void push(const Key& k, Value&& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    Value drop(const Key& k);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

    void swap(BSTree& other) noexcept;
    void clear() noexcept;
    bool has(const Key& k) const;
    bool empty() const noexcept;
    size_t size() const noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

  private:
    node_t* root_;
    size_t size_;
    Compare comp_;

    node_t* copy(node_t* other, node_t* parent);
    void clearSubtree(node_t* node) noexcept;
    size_t calcHeight(node_t* node) const;
    void updateHeightUpwards(node_t* node);
    node_t* findNode(const Key& k) const;
    node_t* minimum(node_t* node) const;
    node_t* maximum(node_t* node) const;
  };

  template< class Key, class Value >
  class BSTIterator {
  public:
    BSTIterator() = default;
    BSTIterator(const BSTIterator& other) = default;
    BSTIterator(BSTIterator&& other) noexcept = default;
    BSTIterator& operator=(const BSTIterator& other) = default;
    BSTIterator& operator=(BSTIterator&& other) noexcept = default;
    ~BSTIterator() = default;

    detail::Node< Key, Value >& operator*();
    detail::Node< Key, Value >* operator->();

    BSTIterator& operator++();
    BSTIterator operator++(int);
    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

  private:
    template< class, class, class >
    friend class BSTree;
    detail::Node< Key, Value >* curr_;
    BSTIterator(detail::Node< Key, Value >* node);
  };

  template< class Key, class Value >
  class BSTConstIterator {
  public:
    BSTConstIterator() = default;
    BSTConstIterator(const BSTConstIterator& other) = default;
    BSTConstIterator(BSTConstIterator&& other) noexcept = default;
    BSTConstIterator& operator=(const BSTConstIterator& other) = default;
    BSTConstIterator& operator=(BSTConstIterator&& other) noexcept = default;
    ~BSTConstIterator() = default;

    const detail::Node< Key, Value >& operator*() const;
    const detail::Node< Key, Value >* operator->() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator& other) const;
    bool operator!=(const BSTConstIterator& other) const;

  private:
    template< class, class, class >
    friend class BSTree;
    detail::Node< Key, Value >* curr_;
    BSTConstIterator(detail::Node< Key, Value >* node);
  };

}

template< class Key, class Value >
hvostov::detail::Node< Key, Value >::Node(const Key& k, const Value& v, Node* p):
  data(k, v),
  parent(p),
  left(nullptr),
  right(nullptr),
  height(0)
{}

template< class Key, class Value >
hvostov::detail::Node< Key, Value >::Node(const Key& k, Value&& v, Node* p):
  data(k, std::move(v)),
  parent(p),
  left(nullptr),
  right(nullptr),
  height(0)
{}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >::BSTree():
  root_(nullptr),
  size_(0),
  comp_(Compare())
{}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(nullptr),
  size_(other.size_),
  comp_(other.comp_)
{
  root_ = copy(other.root_, nullptr);
}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(std::exchange(other.root_, nullptr)),
  size_(std::exchange(other.size_, 0)),
  comp_(std::move(other.comp_))
{}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >& hvostov::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != std::addressof(other)) {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >& hvostov::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != std::addressof(other)) {
    clear();
    root_ = std::exchange(other.root_, nullptr);
    size_ = std::exchange(other.size_, 0);
    comp_ = std::move(other.comp_);
  }
  return *this;
}

template< class Key, class Value, class Compare >
hvostov::BSTree< Key, Value, Compare >::~BSTree()
{
  clear();
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::node_t* hvostov::BSTree< Key, Value, Compare >::copy(node_t* other,
                                                                                                      node_t* parent)
{
  if (!other) {
    return nullptr;
  }
  node_t* new_node = new node_t(other->data.first, other->data.second, parent);
  new_node->height = other->height;
  new_node->left = copy(other->left, new_node);
  new_node->right = copy(other->right, new_node);
  return new_node;
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  if (!root_) {
    root_ = new node_t(k, v, nullptr);
    size_ = 1;
    root_->height = 1;
    return;
  }
  node_t* current = root_;
  node_t* parent = nullptr;
  while (current) {
    parent = current;
    if (comp_(k, current->data.first)) {
      current = current->left;
    } else if (comp_(current->data.first, k)) {
      current = current->right;
    } else {
      current->data.second = v;
      return;
    }
  }

  node_t* new_node = new node_t(k, v, parent);
  if (comp_(k, parent->data.first)) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
  size_++;
  updateHeightUpwards(parent);
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::push(const Key& k, Value&& v)
{
  if (!root_) {
    root_ = new node_t(k, std::move(v), nullptr);
    size_ = 1;
    root_->height = 1;
    return;
  }
  node_t* current = root_;
  node_t* parent = nullptr;

  while (current) {
    parent = current;
    if (comp_(k, current->data.first)) {
      current = current->left;
    } else if (comp_(current->data.first, k)) {
      current = current->right;
    } else {
      current->data.second = std::move(v);
      return;
    }
  }

  node_t* new_node = new node_t(k, std::move(v), parent);
  if (comp_(k, parent->data.first)) {
    parent->left = new_node;
  } else {
    parent->right = new_node;
  }
  size_++;
  updateHeightUpwards(parent);
}

template< class Key, class Value, class Compare >
Value& hvostov::BSTree< Key, Value, Compare >::get(const Key& k)
{
  node_t* node = findNode(k);
  if (!node) {
    throw std::runtime_error("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
const Value& hvostov::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  node_t* node = findNode(k);
  if (!node) {
    throw std::runtime_error("Key not found");
  }
  return node->data.second;
}

template< class Key, class Value, class Compare >
Value hvostov::BSTree< Key, Value, Compare >::drop(const Key& k)
{
  node_t* node = findNode(k);
  if (!node) {
    throw std::runtime_error("Key not found");
  }

  Value result = std::move(node->data.second);
  node_t* parent = node->parent;
  node_t* height_start = parent;

  if (!node->left && !node->right) {
    if (parent) {
      if (parent->left == node)
        parent->left = nullptr;
      else
        parent->right = nullptr;
    } else {
      root_ = nullptr;
    }
    delete node;
  } else if (!node->right) {
    node->left->parent = parent;
    if (parent) {
      if (parent->left == node)
        parent->left = node->left;
      else
        parent->right = node->left;
    } else {
      root_ = node->left;
    }
    delete node;
  } else if (!node->left) {
    node->right->parent = parent;
    if (parent) {
      if (parent->left == node)
        parent->right = node->right;
      else
        parent->right = node->right;
    } else {
      root_ = node->right;
    }
    delete node;
  } else {
    node_t* successor = minimum(node->right);
    height_start = successor->parent;
    if (height_start == node) {
      height_start = successor;
    }
    if (successor->parent != node) {
      successor->parent->left = successor->right;
      if (successor->right) {
        successor->right->parent = successor->parent;
      }
      successor->right = node->right;
      node->right->parent = successor;
    }
    successor->left = node->left;
    if (node->left)
      node->left->parent = successor;
    successor->parent = parent;
    if (parent) {
      if (parent->left == node)
        parent->left = successor;
      else
        parent->right = successor;
    } else {
      root_ = successor;
    }
    delete node;
  }
  size_--;
  updateHeightUpwards(height_start);
  return result;
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
{
  node_t* node = it.curr_;
  if (!node || !node->right) {
    return it;
  }
  node_t* right_child = node->right;
  node_t* parent = node->parent;

  node->right = right_child->left;
  if (right_child->left) {
    right_child->left->parent = node;
  }
  right_child->left = node;
  right_child->parent = parent;
  node->parent = right_child;
  if (parent) {
    if (parent->left == node)
      parent->left = right_child;
    else
      parent->right = right_child;
  } else {
    root_ = right_child;
  }
  updateHeightUpwards(node);
  return {right_child};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
{
  node_t* node = it.curr_;
  if (!node || !node->left) {
    return it;
  }
  node_t* left_child = node->left;
  node_t* parent = node->parent;

  node->left = left_child->right;
  if (left_child->right) {
    left_child->right->parent = node;
  }
  left_child->right = node;
  left_child->parent = parent;
  node->parent = left_child;
  if (parent) {
    if (parent->left == node)
      parent->left = left_child;
    else
      parent->right = left_child;
  } else {
    root_ = left_child;
  }
  updateHeightUpwards(node);
  return {left_child};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
{
  node_t* node = it.curr_;
  if (!node) {
    return it;
  }

  if (node->left && node->left->right) {
    rotateLeft({node->left});
  }
  return rotateRight({node});
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
{
  node_t* node = it.curr_;
  if (!node) {
    return it;
  }

  if (node->right && node->right->left) {
    rotateRight({node->right});
  }
  return rotateLeft({node});
}

template< class Key, class Value, class Compare >
size_t hvostov::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return it.curr_ ? it.curr_->height : 0;
}

template< class Key, class Value, class Compare >
size_t hvostov::BSTree< Key, Value, Compare >::height() const
{
  return root_ ? root_->height : 0;
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(size_, other.size_);
  std::swap(comp_, other.comp_);
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::clear() noexcept
{
  clearSubtree(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::clearSubtree(node_t* node) noexcept
{
  if (!node) {
    return;
  }
  clearSubtree(node->left);
  clearSubtree(node->right);
  delete node;
}

template< class Key, class Value, class Compare >
bool hvostov::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return size_ == 0;
}

template< class Key, class Value, class Compare >
size_t hvostov::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::iterator hvostov::BSTree< Key, Value, Compare >::begin()
{
  return {minimum(root_)};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::iterator hvostov::BSTree< Key, Value, Compare >::end()
{
  return {nullptr};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator hvostov::BSTree< Key, Value, Compare >::begin() const
{
  return {minimum(root_)};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator hvostov::BSTree< Key, Value, Compare >::end() const
{
  return {nullptr};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::cbegin() const noexcept
{
  return {minimum(root_)};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::const_iterator
hvostov::BSTree< Key, Value, Compare >::cend() const noexcept
{
  return {nullptr};
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::node_t*
hvostov::BSTree< Key, Value, Compare >::findNode(const Key& k) const
{
  node_t* current = root_;
  while (current) {
    if (comp_(k, current->data.first)) {
      current = current->left;
    } else if (comp_(current->data.first, k)) {
      current = current->right;
    } else {
      return current;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::node_t*
hvostov::BSTree< Key, Value, Compare >::minimum(node_t* node) const
{
  if (!node) {
    return nullptr;
  }
  while (node->left) {
    node = node->left;
  }
  return node;
}

template< class Key, class Value, class Compare >
typename hvostov::BSTree< Key, Value, Compare >::node_t*
hvostov::BSTree< Key, Value, Compare >::maximum(node_t* node) const
{
  if (!node) {
    return nullptr;
  }
  while (node->right) {
    node = node->right;
  }
  return node;
}

template< class Key, class Value, class Compare >
size_t hvostov::BSTree< Key, Value, Compare >::calcHeight(node_t* node) const
{
  if (!node) {
    return 0;
  }
  return 1 + std::max(calcHeight(node->left), calcHeight(node->right));
}

template< class Key, class Value, class Compare >
void hvostov::BSTree< Key, Value, Compare >::updateHeightUpwards(node_t* node)
{
  while (node) {
    node->height = calcHeight(node);
    node = node->parent;
  }
}

template< class Key, class Value >
hvostov::BSTIterator< Key, Value >::BSTIterator(detail::Node< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
hvostov::detail::Node< Key, Value >& hvostov::BSTIterator< Key, Value >::operator*()
{
  return *curr_;
}

template< class Key, class Value >
hvostov::detail::Node< Key, Value >* hvostov::BSTIterator< Key, Value >::operator->()
{
  return curr_;
}

template< class Key, class Value >
hvostov::BSTIterator< Key, Value >& hvostov::BSTIterator< Key, Value >::operator++()
{
  if (!curr_) {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (next->right) {
    next = next->right;
    while (next->left) {
      next = next->left;
    }
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && parent->left != next) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
hvostov::BSTIterator< Key, Value > hvostov::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value >
hvostov::BSTIterator< Key, Value >& hvostov::BSTIterator< Key, Value >::operator--()
{
  if (!curr_) {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (next->left) {
    next = next->left;
    while (next->right) {
      next = next->right;
    }
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && next != parent->right) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
hvostov::BSTIterator< Key, Value > hvostov::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value >
bool hvostov::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool hvostov::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
hvostov::BSTConstIterator< Key, Value >::BSTConstIterator(detail::Node< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
const hvostov::detail::Node< Key, Value >& hvostov::BSTConstIterator< Key, Value >::operator*() const
{
  return *curr_;
}

template< class Key, class Value >
const hvostov::detail::Node< Key, Value >* hvostov::BSTConstIterator< Key, Value >::operator->() const
{
  return curr_;
}

template< class Key, class Value >
hvostov::BSTConstIterator< Key, Value >& hvostov::BSTConstIterator< Key, Value >::operator++()
{
  if (!curr_) {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (next->right) {
    next = next->right;
    while (next->left) {
      next = next->left;
    }
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && parent->left != next) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
hvostov::BSTConstIterator< Key, Value > hvostov::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value >
hvostov::BSTConstIterator< Key, Value >& hvostov::BSTConstIterator< Key, Value >::operator--()
{
  if (!curr_) {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (next->left) {
    next = next->left;
    while (next->right) {
      next = next->right;
    }
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && next != parent->right) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
hvostov::BSTConstIterator< Key, Value > hvostov::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value >
bool hvostov::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool hvostov::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value, class Compare >
bool hvostov::BSTree< Key, Value, Compare >::has(const Key& k) const
{
  return findNode(k) != nullptr;
}

#endif
