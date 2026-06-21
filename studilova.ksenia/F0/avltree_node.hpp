#ifndef AVLTREE_NODE
#define AVLTREE_NODE

#include <cstddef>

namespace studilova
{
  template< class K, class V, class C >
  class AVLTree;

  template< class K, class V >
  class AVLTreeIt;

  template< class K, class V >
  class AVLTreeCIt;

  namespace detail
  {
    template< class Key, class Value >
    class AVLNode
    {
      public:
      AVLNode(const Key& key, const Value& value);

      private:
        Key key_;
        Value value_;
        AVLNode* left_;
        AVLNode* right_;
        AVLNode* parent_;
        size_t height_;
    };
  }
}

template< class Key, class Value >
studilova::detail::AVLNode< Key, Value >::AVLNode(const Key& key, const Value& value):
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr),
  height_(1)
{}

#endif
