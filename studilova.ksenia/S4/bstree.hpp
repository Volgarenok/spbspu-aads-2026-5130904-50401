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

      BSTree(const BSTree& other);
      BSTree(BSTree&& other) noexcept;
      BSTree& operator=(const BSTree& other);
      BSTree& operator=(BSTree&& other) noexcept;

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
      Node* clone(Node* node, Node* parent);
      void swap(BSTree& other) noexcept;
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
studilova::BSTree< Key, Value, Compare >::BSTree(const BSTree& other) :
  root_(nullptr),
  cmp_(other.cmp_)
{
  root_ = clone(other.root_, nullptr);
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept :
  root_(other.root_),
  cmp_(other.cmp_)
{
  other.root_ = nullptr;
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >& studilova::BSTree< Key, Value, Compare >::operator=(const BSTree& other)
{
  if (this != &other)
  {
    BSTree temp(other);
    swap(temp);
  }
  return *this;
}

template< class Key, class Value, class Compare >
studilova::BSTree< Key, Value, Compare >& studilova::BSTree< Key, Value, Compare >::operator=(BSTree&& other) noexcept
{
  if (this != other)
  {
    clear(root_);
    root_ = other.root_;
    cmp_ = other.cmp_;
    other.root_ = nullptr;
  }
  return *this;
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

template< class Key, class Value, class Compare >
typename studilova::BSTree< Key, Value, Compare >::Node* studilova::BSTree< Key, Value, Compare >::clone(Node* node, Node* parent)
{
  if (!node)
  {
    return nullptr;
  }

  Node* copy = new Node(node->key_, node->value_);
  copy->parent_ = parent;
  copy->left_ = clone(node->left_, copy);
  copy->right_ = clone(node->right_, copy);

  return copy;
}

template< class Key, class Value, class Compare >
void studilova::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(root_, other.root_);
  std::swap(cmp_, other.cmp_);
}

#endif
