#ifndef TNODE_HPP
#define TNODE_HPP

namespace detail
{
  template < class Key, class Value >
  struct TNode
  {
    TNode();
    TNode(Key k, Value v);

    Key key_;
    Value value_;
    TNode *left_;
    TNode *right_;
    TNode *parent_;
  };

  template < class Key, class Value >
  TNode< Key, Value >::TNode():
    key_(),
    value_(),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}

  template < class Key, class Value >
  TNode< Key, Value >::TNode(Key k, Value v):
    key_(k),
    value_(v),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}
}

#endif
