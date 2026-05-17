#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>

namespace studilova
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    public:
      BSTree();
      explicit BSTree(const Compare& cmp);
      ~BSTree();

      bool empty() const;

    private:
      struct Node
      {
        Key key_;
        Value value_;
        Node* left_;
        Node* right_;
        Node* parent_;

        Node(const Key& key, const Value& value);
      };

      Node* root_;
      Compare cmp_;

      void clear(Node* node);
  };
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::Node::Node(const Key& key, const Value& value) :
  key_(key),
  value_(value),
  left_(nullptr),
  right_(nullptr),
  parent_(nullptr)
{}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree() :
  root_(nullptr),
  cmp_(Compare())
{}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree(const Compare& cmp) :
  root_(nullptr),
  cmp_(cmp)
{}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::~BSTree()
{
  clear(root_);
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::clear(Node* node)
{
  if (!node)
  {
    return;
  }
  clear(node->left_);
  clear(node->right_);
  delete node;
}

#endif
