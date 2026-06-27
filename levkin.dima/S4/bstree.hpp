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
   }
}
#endif
