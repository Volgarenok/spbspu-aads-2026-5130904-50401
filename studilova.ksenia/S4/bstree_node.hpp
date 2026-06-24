#ifndef BSTREE_NODE
#define BSTREE_NODE

namespace studilova
{
  template< class Key, class Value >
  class Node
  {
    public:
      Node(const Key& key, const Value& value);

    private:
      Key key_;
      Value value_;
      Node* left_;
      Node* right_;
      Node* parent_;

      template< class K, class V, class C >
      friend class BSTree;

      template< class K, class V >
      friend class BSTreeIt;

      template< class K, class V >
      friend class BSTreeCIt;
  };
}

template< class Key, class Value >
studilova::Node< Key, Value >::Node(const Key& key, const Value& value) :
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

#endif
