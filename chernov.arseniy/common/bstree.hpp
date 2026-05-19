#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <type_traits>
#include <utility>

namespace chernov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      using value_type = std::pair< const Key, Value >;
      value_type key_value_;
      Node * left;
      Node * parent;
      Node * right;
    };
  }

  template< class Key, class Value, class Compare >
  class BSTree {
  public:
    void push(Key k, Value v);
    Value get(Key k);
    void remove(Key k);

  private:
    detail::Node< Key, Value > * fake_root_;
    detail::Node< Key, Value > * fake_leaf_;
    Compare cmp;
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

    detail::Node< Key, Value > * node_;
    explicit BSTIterator(detail::Node< Key, Value > * node);
  };
}

#endif
