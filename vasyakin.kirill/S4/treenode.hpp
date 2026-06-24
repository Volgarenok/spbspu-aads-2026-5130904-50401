#ifndef TREENODE_HPP
#define TREENODE_HPP
#include <utility>

namespace vasyakin
{
  template< class Key, class Value >
  struct Node
  {
    Node(const Key& k, const Value& v);

    std::pair< Key, Value > data_;
    Node* left_;
    Node* right_;
    Node* parent_;
  };

  template< class Key, class Value >
  Node< Key, Value >::Node(const Key& k, const Value& v):
    data_(k, v),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}
}

#endif
