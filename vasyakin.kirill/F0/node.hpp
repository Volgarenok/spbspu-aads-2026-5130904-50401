#ifndef NODE_HPP
#define NODE_HPP
#include <utility>

namespace vasyakin
{
  template< class K, class V >
  class AVLIterator;

  template< class K, class V >
  class AVLConstIterator;

  template< class K, class V, class C >
  class AVLTree;

  namespace detail
  {
    template< class Key, class Value >
    class Node
    {
    public:
      Node(const Key& key, const Value& value);
      Node(Key&& key, Value&& value);
    private:
      Key key_;
      Value value_;
      Node* left_;
      Node* right_;
      Node* parent_;
      int height_;

      template< class K, class V >
      friend class vasyakin::AVLIterator;

      template< class K, class V >
      friend class vasyakin::AVLConstIterator;

      template< class K, class V, class C >
      friend class vasyakin::AVLTree;
    };

    template< class Key, class Value >
    Node< Key, Value >::Node(const Key& key, const Value& value):
      key_(key),
      value_(value),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
    {}

    template< class Key, class Value >
    Node< Key, Value >::Node(Key&& key, Value&& value):
      key_(std::move(key)),
      value_(std::move(value)),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
    {}
  }
}

#endif
