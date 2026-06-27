#ifndef BSTREE_HPP
#define BSTREE_HPP 
#include <iostream>
#include <stdexcept>
#include <functional>
namespace levkin {
  template < class Key, class Value >
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;
  };
  template < class Key, class Value >
  class BSTConstIterator
  {
  public:
    Node< Key, Value >* node;
    Node< Key, Value >* nil;
    BSTConstIterator(Node< Key, Value >* n, Node< Key, Value >* nil_ptr) :
      node(n), nil(nil_ptr)
    {
    }
    const Node< Key, Value >* operator->() const { return node; }
    const Node< Key, Value >& operator*() const { return *node; }
    BSTConstIterator& operator++()
    {
      if (node->right != nil) {
        node = node->right;
        while (node->left != nil)
          node = node->left;
      } else {
        Node< Key, Value >* p = node->parent;
        while (p != nullptr && p->left != nil && node == p->right) {
          node = p;
          p = p->parent;
        }
        node = p;
      }
      return *this;
    }
    bool operator!=(const BSTConstIterator& other) const
    {
      return node != other.node;
    }
    bool operator==(const BSTConstIterator& other) const
    {
      return node == other.node;
    }
  };
  template < class Key, class Value >
  class BSTIterator : public BSTConstIterator< Key, Value >
  {
  public:
    BSTIterator(Node< Key, Value >* n, Node< Key, Value >* nil_ptr) :
      BSTConstIterator< Key, Value >(n, nil_ptr)
    {
    }
    Node< Key, Value >* operator->() { return this->node; }
    Node< Key, Value >& operator*() { return *(this->node); }
    BSTIterator& operator++()
    {
      BSTConstIterator< Key, Value >::operator++();
      return *this;
    }
  };
  
#endif
