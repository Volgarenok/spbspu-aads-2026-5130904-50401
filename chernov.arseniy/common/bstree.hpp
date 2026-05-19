#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <type_traits>
#include <utility>

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
    void push(Key k, Value v);
    Value get(Key k);
    void remove(Key k);

  private:
    detail::NodeBase * fake_root_;
    detail::NodeBase * fake_leaf_;
    Compare cmp_;
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
  BSTree< Key, Value, Compare >::BSTree():
    fake_root_(nullptr),
    fake_leaf_(nullptr),
    cmp_(Compare{})
  {
    fake_leaf_ = new detail::NodeBase();
    try {
      fake_root_ = new detail::NodeBase(nullptr, fake_leaf_, fake_leaf_);
    } catch (...) {
      delete fake_leaf_;
      throw;
    }
    fake_leaf_->parent = fake_root_;
  }
}

#endif
