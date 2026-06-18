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
      template< class K, class V >
      Node(K&& key, V&& value);
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
    template< class K, class V >
    Node< Key, Value >::Node(K&& key, V&& value):
      key_(std::forward< K >(key)),
      value_(std::forward< V >(value)),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
    {}
  }
}

#endif
