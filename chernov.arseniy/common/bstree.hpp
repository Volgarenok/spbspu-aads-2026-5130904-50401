#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <stdexcept>
#include <type_traits>
#include <utility>

#include <stack.hpp>

namespace chernov {
  namespace detail {
    struct NodeBase {
      NodeBase * parent = nullptr;
      NodeBase * left = nullptr;
      NodeBase * right = nullptr;
      size_t height = 0;

      NodeBase() = default;
      NodeBase(NodeBase * p, NodeBase * l, NodeBase * r, size_t h):
        parent(p),
        left(l),
        right(r),
        height(h)
      {}
    };

    template< class Key, class Value >
    struct Node: NodeBase {
      std::pair< const Key, Value > key_value_;

      Node(const Key & k, const Value & v, NodeBase * p, NodeBase * l, NodeBase * r, size_t h):
        NodeBase(p, l, r, h),
        key_value_(k, v)
      {}
    };
  }

  template< class Key, class Value, bool IsConst >
  class BSTIterator;

  template< class Key, class Value, class Compare >
  class BSTree {
  public:
    using iterator = BSTIterator< Key, Value, false >;
    using const_iterator = BSTIterator< Key, Value, true >;

    BSTree();
    BSTree(const BSTree & other);
    BSTree(BSTree && other) noexcept;
    ~BSTree() noexcept;

    BSTree & operator=(const BSTree & other);
    BSTree & operator=(BSTree && other) noexcept;

    void swap(BSTree & other) noexcept;

    template< class U >
    void push(const Key & k, U && v);
    void remove(const Key & k);

    void clear() noexcept;

    const_iterator rotateLeft(const_iterator iter);
    const_iterator rotateRight(const_iterator iter);

    const_iterator rotateLargeLeft(const_iterator iter);
    const_iterator rotateLargeRight(const_iterator iter);

    Value & at(const Key & k);
    const Value & at(const Key & k) const;

    bool empty() const noexcept;
    size_t size() const noexcept;
    bool contains(const Key & k) const noexcept;

    size_t height() const noexcept;
    size_t height(const_iterator iter) const noexcept;

    iterator beforeBegin() noexcept;
    const_iterator beforeBegin() const noexcept;
    const_iterator cbeforeBegin() const noexcept;

    iterator begin() noexcept;
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;

    iterator end() noexcept;
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;

    iterator findIter(const Key & k) noexcept;
    const_iterator findIter(const Key & k) const noexcept;
    const_iterator cfindIter(const Key & k) const noexcept;

  private:
    detail::NodeBase * fake_root_;
    detail::NodeBase * fake_leaf_;
    Compare cmp_;
    size_t size_;

    void createFakes();
    void updateHeights(detail::NodeBase * node) noexcept;
    detail::NodeBase * findNode(const Key & k) const;
  };

  template< class Key, class Value, bool IsConst >
  class BSTIterator {
  public:
    using value_type = std::pair< const Key, Value >;
    using reference = std::conditional_t< IsConst, const value_type &, value_type & >;
    using pointer = std::conditional_t< IsConst, const value_type *, value_type * >;

    BSTIterator() = delete;

    reference operator*() const;
    pointer operator->() const;

    BSTIterator & operator++();
    BSTIterator operator++(int);
    BSTIterator & operator--();
    BSTIterator operator--(int);

    template< bool OtherConst >
    bool operator==(const BSTIterator< Key, Value, OtherConst > & other) const noexcept;

    template< bool OtherConst >
    bool operator!=(const BSTIterator< Key, Value, OtherConst > & other) const noexcept;

  private:
    template< class, class, class >
    friend class BSTree;

    detail::NodeBase * node_;
    detail::NodeBase * fake_root_;
    detail::NodeBase * fake_leaf_;

    explicit BSTIterator(detail::NodeBase * node, detail::NodeBase * fake_root, detail::NodeBase * fake_leaf);
    detail::NodeBase * fallMinimum(detail::NodeBase * node) const noexcept;
    detail::NodeBase * fallMaximum(detail::NodeBase * node) const noexcept;
  };

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::createFakes()
  {
    fake_leaf_ = new detail::NodeBase();
    try {
      fake_root_ = new detail::NodeBase(nullptr, fake_leaf_, fake_leaf_, 0);
    } catch (...) {
      delete fake_leaf_;
      throw;
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    fake_root_(nullptr),
    fake_leaf_(nullptr),
    cmp_(Compare{}),
    size_(0)
  {
    createFakes();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
    fake_root_(nullptr),
    fake_leaf_(nullptr),
    cmp_(),
    size_(0)
  {
    createFakes();

    if (other.fake_root_->left == other.fake_leaf_) {
      return;
    }

    BSTree< Key, Value, Compare > temp;
    temp.cmp_ = other.cmp_;
    temp.size_ = other.size_;

    Stack< std::pair< detail::NodeBase *, detail::NodeBase * > > stack;

    detail::NodeBase * src_root = other.fake_root_->left;
    const std::pair< const Key, Value > & root_kv = static_cast< detail::Node< Key, Value > * >(src_root)->key_value_;
    detail::NodeBase * dst_root = new detail::Node< Key, Value >(root_kv.first,
      root_kv.second, temp.fake_root_, temp.fake_leaf_, temp.fake_leaf_, src_root->height);
    temp.fake_root_->left = dst_root;
    temp.fake_root_->right = dst_root;
    stack.push({src_root, dst_root});

    while (!stack.empty()) {
      detail::NodeBase * src = stack.top().first;
      detail::NodeBase * dst = stack.top().second;
      stack.pop();

      if (src->left != other.fake_leaf_) {
        const std::pair< const Key, Value > & l_kv = static_cast< detail::Node< Key, Value > * >(src->left)->key_value_;
        detail::NodeBase * new_left = new detail::Node< Key, Value >(l_kv.first,
          l_kv.second, dst, temp.fake_leaf_, temp.fake_leaf_, src->height);
        dst->left = new_left;
        new_left->parent = dst;
        stack.push({src->left, new_left});
      } else {
        dst->left = temp.fake_leaf_;
      }

      if (src->right != other.fake_leaf_) {
        const std::pair< const Key, Value > & r_kv = static_cast< detail::Node< Key, Value > * >(src->right)->key_value_;
        detail::NodeBase * new_right = new detail::Node< Key, Value >(r_kv.first,
          r_kv.second, dst, temp.fake_leaf_, temp.fake_leaf_, src->height);
        dst->right = new_right;
        new_right->parent = dst;
        stack.push({src->right, new_right});
      } else {
        dst->right = temp.fake_leaf_;
      }
    }

    swap(temp);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree && other) noexcept:
    fake_root_(std::exchange(other.fake_root_, nullptr)),
    fake_leaf_(std::exchange(other.fake_leaf_, nullptr)),
    cmp_(other.cmp_),
    size_(std::exchange(other.size_, 0))
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree() noexcept
  {
    if (!fake_root_) {
      return;
    }

    clear();

    delete fake_root_;
    delete fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
  {
    std::swap(fake_root_, other.fake_root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(cmp_, other.cmp_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(const BSTree & other)
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    BSTree< Key, Value, Compare > temp(other);
    swap(temp);
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(BSTree && other) noexcept
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    BSTree< Key, Value, Compare > temp(std::move(other));
    swap(temp);
    return *this;
  }

  template< class Key, class Value, class Compare >
  template< class U >
  void BSTree< Key, Value, Compare >::push(const Key & k, U && v)
  {
    detail::NodeBase * parent = fake_root_;
    detail::NodeBase * curr = parent->left;
    while (curr != fake_leaf_) {
      parent = curr;
      const Key & curr_key = static_cast< detail::Node< Key, Value > * >(curr)->key_value_.first;
      if (cmp_(k, curr_key)) {
        curr = curr->left;
      } else if (cmp_(curr_key, k)) {
        curr = curr->right;
      } else {
        static_cast< detail::Node< Key, Value > * >(curr)->key_value_.second = std::forward< U >(v);
        return;
      }
    }

    detail::NodeBase * new_node = new detail::Node< Key, Value >(k, std::forward< U >(v),
      parent, fake_leaf_, fake_leaf_, 1);

    if (parent == fake_root_) {
      fake_root_->left = new_node;
      fake_root_->right = new_node;
    } else if (cmp_(k, static_cast< detail::Node< Key, Value > * >(parent)->key_value_.first)) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    ++size_;
    updateHeights(new_node);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::remove(const Key & k)
  {
    detail::NodeBase * node = findNode(k);

    detail::NodeBase * moved_node = node;
    detail::NodeBase * child = fake_leaf_;

    if (node->left != fake_leaf_ && node->right != fake_leaf_) {
      moved_node = node->right;
      while (moved_node->left != fake_leaf_) {
        moved_node = moved_node->left;
      }
    }

    child = (moved_node->left != fake_leaf_) ? moved_node->left : moved_node->right;

    if (child != fake_leaf_) {
      child->parent = moved_node->parent;
    }
    if (moved_node->parent->left == moved_node) {
      moved_node->parent->left = child;
    } else {
      moved_node->parent->right = child;
    }
    if (moved_node->parent == fake_root_) {
      fake_root_->left = child;
      fake_root_->right = child;
    }

    detail::NodeBase * parent_for_height = moved_node->parent;

    if (moved_node == node) {
      delete node;
      --size_;
      updateHeights(parent_for_height);
      return;
    }

    detail::NodeBase * node_left   = node->left;
    detail::NodeBase * node_right  = node->right;
    detail::NodeBase * node_parent = node->parent;

    moved_node->parent = node_parent;
    if (node_parent == fake_root_) {
      fake_root_->left = moved_node;
      fake_root_->right = moved_node;
    } else if (node_parent->left == node) {
      node_parent->left = moved_node;
    } else {
      node_parent->right = moved_node;
    }

    moved_node->left = node_left;
    if (node_left != fake_leaf_) node_left->parent = moved_node;

    moved_node->right = node_right;
    if (node_right != fake_leaf_) node_right->parent = moved_node;

    delete node;
    --size_;
    updateHeights(moved_node);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear() noexcept
  {
    detail::NodeBase * root = fake_root_->left;
    if (root == fake_leaf_) {
      return;
    }

    Stack< detail::NodeBase * > stack;
    stack.push(root);

    while (!stack.empty()) {
      detail::NodeBase * node = stack.top();
      stack.pop();
      if (node->left != fake_leaf_) {
        stack.push(node->left);
      }
      if (node->right != fake_leaf_) {
        stack.push(node->right);
      }
      delete node;
    }
    fake_root_->left = fake_leaf_;
    fake_root_->right = fake_leaf_;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLeft(const_iterator iter)
  {
    detail::NodeBase * root = iter.node_;
    detail::NodeBase * new_root = root->right;
    if (new_root == fake_leaf_) {
      throw std::logic_error("rotation is not possible");
    }

    root->right = new_root->left;
    if (new_root->left != fake_leaf_) {
      new_root->left->parent = root;
    }

    new_root->parent = root->parent;
    if (root->parent == fake_root_) {
      fake_root_->left = new_root;
      fake_root_->right = new_root;
    } else if (root->parent->right == root) {
      root->parent->right = new_root;
    } else {
      root->parent->left = new_root;
    }

    root->parent = new_root;
    new_root->left = root;

    updateHeights(root);

    return const_iterator(new_root, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateRight(const_iterator iter)
  {
    detail::NodeBase * root = iter.node_;
    detail::NodeBase * new_root = root->left;
    if (new_root == fake_leaf_) {
      throw std::logic_error("rotation is not possible");
    }

    root->left = new_root->right;
    if (new_root->right != fake_leaf_) {
      new_root->right->parent = root;
    }

    new_root->parent = root->parent;
    if (root->parent == fake_root_) {
      fake_root_->left = new_root;
      fake_root_->right = new_root;
    } else if (root->parent->left == root) {
      root->parent->left = new_root;
    } else {
      root->parent->right = new_root;
    }

    root->parent = new_root;
    new_root->right = root;

    updateHeights(root);

    return const_iterator(new_root, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator iter)
  {
    detail::NodeBase * root = iter.node_;
    if (root->right == fake_leaf_) {
      throw std::logic_error("rotate is not possible");
    }

    rotateRight(const_iterator(root->right, fake_root_, fake_leaf_));
    return rotateLeft(iter);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
  BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator iter)
  {
    detail::NodeBase * root = iter.node_;
    if (root->left == fake_leaf_) {
      throw std::logic_error("rotate is not possible");
    }

    rotateLeft(const_iterator(root->left, fake_root_, fake_leaf_));
    return rotateRight(iter);
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::at(const Key & k)
  {
    const BSTree< Key, Value, Compare > * cthis = this;
    return const_cast< Value & >(cthis->at(k));
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::at(const Key & k) const
  {
    return static_cast< detail::Node< Key, Value > * >(findNode(k))->key_value_.second;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(const Key & k) const noexcept
  {
    try {
      findNode(k);
    } catch (const std::out_of_range &) {
      return false;
    }
    return true;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const noexcept
  {
    return fake_root_->left->height;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator iter) const noexcept
  {
    return iter.node_->height;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::iterator
  BSTree< Key, Value, Compare >::beforeBegin() noexcept
  {
    return iterator(fake_root_, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::beforeBegin() const noexcept
  {
    return const_iterator(fake_root_, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::cbeforeBegin() const noexcept
  {
    return beforeBegin();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::iterator
  BSTree< Key, Value, Compare >::begin() noexcept
  {
    detail::NodeBase * first = fake_root_->left;
    if (first != fake_leaf_) {
      while (first->left != fake_leaf_) {
        first = first->left;
      }
    }
    return iterator(first, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::begin() const noexcept
  {
    detail::NodeBase * first = fake_root_->left;
    if (first != fake_leaf_) {
      while (first->left != fake_leaf_) {
        first = first->left;
      }
    }
    return const_iterator(first, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::cbegin() const noexcept
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::iterator
  BSTree< Key, Value, Compare >::end() noexcept
  {
    return iterator(fake_leaf_, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::end() const noexcept
  {
    return const_iterator(fake_leaf_, fake_root_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::cend() const noexcept
  {
    return end();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::iterator
  BSTree< Key, Value, Compare >::findIter(const Key & k) noexcept
  {
    try {
      return iterator(findNode(k), fake_root_, fake_leaf_);
    } catch (...) {
      return end();
    }
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::findIter(const Key & k) const noexcept
  {
    try {
      return const_iterator(findNode(k), fake_root_, fake_leaf_);
    } catch (...) {
      return cend();
    }
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare>::const_iterator
  BSTree< Key, Value, Compare >::cfindIter(const Key & k) const noexcept
  {
    return findIter(k);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::updateHeights(detail::NodeBase * node) noexcept
  {
    while (node != fake_root_ && node != nullptr) {
      size_t lh = node->left->height;
      size_t rh = node->right->height;
      node->height = (lh > rh ? lh : rh) + 1;
      node = node->parent;
    }
  }

  template< class Key, class Value, class Compare >
  detail::NodeBase * BSTree< Key, Value, Compare >::findNode(const Key & k) const
  {
    detail::NodeBase * node = fake_root_->left;
    while (node != fake_leaf_) {
      const Key & node_key = static_cast< detail::Node< Key, Value > * >(node)->key_value_.first;
      if (cmp_(k, node_key)) {
        node = node->left;
      } else if (cmp_(node_key, k)) {
        node = node->right;
      } else {
        return node;
      }
    }
    throw std::out_of_range("element not found");
  }

  template< class Key, class Value, bool IsConst >
  BSTIterator< Key, Value, IsConst >::BSTIterator(detail::NodeBase * node,
    detail::NodeBase * fake_root, detail::NodeBase * fake_leaf):
    node_(node),
    fake_root_(fake_root),
    fake_leaf_(fake_leaf)
  {}

  template< class Key, class Value, bool IsConst >
  typename BSTIterator< Key, Value, IsConst >::reference BSTIterator< Key, Value, IsConst >::operator*() const
  {
    return static_cast< detail::Node< Key, Value > * >(node_)->key_value_;
  }

  template< class Key, class Value, bool IsConst >
  typename BSTIterator< Key, Value, IsConst >::pointer BSTIterator< Key, Value, IsConst >::operator->() const
  {
    return std::addressof(static_cast< detail::Node< Key, Value > * >(node_)->key_value_);
  }

  template< class Key, class Value, bool IsConst >
  BSTIterator< Key, Value, IsConst > & BSTIterator< Key, Value, IsConst >::operator++()
  {
    if (node_ == fake_root_) {
      node_ = fallMinimum(fake_root_->left);
      return *this;
    }
    detail::NodeBase * next = node_;
    if (next->right != fake_leaf_) {
      next = next->right;
      next = fallMinimum(next);
    } else {
      detail::NodeBase * parent = next->parent;
      while (parent != fake_root_ && parent->left != next) {
        next = parent;
        parent = next->parent;
      }
      next = parent;
    }
    if (next == fake_root_) {
      next = fake_leaf_;
    }
    node_ = next;
    return *this;
  }

  template< class Key, class Value, bool IsConst >
  BSTIterator< Key, Value, IsConst > BSTIterator< Key, Value, IsConst >::operator++(int)
  {
    BSTIterator temp = *this;
    ++(*this);
    return temp;
  }

  template< class Key, class Value, bool IsConst >
  BSTIterator< Key, Value, IsConst > & BSTIterator< Key, Value, IsConst >::operator--()
  {
    if (node_ == fake_leaf_) {
      node_ = fallMaximum(fake_root_);
      return *this;
    }
    detail::NodeBase * prev = node_;
    if (prev->left != fake_leaf_) {
      prev = prev->left;
      prev = fallMaximum(prev);
    } else {
      detail::NodeBase * parent = prev->parent;
      while (parent != fake_root_ && parent->right != prev) {
        prev = parent;
        parent = prev->parent;
      }
      prev = parent;
    }
    node_ = prev;
    return *this;
  }

  template< class Key, class Value, bool IsConst >
  BSTIterator< Key, Value, IsConst > BSTIterator< Key, Value, IsConst >::operator--(int)
  {
    BSTIterator temp = *this;
    --(*this);
    return temp;
  }

  template< class Key, class Value, bool IsConst >
  template< bool OtherConst >
  bool BSTIterator< Key, Value, IsConst >::
  operator==(const BSTIterator< Key, Value, OtherConst > & other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, bool IsConst >
  template< bool OtherConst >
  bool BSTIterator< Key, Value, IsConst >::
  operator!=(const BSTIterator< Key, Value, OtherConst > & other) const noexcept
  {
    return !(*this == other);
  }

  template< class Key, class Value, bool IsConst >
  detail::NodeBase * BSTIterator< Key, Value, IsConst >::fallMinimum(detail::NodeBase * node) const noexcept
  {
    if (node == fake_leaf_) {
      return node;
    }
    while (node->left != fake_leaf_) {
      node = node->left;
    }
    return node;
  }

  template< class Key, class Value, bool IsConst >
  detail::NodeBase * BSTIterator< Key, Value, IsConst >::fallMaximum(detail::NodeBase * node) const noexcept
  {
    if (node == fake_leaf_) {
      return node;
    }
    while (node->right != fake_leaf_) {
      node = node->right;
    }
    return node;
  }
}

#endif
