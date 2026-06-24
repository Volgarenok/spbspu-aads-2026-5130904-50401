#ifndef TREENODE_HPP
#define TREENODE_HPP

namespace vasyakin
{

  template < class K, class V, class C >
  class BSTree;

  template < class K, class V >
  class BSTIterator;

  template < class K, class V >
  class BSTConstIterator;

  template< class Key, class Value >
  class Node
  {
  public:
    Node(const Key& k, const Value& v);
  private:
    Key key_;
    Value value_;
    Node* left_;
    Node* right_;
    Node* parent_;

    template< class K, class V, class C >
    friend class BSTree;

    template< class K, class V >
    friend class BSTIterator;

    template< class K, class V >
    friend class BSTConstIterator;
  };

  template< class Key, class Value >
  Node< Key, Value >::Node(const Key& k, const Value& v):
    key_(k),
    value_(v),
    left_(nullptr),
    right_(nullptr),
    parent_(nullptr)
  {}
}

#endif
