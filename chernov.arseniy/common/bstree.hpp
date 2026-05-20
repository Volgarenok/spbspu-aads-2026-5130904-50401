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

      NodeBase() = default;
      NodeBase(NodeBase * p, NodeBase * l, NodeBase * r):
        parent(p),
        left(l),
        right(r)
      {}
    };

    template< class Key, class Value >
    struct Node: NodeBase {
      std::pair< const Key, Value > key_value_;

      Node(const Key & k, const Value & v, NodeBase * p, NodeBase * l, NodeBase * r):
        NodeBase(p, l, r),
        key_value_(k, v)
      {}
    };
  }

  template< class Key, class Value, class Compare >
  class BSTree {
  public:
    BSTree();
    BSTree(const BSTree & bst);
    BSTree(BSTree && bst);
    ~BSTree() noexcept;

    void push(Key k, Value v);
    Value get(Key k);
    void remove(Key k);

    void swap(BSTree & other) noexcept;

  private:
    detail::NodeBase * fake_root_;
    detail::NodeBase * fake_leaf_;
    Compare cmp_;

    void createFakes();
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
      fake_root_ = new detail::NodeBase(nullptr, fake_leaf_, fake_leaf_);
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
    detail::NodeBase * dst_root = new detail::Node< Key, Value >(root_kv.first, root_kv.second, temp.fake_root_, temp.fake_leaf_, temp.fake_leaf_);
    temp.fake_root_->left = dst_root;
    temp.fake_root_->right = dst_root;
    stack.push({src_root, dst_root});

    while (!stack.empty()) {
      detail::NodeBase * src = stack.top().first;
      detail::NodeBase * dst = stack.top().second;
      stack.pop();

      if (src->left != other.fake_leaf_) {
        const std::pair< const Key, Value > & l_kv = static_cast< detail::Node< Key, Value > * >(src->left)->key_value_;
        detail::NodeBase * new_left = new detail::Node< Key, Value >(l_kv.first, l_kv.second, dst, temp.fake_leaf_, temp.fake_leaf_);
        dst->left = new_left;
        new_left->parent = dst;
        stack.push({src->left, new_left});
      } else {
        dst->left = temp.fake_leaf_;
      }

      if (src->right != other.fake_leaf_) {
        const std::pair< const Key, Value > & r_kv = static_cast< detail::Node< Key, Value > * >(src->right)->key_value_;
        detail::NodeBase * new_right = new detail::Node< Key, Value >(r_kv.first, r_kv.second, dst, temp.fake_leaf_, temp.fake_leaf_);
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
  void BSTree< Key, Value, Compare >::swap(BSTree & other) noexcept
  {
    std::swap(fake_root_, other.fake_root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(cmp_, other.cmp_);
  }
}

#endif
