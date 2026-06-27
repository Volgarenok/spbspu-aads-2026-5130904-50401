#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <stdexcept>
#include <functional>
#include <utility>
namespace levkin {
  template < class Key, class Value >
  struct BSTNode
  {
    Key key;
    Value value;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
  };
  template < class Key, class Value >
  class BSTConstIterator
  {
  public:
    BSTNode< Key, Value >* node;
    BSTNode< Key, Value >* nil;
    BSTNode< Key, Value >* header;
    BSTConstIterator(BSTNode< Key, Value >* n,
                     BSTNode< Key, Value >* nil_ptr,
                     BSTNode< Key, Value >* header_ptr):
      node(n),
      nil(nil_ptr),
      header(header_ptr)
    {
    }
    const BSTNode< Key, Value >* operator->() const { return node; }
    const BSTNode< Key, Value >& operator*() const { return *node; }
    BSTConstIterator& operator++()
    {
      if (node == header) {
        return *this;
      }
      if (node->right != nil) {
        node = node->right;
        while (node->left != nil) {
          node = node->left;
        }
      } else {
        BSTNode< Key, Value >* p = node->parent;
        while (p != header && node == p->right) {
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
    BSTIterator(BSTNode< Key, Value >* n,
                BSTNode< Key, Value >* nil_ptr,
                BSTNode< Key, Value >* header_ptr):
      BSTConstIterator< Key, Value >(n, nil_ptr, header_ptr)
    {
    }
    BSTNode< Key, Value >* operator->() { return this->node; }
    BSTNode< Key, Value >& operator*() { return *(this->node); }
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
    BSTNode< Key, Value >* header;
    BSTNode< Key, Value >* nil;
    Compare comp;
    void init()
    {
      nil = new BSTNode< Key, Value >{
          Key(), Value(), nullptr, nullptr, nullptr};
      nil->left = nil->right = nil->parent = nil;
      header = new BSTNode< Key, Value >{Key(), Value(), nil, nil, nil};
    }
    void clear(BSTNode< Key, Value >* node)
    {
      if (node != nil) {
        clear(node->left);
        clear(node->right);
        delete node;
      }
    }
    BSTNode< Key, Value >* copyTree(BSTNode< Key, Value >* otherNode,
                                    BSTNode< Key, Value >* parent,
                                    BSTNode< Key, Value >* otherNil)
    {
      if (otherNode == otherNil) {
        return nil;
      }
      BSTNode< Key, Value >* newNode = new BSTNode< Key, Value >{
          otherNode->key, otherNode->value, nil, nil, parent};
      newNode->left = copyTree(otherNode->left, newNode, otherNil);
      newNode->right = copyTree(otherNode->right, newNode, otherNil);
      return newNode;
    }
    void transplant(BSTNode< Key, Value >* u, BSTNode< Key, Value >* v)
    {
      if (u->parent == header) {
        header->left = v;
      } else if (u == u->parent->left) {
        u->parent->left = v;
      } else {
        u->parent->right = v;
      }
      if (v != nil) {
        v->parent = u->parent;
      }
    }
    size_t heightInternal(BSTNode< Key, Value >* n) const
    {
      if (n == nil) {
        return 0;
      }
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
      if (nil) {
        delete nil;
      }
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
      other.init();
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
      BSTNode< Key, Value >* curr = header->left;
      if (curr == nil) {
        return const_iterator(header, nil, header);
      }
      while (curr->left != nil) {
        curr = curr->left;
      }
      return const_iterator(curr, nil, header);
    }
    const_iterator cend() const { return const_iterator(header, nil, header); }
    bool has(Key k) const
    {
      BSTNode< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key)) {
          z = z->left;
        } else if (comp(z->key, k)) {
          z = z->right;
        } else {
          return true;
        }
      }
      return false;
    }
    void push(Key k, Value v)
    {
      BSTNode< Key, Value >* z = new BSTNode< Key, Value >{k, v, nil, nil, nil};
      BSTNode< Key, Value >* y = header;
      BSTNode< Key, Value >* x = header->left;
      while (x != nil) {
        y = x;
        if (comp(k, x->key)) {
          x = x->left;
        } else if (comp(x->key, k)) {
          x = x->right;
        } else {
          x->value = v;
          delete z;
          return;
        }
      }
      z->parent = y;
      if (y == header) {
        header->left = z;
      } else if (comp(z->key, y->key)) {
        y->left = z;
      } else {
        y->right = z;
      }
    }
    Value get(Key k) const
    {
      BSTNode< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key)) {
          z = z->left;
        } else if (comp(z->key, k)) {
          z = z->right;
        } else {
          return z->value;
        }
      }
      throw std::out_of_range("Key not found");
    }
    Value drop(Key k)
    {
      BSTNode< Key, Value >* z = header->left;
      while (z != nil) {
        if (comp(k, z->key)) {
          z = z->left;
        } else if (comp(z->key, k)) {
          z = z->right;
        } else {
          break;
        }
      }
      if (z == nil) {
        throw std::out_of_range("Key not found");
      }
      Value val = z->value;
      if (z->left == nil) {
        transplant(z, z->right);
      } else if (z->right == nil) {
        transplant(z, z->left);
      } else {
        BSTNode< Key, Value >* y = z->right;
        while (y->left != nil) {
          y = y->left;
        }
        if (y->parent != z) {
          transplant(y, y->right);
          y->right = z->right;
          if (y->right != nil) {
            y->right->parent = y;
          }
        }
        transplant(z, y);
        y->left = z->left;
        if (y->left != nil) {
          y->left->parent = y;
        }
      }
      delete z;
      return val;
    }
    const_iterator rotateLeft(const_iterator it)
    {
      BSTNode< Key, Value >* x = it.node;
      if (x == nil || x == header) {
        throw std::invalid_argument("Invalid node for rotation");
      }
      BSTNode< Key, Value >* p = x->parent;
      if (p == header || p->right != x) {
        throw std::invalid_argument("Invalid left rotation context");
      }
      p->right = x->left;
      if (x->left != nil) {
        x->left->parent = p;
      }
      x->parent = p->parent;
      if (p->parent == header) {
        header->left = x;
      } else if (p == p->parent->left) {
        p->parent->left = x;
      } else {
        p->parent->right = x;
      }
      x->left = p;
      p->parent = x;
      return const_iterator(x, nil, header);
    }
    const_iterator rotateRight(const_iterator it)
    {
      BSTNode< Key, Value >* x = it.node;
      if (x == nil || x == header) {
        throw std::invalid_argument("Invalid node for rotation");
      }
      BSTNode< Key, Value >* p = x->parent;
      if (p == header || p->left != x) {
        throw std::invalid_argument("Invalid right rotation context");
      }
      p->left = x->right;
      if (x->right != nil) {
        x->right->parent = p;
      }
      x->parent = p->parent;
      if (p->parent == header) {
        header->left = x;
      } else if (p == p->parent->right) {
        p->parent->right = x;
      } else {
        p->parent->left = x;
      }
      x->right = p;
      p->parent = x;
      return const_iterator(x, nil, header);
    }
    const_iterator rotateLargeLeft(const_iterator it)
    {
      rotateRight(it);
      return rotateLeft(it);
    }
    const_iterator rotateLargeRight(const_iterator it)
    {
      rotateLeft(it);
      return rotateRight(it);
    }
    size_t height(const_iterator it) const { return heightInternal(it.node); }
    size_t height() const { return heightInternal(header->left); }
  };
}
#endif
