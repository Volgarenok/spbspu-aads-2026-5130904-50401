#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <utility>
#include <stdexcept>
#include <cstddef>

namespace madieva {
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class TIter;

  template< class Key, class Value, class Compare >
  class TCIter;

  template < class Key, class Value, class Compare >
  class Node {
    Node< Key, Value, Compare > * left;
    Node< Key, Value, Compare > * right;
    Node< Key, Value, Compare > * parent;
    std::pair< Key, Value > data;
    friend class BSTree< Key, Value, Compare >;
    friend class TIter< Key, Value, Compare >;
    friend class TCIter< Key, Value, Compare >;
  public:
    Node(const Key & k, const Value & v);
    Node(Key && k, Value && v);
    ~Node() = default;
  };

  template < class Key, class Value, class Compare >
  class BSTree {
    Node< Key, Value, Compare> * root_;
    Compare cmp_;
    Node< Key, Value, Compare > *
      minimum(Node< Key, Value, Compare > * node);
    const Node< Key, Value, Compare > *
      minimum(const Node< Key, Value, Compare > * node) const;
    Node< Key, Value, Compare > *
      maximum(Node< Key, Value, Compare > * node);
    Node< Key, Value, Compare > * cloneNode(const Node< Key, Value, Compare > * src,
      Node< Key, Value, Compare > * parent);
    void clear(Node< Key, Value, Compare > * node);
    friend class TIter< Key, Value, Compare >;
    friend class TCIter< Key, Value, Compare >;
  public:
    BSTree();
    BSTree(const BSTree& other);
    BSTree& operator=(const BSTree& other);
    void push(const Key & k, const Value & v);
    const Value & get(const Key & k) const;
    Value & get(const Key & k);
    void drop(const Key & k);
    bool has(const Key & k) const;
    TIter< Key, Value, Compare > find(const Key & k);
    TCIter< Key, Value, Compare > find(const Key & k) const;
    size_t height() const;
    size_t height(const Node< Key, Value, Compare > * node) const;
    size_t height(TIter< Key, Value, Compare > it) const;
    size_t height(TCIter< Key, Value, Compare > it) const;
    ~BSTree();

    TIter< Key, Value, Compare > begin();
    TIter< Key, Value, Compare > end();
    TCIter< Key, Value, Compare > begin() const;
    TCIter< Key, Value, Compare > end() const;

    TIter< Key, Value, Compare > rotateLeft(TIter< Key, Value, Compare > it);
    TIter< Key, Value, Compare > rotateRight(TIter< Key, Value, Compare > it);
    TIter< Key, Value, Compare > rotateLargeLeft(TIter< Key, Value, Compare > it);
    TIter< Key, Value, Compare > rotateLargeRight(TIter< Key, Value, Compare > it);


  };

  template < class Key, class Value, class Compare>
  Node< Key, Value, Compare >::Node(const Key & k,const Value & v) :
    left(nullptr),
    right(nullptr),
    parent(nullptr),
    data(k, v)
  {}

  template < class Key, class Value, class Compare>
  Node< Key, Value, Compare >::Node(Key && k, Value && v) :
    left(nullptr),
    right(nullptr),
    parent(nullptr),
    data(std::move(k), std::move(v))
  {}

  template < class Key, class Value, class Compare>
  Node< Key, Value, Compare > * BSTree< Key, Value, Compare >::minimum(Node< Key, Value, Compare > * node)
  {
    if (!node) {
      return nullptr;
    }
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  template < class Key, class Value, class Compare>
  const Node< Key, Value, Compare > *
    BSTree< Key, Value, Compare >::minimum(const Node< Key, Value, Compare > * node) const
  {
    if (!node) {
      return nullptr;
    }
    while (node->left) {
      node = node->left;
    }
    return node;
  }

  template < class Key, class Value, class Compare >
  Node< Key, Value, Compare > * BSTree< Key, Value, Compare >::maximum(Node< Key, Value, Compare >* node) {
    if (!node) {
      return nullptr;
    }
    while (node->right) {
      node = node->right;
    }
    return node;
  }

  template < class Key, class Value, class Compare>
  Node< Key, Value, Compare > *
    BSTree< Key, Value, Compare >::cloneNode(const Node< Key, Value, Compare > * src, Node< Key, Value, Compare > * parent)
  {
    if (!src) return nullptr;
    Node< Key, Value, Compare > * n = new Node< Key, Value, Compare >(src->data.first, src->data.second);
    n->parent = parent;
    n->left  = cloneNode(src->left,  n);
    n->right = cloneNode(src->right, n);
    return n;
  }

  template< typename Key, typename Value, typename Compare >
  void BSTree< Key, Value, Compare >::clear(Node< Key, Value, Compare > * node)
  {
    if (!node) {
      return;
    }
    clear(node->left);
    clear(node->right);
    delete node;
  }

  template< typename Key, typename Value, typename Compare >
  BSTree< Key, Value, Compare >::BSTree() :
    root_(nullptr),
    cmp_()
  {}

  template < class Key, class Value, class Compare>
  BSTree< Key, Value, Compare >::BSTree(const BSTree& other) :
    root_(nullptr), cmp_(other.cmp_)
  {
    root_ = cloneNode(other.root_, nullptr);
  }

  template < class Key, class Value, class Compare>
  BSTree< Key, Value, Compare >&
    BSTree< Key, Value, Compare >::operator=(const BSTree& other)
  {
    if (this != &other) {
      clear(root_);
      root_ = nullptr;
      cmp_ = other.cmp_;
      root_ = cloneNode(other.root_, nullptr);
    }
    return *this;
  }

  template < class Key, class Value, class Compare>
  void BSTree< Key, Value, Compare >::push(const Key & k, const Value & v)
  {
    Node< Key, Value, Compare > * n = nullptr;
    try {
      n = new Node< Key, Value, Compare >(k, v);
      if (!root_) {
        root_ = n;
        return;
      }
      Node< Key, Value, Compare > * curr = root_;
      while (curr) {
        if (cmp_(n->data.first, curr->data.first)) {
          if (!curr->left) {
            curr->left = n;
            n->parent = curr;
            return;
          }
          curr = curr->left;
        } else if (cmp_(curr->data.first, n->data.first)) {
          if (!curr->right) {
            curr->right = n;
            n->parent = curr;
            return;
          }
          curr = curr->right;
        } else {
          delete n;
          return;
        }
      }
    } catch (...) {
      delete n;
      throw;
    }
  }

  template < class Key, class Value, class Compare>
  const Value & BSTree< Key, Value, Compare >::get(const Key & k) const
  {
    const Node< Key, Value, Compare > * curr = root_;
    while(curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return curr->data.second;
      }
    }
    throw std::out_of_range("Key not found");
  }

  template < class Key, class Value, class Compare>
  Value & BSTree< Key, Value, Compare >::get(const Key & k)
  {
    Node< Key, Value, Compare > * curr = root_;
    while(curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return curr->data.second;
      }
    }
    throw std::out_of_range("Key not found");
  }

  template < class Key, class Value, class Compare>
  bool BSTree< Key, Value, Compare >::has(const Key & k) const
  {
    const Node< Key, Value, Compare > * curr = root_;
    while(curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return true;
      }
    }
    return false;
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::find(const Key & k)
  {
    Node< Key, Value, Compare > * curr = root_;
    while (curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return TIter< Key, Value, Compare >(curr);
      }
    }
    return end();
  }

  template < class Key, class Value, class Compare>
  TCIter< Key, Value, Compare > BSTree< Key, Value, Compare >::find(const Key & k) const
  {
    const Node< Key, Value, Compare > * curr = root_;
    while (curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        return TCIter< Key, Value, Compare >(curr);
      }
    }
    return end();
  }

  template < class Key, class Value, class Compare>
  void BSTree< Key, Value, Compare >::drop(const Key & k)
  {
    Node< Key, Value, Compare > * curr = root_;
    while(curr) {
      if (cmp_(k, curr->data.first)) {
        curr = curr->left;
      } else if (cmp_(curr->data.first, k)) {
        curr = curr->right;
      } else {
        if (!curr->left && !curr->right) {
          if (curr->parent) {
            if (curr->parent->left == curr) {
              curr->parent->left = nullptr;
            } else {
              curr->parent->right = nullptr;
            }
          } else {
            root_ = nullptr;
          }
          delete curr;
          return;
        }
        if (!curr->left || !curr->right) {
          Node< Key, Value, Compare > * child = curr->left ? curr->left : curr->right;
          child->parent = curr->parent;
          if (curr->parent) {
            if (curr->parent->left == curr) {
              curr->parent->left = child;
            } else {
              curr->parent->right = child;
            }
          } else {
            root_ = child;
          }
          delete curr;
          return;
        }
        Node< Key, Value, Compare > * succ = minimum(curr->right);
        curr->data = std::move(succ->data);
        Node< Key, Value, Compare > * succ_child = succ->right;
        if (succ->parent != curr) {
          succ->parent->left = succ_child;
        } else {
          curr->right = succ_child;
        }
        if (succ_child) {
          succ_child->parent = succ->parent;
        }
        delete succ;
        return;
      }
    }
    throw std::out_of_range("Key not found");
  }

  template < class Key, class Value, class Compare>
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return height(root_);
  }

  template < class Key, class Value, class Compare>
  size_t BSTree< Key, Value, Compare >::height(const Node< Key, Value, Compare > * node) const
  {
    if (!node) return 0;
    size_t leftH = height(node->left);
    size_t rightH = height(node->right);
    return 1 + (leftH > rightH ? leftH : rightH);
  }

  template < class Key, class Value, class Compare>
  size_t BSTree< Key, Value, Compare >::height(TIter< Key, Value, Compare > it) const
  {
    return height(it.it_);
  }

  template < class Key, class Value, class Compare>
  size_t BSTree< Key, Value, Compare >::height(TCIter< Key, Value, Compare > it) const
  {
    return height(it.it_);
  }

  template < class Key, class Value, class Compare>
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear(root_);
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::begin()
  {
    Node< Key, Value, Compare > * node = root_;
    while (node && node->left) {
      node = node->left;
    }
    return TIter< Key, Value, Compare >(node);
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::end()
  {
    return TIter< Key, Value, Compare >(nullptr);
  }

  template < class Key, class Value, class Compare>
  TCIter< Key, Value, Compare > BSTree< Key, Value, Compare >::begin() const
  {
    const Node< Key, Value, Compare > * node = root_;
    while (node && node->left) {
      node = node->left;
    }
    return TCIter< Key, Value, Compare >(node);
  }

  template < class Key, class Value, class Compare>
  TCIter< Key, Value, Compare > BSTree< Key, Value, Compare >::end() const
  {
    return TCIter< Key, Value, Compare >(nullptr);
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::rotateLeft(TIter< Key, Value, Compare > it)
  {
    if (!it.it_ || !it.it_->right) {
      throw std::logic_error("Cannot rotate left");
    }
    TIter< Key, Value, Compare > it2(it.it_->right);
    TIter< Key, Value, Compare > parent(it.it_->parent);
    if (parent.it_) {
      if (parent.it_->left == it.it_) {
        parent.it_->left = it2.it_;
      } else {
        parent.it_->right = it2.it_;
      }
    } else {
      root_ = it2.it_;
    }
    it2.it_->parent = parent.it_;
    it.it_->parent = it2.it_;
    if (it2.it_->left) {
      it.it_->right = it2.it_->left;
      it.it_->right->parent = it.it_;
    } else {
      it.it_->right = nullptr;
    }
    it2.it_->left = it.it_;
    return it2;
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::rotateRight(TIter< Key, Value, Compare > it)
  {
    if (!it.it_ || !it.it_->left) {
      throw std::logic_error("Cannot rotate right");
    }
    TIter< Key, Value, Compare > it2(it.it_->left);
    TIter< Key, Value, Compare > parent(it.it_->parent);
    if (parent.it_) {
      if (parent.it_->left == it.it_) {
        parent.it_->left = it2.it_;
      } else {
        parent.it_->right = it2.it_;
      }
    } else {
      root_ = it2.it_;
    }
    it2.it_->parent = parent.it_;
    it.it_->parent = it2.it_;
    if (it2.it_->right) {
      it.it_->left = it2.it_->right;
      it.it_->left->parent = it.it_;
    } else {
      it.it_->left = nullptr;
    }
    it2.it_->right = it.it_;
    return it2;
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::rotateLargeLeft(TIter< Key, Value, Compare > it)
  {
    if (!it.it_ || !it.it_->right || !it.it_->right->left) {
      throw std::logic_error("Cannot rotate large left");
    }
    TIter< Key, Value, Compare > it2(it.it_->right);
    rotateRight(it2);
    return rotateLeft(it);
  }

  template < class Key, class Value, class Compare>
  TIter< Key, Value, Compare > BSTree< Key, Value, Compare >::rotateLargeRight(TIter< Key, Value, Compare > it)
  {
    if (!it.it_ || !it.it_->left || !it.it_->left->right) {
      throw std::logic_error("Cannot rotate large right");
    }
    TIter< Key, Value, Compare > it2(it.it_->left);
    rotateLeft(it2);
    return rotateRight(it);
  }

}

#include "tree_iterator.hpp"
#include "tree_citerator.hpp"


#endif
