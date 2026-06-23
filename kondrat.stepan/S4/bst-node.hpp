#ifndef BST_NODE_HPP
#define BST_NODE_HPP

#include <utility>

namespace kondrat
{
  namespace detail
  {
    template< class Key, class Value >
    struct Node
    {
      Node(const Key & key, const Value & value);
      Node(const Key & key, Value && value);

      std::pair< Key, Value > data_;
      Node * right_;
      Node * left_;
      Node * parent_;
    };
  }
}

template< class Key, class Value >
kondrat::detail::Node< Key, Value >::Node(const Key & key, const Value & value):
  data_(key, value),
  right_(nullptr),
  left_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value >
kondrat::detail::Node< Key, Value >::Node(const Key & key, Value && value):
  data_(key, std::move(value)),
  right_(nullptr),
  left_(nullptr),
  parent_(nullptr)
{}

#endif
