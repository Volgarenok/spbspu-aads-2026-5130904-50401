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
    BSTConstIterator(Node< Key, Value >* n, Node< Key, Value >* nil_ptr):
      node(n),
      nil(nil_ptr)
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
    BSTIterator(Node< Key, Value >* n, Node< Key, Value >* nil_ptr):
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
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  private:
    Node< Key, Value >* header;
    Node< Key, Value >* nil;
    Compare comp;
    void init()
    {
      nil = new Node< Key, Value >{Key(), Value(), nullptr, nullptr, nullptr};
      header = new Node< Key, Value >{Key(), Value(), nil, nil, nil};
      nil->left = nil->right = nil->parent = nil;
    }
    void clear(Node< Key, Value >* node)
    {
      if (node != nil) {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }
    Node< Key, Value >* copyTree(Node< Key, Value >* otherNode,
                                 Node< Key, Value >* parent,
                                 Node< Key, Value >* otherNil)
    {
      if (otherNode == otherNil)
        return nil;
      Node< Key, Value >* newNode = new Node< Key, Value >{
          otherNode->key, otherNode->value, nil, nil, parent};
      newNode->left = copyTree(otherNode->left, newNode, otherNil);
      newNode->right = copyTree(otherNode->right, newNode, otherNil);
      return newNode;
    }
    void transplant(Node< Key, Value >* u, Node< Key, Value >* v)
    {
      if (u->parent == header)
        header->left = v;
      else if (u == u->parent->left)
        u->parent->left = v;
      else
        u->parent->right = v;
      if (v != nil)
        v->parent = u->parent;
    }
    size_t heightInternal(Node< Key, Value >* n) const
    {
      if (n == nil)
        return 0;
      size_t l = heightInternal(n->left);
      size_t r = heightInternal(n->right);
      return 1 + (l > r ? l : r);
    }

  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    BSTree() { init(); }
    ~BSTree()
    {
      if (header) {
        clear(header->left);
        delete header;
      }
      if (nil)
        delete nil;
    }
    BSTree(const BSTree& other):
      comp(other.comp)
    {
      init();
      if (other.header->left != other.nil) {
        header->left = copyTree(other.header->left, header, other.nil);
      }
    }
    BSTree(BSTree&& other) noexcept:
      header(other.header),
      nil(other.nil),
      comp(std::move(other.comp))
    {
      other.header = nullptr;
      other.nil = nullptr;
    }
    BSTree& operator=(BSTree other)
    {
      std::swap(comp, other.comp);
      std::swap(header, other.header);
      std::swap(nil, other.nil);
      return *this;
    }
    const_iterator cbegin() const
    {
      Node< Key, Value >* curr = header->left;
      if (curr == nil)
        return const_iterator(header, nil);
      while (curr->left != nil)
        curr = curr->left;
      return const_iterator(curr, nil);
    }
    const_iterator cend() const { return const_iterator(header, nil); }
    bool has(Key k) const
    {
      Node< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key))
          z = z->left;
        else if (comp(z->key, k))
          z = z->right;
        else
          return true;
      }
      return false;
    }
    void push(Key k, Value v)
    {
      Node< Key, Value >* z = new Node< Key, Value >{k, v, nil, nil, nil};
      Node< Key, Value >* y = header;
      Node< Key, Value >* x = header->left;
      while (x != nil) {
        y = x;
        if (comp(k, x->key))
          x = x->left;
        else if (comp(x->key, k))
          x = x->right;
        else {
          x->value = v;
          delete z;
          return;
        }
      }
      z->parent = y;
      if (y == header)
        header->left = z;
      else if (comp(z->key, y->key))
        y->left = z;
      else
        y->right = z;
    }
    Value get(Key k) const
    {
      Node< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key))
          z = z->left;
        else if (comp(z->key, k))
          z = z->right;
        else
          return z->value;
      }
      throw std::out_of_range("Key not found");
    }
    Value drop(Key k)
    {
      Node< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key))
          z = z->left;
        else if (comp(z->key, k))
          z = z->right;
        else
          break;
      }
      if (z == nil)
        throw std::out_of_range("Key not found");
      Value val = z->value;
      if (z->left == nil)
        transplant(z, z->right);
      else if (z->right == nil)
        transplant(z, z->left);
      else {
        Node< Key, Value >* y = z->right;
        while (y->left != nil)
          y = y->left;
        if (y->parent != z) {
          transplant(y, y->right);
          y->right = z->right;
          y->right->parent = y;
        }
        transplant(z, y);
        y->left = z->left;
        y->left->parent = y;
      }
      delete z;
      return val;
    }
 
    size_t height(const_iterator it) const { return heightInternal(it.node); }
    size_t height() const { return heightInternal(header->left); }
  };
}
#endif
