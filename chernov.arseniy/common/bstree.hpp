#ifndef BSTREE_HPP
#define BSTREE_HPP

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

    void push(Key k, Value v);
    Value get(Key k);
    void remove(Key k);

    void swap(BSTree & other) noexcept;

    size_t height() const noexcept;
    size_t height(const_iterator iter) const noexcept;

  private:
    detail::NodeBase * fake_root_;
    detail::NodeBase * fake_leaf_;
    Compare cmp_;

    void createFakes();
    void updateHeights(detail::NodeBase * node) noexcept;
  };

  template< class Key, class Value, bool IsConst >
  class BSTIterator {
  public:
    using value_type = std::pair< const Key, Value >;
    using reference = std::conditional_t< IsConst, const value_type &, value_type & >;
    using pointer = std::conditional_t< IsConst, const value_type *, value_type * >;

  private:
    template< class, class, class >
    friend class BSTree;

    detail::NodeBase * node_;
    explicit BSTIterator(detail::NodeBase * node);
  };

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::createFakes()
  {
    fake_leaf_ = new detail::NodeBase();
    try {
      fake_root_ = new detail::NodeBase(nullptr, fake_leaf_, fake_leaf_, 1);
    } catch (...) {
      delete fake_leaf_;
      throw;
    }
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    cmp_(Compare{})
  {
    createFakes();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other)
  {
    createFakes();

    if (other.fake_root_->left == other.fake_leaf_) {
      return;
    }

    BSTree< Key, Value, Compare > temp;
    temp.cmp_ = other.cmp_;

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
    cmp_(other.cmp_)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree() noexcept
  {
    if (!fake_root_) {
      return;
    }
    detail::NodeBase * root = fake_root_->left;
    if (root == fake_leaf_) {
      delete fake_root_;
      delete fake_leaf_;
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

    delete fake_root_;
    delete fake_leaf_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(const BSTree & other)
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    BSTree< Key, Value, Compare > temp(other);
    swap(temp);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(BSTree && other) noexcept
  {
    if (this == std::addressof(other)) {
      return *this;
    }
    BSTree< Key, Value, Compare > temp(std::move(other));
    swap(temp);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key k, Value v)
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
        static_cast< detail::Node< Key, Value > * >(curr)->key_value_.second = v;
        return;
      }
    }

    detail::NodeBase * new_node = new detail::Node< Key, Value >(k, v, parent, fake_leaf_, fake_leaf_, 1);
    if (parent == fake_root_) {
      fake_root_->left = new_node;
      fake_root_->right = new_node;
    }
    if (cmp_(k, static_cast< detail::Node< Key, Value > * >(parent)->key_value_.first)) {
      parent->left = new_node;
    } else {
      parent->right = new_node;
    }

    updateHeights(new_node);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
  {
    std::swap(fake_root_, other.fake_root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(cmp_, other.cmp_);
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
  void BSTree< Key, Value, Compare >::updateHeights(detail::NodeBase * node) noexcept
  {
    while (node != fake_root_) {
      size_t lh = node->parent->left->height;
      size_t rh = node->parent->right->height;
      node->parent->height = (lh > rh ? lh : rh) + 1;
      node = node->parent;
    }
  }
}

#endif
