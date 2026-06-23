#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <memory>
#include <utility>

namespace kuznetsov {
  namespace detail {
    template< class Key, class Value >
    struct Node {
      std::pair< const Key, Value > value;

      Node* parent;
      Node* lt;
      Node* rt;

      Node(const Key& k, const Value& v, Node* p);
    };

    template< class Key, class Value >
    Node< Key, Value >* copyTree(const Node< Key, Value >* oth, Node< Key, Value >* p = nullptr);

    template< class Key, class Value >
    Node< Key, Value >* minimum(Node< Key, Value >* root);

    template< class Key, class Value >
    Node< Key, Value >* maximum(Node< Key, Value >* root);

    template< class Key, class Value >
    void clearNodes(Node< Key, Value >* root);

  }

  template< class Key, class Value, bool IsConst >
  struct BSTIterator;

  template< class Key, class Value, class Compare >
  struct BSTree {
    using iterator = BSTIterator< Key, Value, false >;
    using const_iterator = BSTIterator< Key, Value, true >;

    BSTree();
    BSTree(const BSTree&);
    BSTree(BSTree&&) noexcept;
    ~BSTree() noexcept;

    BSTree& operator=(const BSTree&);
    BSTree& operator=(BSTree&&) noexcept;

    void push(const Key& k, const Value& v);
    void push(const Key& k, Value&& v);

    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    size_t drop(const Key& k);

    const_iterator root() const;
    const_iterator leftChild(const_iterator) const;
    const_iterator rightChild(const_iterator) const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const noexcept;
    size_t height() const noexcept;

    size_t size() const noexcept;
    bool empty() const noexcept;
    bool contain(const Key& k) const noexcept;

    void swap(BSTree& oth) noexcept;
    void clear() noexcept;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;

    iterator end();
    const_iterator end() const;
    const_iterator cend() const;


  private:
    Compare cmptr_;
    detail::Node< Key, Value >* root_;
    size_t size_;

    template< class UV >
    void pushImp(const Key& k, UV&& v);

    detail::Node< Key, Value >* find(const Key& key) const noexcept;
    size_t calcHeight(const detail::Node< Key, Value >*) const noexcept;
  };

  template< class Key, class Value, bool IsConst >
  struct BSTIterator {
    using type_t = std::pair< const Key, Value >;
    using reference = typename std::conditional< IsConst, const type_t&, type_t& >::type;
    using pointer = typename std::conditional< IsConst, const type_t*, type_t* >::type;

    BSTIterator& operator++();
    BSTIterator& operator--();

    BSTIterator operator++(int);
    BSTIterator operator--(int);

    reference operator*();
    pointer operator->();

    template< bool OthConst >
    bool operator==(const BSTIterator< Key, Value, OthConst >&);

    template< bool OthConst >
    bool operator!=(const BSTIterator< Key, Value, OthConst >&);

  private:
    template< class, class, class >
    friend struct BSTree;
    detail::Node< Key, Value >* curr_;
    BSTIterator(detail::Node< Key, Value >*);
  };
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::BSTIterator< K, V, C >::operator==(const BSTIterator< K, V, OthConst >& oth)
{
  return curr_ == oth.curr_;
}

template< class K, class V, bool C >
template< bool OthConst >
bool kuznetsov::BSTIterator< K, V, C >::operator!=(const BSTIterator< K, V, OthConst >& oth)
{
  return curr_ != oth.curr_;
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::BSTIterator< Key, Value, IsConst >::reference kuznetsov::BSTIterator< Key, Value, IsConst >::operator*()
{
  return curr_->value;
}

template< class Key, class Value, bool IsConst >
typename kuznetsov::BSTIterator< Key, Value, IsConst >::pointer kuznetsov::BSTIterator< Key, Value, IsConst >::operator->()
{
  return std::addressof(curr_->value);
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >& kuznetsov::BSTIterator< Key, Value, IsConst >::operator++()
{
  detail::Node< Key, Value >* next = curr_;
  if (next->rt) {
    next = next->rt;
    next = detail::minimum(next);
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && parent->lt != next) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst > kuznetsov::BSTIterator< Key, Value, IsConst >::operator++(int)
{
  auto it = BSTIterator< Key, Value, IsConst >(curr_);
  ++(*this);
  return it;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >& kuznetsov::BSTIterator< Key, Value, IsConst >::operator--()
{
  detail::Node< Key, Value >* next = curr_;
  if (next->lt) {
    next = next->lt;
    next = maximum(next);
  } else {
    detail::Node< Key, Value >* parent = next->parent;
    while (parent && parent->rt != next) {
      next = parent;
      parent = next->parent;
    }
    next = parent;
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst > kuznetsov::BSTIterator< Key, Value, IsConst >::operator--(int)
{
  auto it = BSTIterator< Key, Value, IsConst >(curr_);
  --(*this);
  return it;
}

template< class Key, class Value, bool IsConst >
kuznetsov::BSTIterator< Key, Value, IsConst >::BSTIterator(detail::Node< Key, Value >* n):
  curr_(n)
{}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >::Node(const Key& k, const Value& v, Node* p):
  value(std::make_pair(k, v)),
  parent(p),
  lt(nullptr),
  rt(nullptr)
{}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree():
  cmptr_(Compare{}),
  root_(nullptr),
  size_(0)
{}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree(const BSTree& oth):
  cmptr_(oth.cmptr_),
  root_(nullptr),
  size_(oth.size_)
{
  root_ = detail::copyTree(oth.root_);
}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::BSTree(BSTree&& oth) noexcept:
  cmptr_(oth.cmptr_),
  root_(std::exchange(oth.root_, nullptr)),
  size_(std::exchange(oth.size_, 0))
{}

template< class Key, class Value >
void kuznetsov::detail::clearNodes(Node< Key, Value >* node)
{
  if (!node) {
    return;
  }
  clearNodes(node->lt);
  clearNodes(node->rt);
  delete node;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::copyTree(const Node< Key, Value >* oth, Node< Key, Value >* p)
{
  if (!oth) {
    return nullptr;
  }
  Node< Key, Value >* n = new Node< Key, Value >(oth->value.first, oth->value.second, p);
  try {
    n->lt = copyTree(oth->lt, n);
    n->rt = copyTree(oth->rt, n);
  } catch (...) {
    detail::clearNodes(n->lt);
    delete n;
    throw;
  }
  return n;
}

template< class Key, class Value, class Compare >
kuznetsov::BSTree< Key, Value, Compare >::~BSTree() noexcept
{
  clear();
}

template< class K, class V, class C >
kuznetsov::BSTree< K, V, C >& kuznetsov::BSTree< K, V, C >::operator=(const BSTree& oth)
{
  if (this == std::addressof(oth)) {
    return *this;
  }
  BSTree< K, V, C > cp(oth);
  swap(cp);
  return *this;
}

template< class K, class V, class C >
kuznetsov::BSTree< K, V, C >& kuznetsov::BSTree< K, V, C >::operator=(BSTree&& oth) noexcept
{
  if (this == std::addressof(oth)) {
    return *this;
  }
  BSTree< K, V, C > cp(std::move(oth));
  swap(cp);
  return *this;
}

template< class Key, class Value, class Compare >
template< class UV >
void kuznetsov::BSTree< Key, Value, Compare >::pushImp(const Key& k, UV&& v)
{
  if (!root_) {
    root_ = new detail::Node< Key, Value >(k, std::forward< UV >(v), nullptr);
    ++size_;
    return;
  }

  detail::Node< Key, Value >* curr = find(k);
  detail::Node< Key, Value >* p;
  if (!curr) {
    curr = root_;
    while (curr) {
      p = curr;
      if (cmptr_(k, curr->value.first)) {
        curr = curr->lt;
      } else {
        curr = curr->rt;
      }
    }
    if (cmptr_(k, p->value.first)) {
      p->lt = new detail::Node< Key, Value >(k, std::forward< UV >(v), p);
    } else {
      p->rt = new detail::Node< Key, Value >(k, std::forward< UV >(v), p);
    }
    ++size_;
  } else {
    curr->value.second = std::forward< UV >(v);
  }
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  pushImp(k, v);
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::push(const Key& k, Value&& v)
{
  pushImp(k, std::forward< Value >(v));
}

template< class K, class V, class C >
const V& kuznetsov::BSTree< K, V, C >::at(const K& k) const
{
  detail::Node< K, V >* curr = find(k);
  if (!curr) {
    throw std::logic_error("No element with such case");
  }
  return curr->value.second;
}

template< class K, class V, class C >
V& kuznetsov::BSTree< K, V, C >::at(const K& k)
{
  const BSTree< K, V, C >* cthis = this;
  return const_cast< V& >(cthis->at(k));
}
template< class K, class V, class Cmp >
size_t kuznetsov::BSTree< K, V, Cmp >::drop(const K& key)
{
  detail::Node< K, V >* curr = find(key);
  if (!curr) {
    throw std::logic_error("Element with such key doesnt exist");
  }

  if (curr->lt && curr->rt) {
    detail::Node< K, V >* succ = detail::maximum(curr->lt);

    if (succ->parent != curr) {
      succ->parent->rt = succ->lt;
      if (succ->lt) {
        succ->lt->parent = succ->parent;
      }
      succ->lt = curr->lt;
      curr->lt->parent = succ;
    }
    succ->rt = curr->rt;
    curr->rt->parent = succ;
    succ->parent = curr->parent;
    if (curr->parent) {
      if (curr->parent->lt == curr) {
        curr->parent->lt = succ;
      } else {
        curr->parent->rt = succ;
      }
    } else {
      root_ = succ;
    }
  } else {
    detail::Node< K, V >* child = curr->lt ? curr->lt : curr->rt;
    if (curr->parent) {
      if (curr->parent->lt == curr) {
        curr->parent->lt = child;
      } else {
        curr->parent->rt = child;
      }
      if (child) {
        child->parent = curr->parent;
      }
    } else {
      root_ = child;
      if (child) {
        child->parent = nullptr;
      }
    }
  }
  delete curr;
  return --size_;
}

template< class K, class V, class Cmp >
kuznetsov::detail::Node< K, V >* kuznetsov::BSTree< K, V, Cmp >::find(const K& key) const noexcept
{
  detail::Node< K, V >* curr = root_;
  while (curr) {
    if (cmptr_(key, curr->value.first)) {
      curr = curr->lt;
    } else if (cmptr_(curr->value.first, key)) {
      curr = curr->rt;
    } else {
      return curr;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
bool kuznetsov::BSTree< Key, Value, Compare >::contain(const Key& k) const noexcept
{
  detail::Node< Key, Value >* curr = find(k);
  return curr != nullptr;
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::size() const noexcept
{
  return size_;
}

template< class Key, class Value, class Compare >
bool kuznetsov::BSTree< Key, Value, Compare >::empty() const noexcept
{
  return !size_;
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::height() const noexcept
{
  return calcHeight(root_);
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::height(const_iterator it) const noexcept
{
  return calcHeight(it.curr_);
}

template< class Key, class Value, class Compare >
size_t kuznetsov::BSTree< Key, Value, Compare >::calcHeight(const detail::Node< Key, Value >* n) const noexcept
{
  if (!n) {
    return 0;
  }
  return 1 + std::max(calcHeight(n->lt), calcHeight(n->rt));
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::swap(BSTree& oth) noexcept
{
  std::swap(root_, oth.root_);
  std::swap(size_, oth.size_);
}

template< class Key, class Value, class Compare >
void kuznetsov::BSTree< Key, Value, Compare >::clear() noexcept
{
  detail::clearNodes(root_);
  root_ = nullptr;
  size_ = 0;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::minimum(Node< Key, Value >* root)
{
  if (!root) {
    return nullptr;
  }
  auto curr = root;
  while (curr->lt) {
    curr = curr->lt;
  }
  return curr;
}

template< class Key, class Value >
kuznetsov::detail::Node< Key, Value >* kuznetsov::detail::maximum(Node< Key, Value >* root)
{
  if (!root) {
    return nullptr;
  }
  auto curr = root;
  while (curr->rt) {
    curr = curr->rt;
  }
  return curr;
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator kuznetsov::BSTree< K, V, Cmp >::rotateLeft(const_iterator it)
{
  detail::Node< K, V >* y = it.curr_;

  if (!y || !y->parent) {
    return const_iterator(y);
  }

  detail::Node< K, V >* x = y->parent;

  if (x->rt != y) {
    throw std::logic_error("Invalid left rotation");
  }

  x->rt = y->lt;
  if (y->lt) {
    y->lt->parent = x;
  }

  y->parent = x->parent;

  if (!x->parent) {
    root_ = y;
  } else if (x->parent->lt == x) {
    x->parent->lt = y;
  } else {
    x->parent->rt = y;
  }

  y->lt = x;
  x->parent = y;

  return const_iterator(y);
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator kuznetsov::BSTree< K, V, Cmp >::rotateRight(const_iterator it)
{
  detail::Node< K, V >* x = it.curr_;

  if (!x || !x->parent) {
    return const_iterator(x);
  }

  detail::Node< K, V >* y = x->parent;

  if (y->lt != x) {
    throw std::logic_error("Invalid right rotation");
  }

  y->lt = x->rt;
  if (x->rt) {
    x->rt->parent = y;
  }

  x->parent = y->parent;

  if (!y->parent) {
    root_ = x;
  } else if (y->parent->lt == y) {
    y->parent->lt = x;
  } else {
    y->parent->rt = x;
  }

  x->rt = y;
  y->parent = x;

  return const_iterator(x);
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator
  kuznetsov::BSTree< K, V, Cmp >::rotateLargeLeft(const_iterator it)
{
  detail::Node< K, V >* b = it.curr_;

  if (!b || !b->parent || !b->parent->parent) {
    throw std::logic_error("Invalid large left rotation");
  }
  detail::Node< K, V >* c = b->parent;
  detail::Node< K, V >* a = c->parent;
  if (a->rt != c || c->lt != b) {
    throw std::logic_error("Not a large left rotation configuration");
  }
  rotateRight(const_iterator(b));
  return rotateLeft(const_iterator(b));
}

template< class K, class V, class Cmp >
typename kuznetsov::BSTree< K, V, Cmp >::const_iterator
  kuznetsov::BSTree< K, V, Cmp >::rotateLargeRight(const_iterator it)
{
  detail::Node< K, V >* b = it.curr_;

  if (!b || !b->parent || !b->parent->parent) {
    throw std::logic_error("Invalid large right rotation");
  }

  detail::Node< K, V >* a = b->parent;
  detail::Node< K, V >* c = a->parent;

  if (c->lt != a || a->rt != b) {
    throw std::logic_error("Not a large right rotation configuration");
  }

  rotateLeft(const_iterator(b));
  return rotateRight(const_iterator(b));
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::iterator kuznetsov::BSTree< K, V, C >::begin()
{
  return iterator(detail::minimum(root_));
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::const_iterator kuznetsov::BSTree< K, V, C >::begin() const
{
  return const_iterator(detail::minimum(root_));
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::const_iterator kuznetsov::BSTree< K, V, C >::cbegin() const
{
  return const_iterator(detail::minimum(root_));
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::iterator kuznetsov::BSTree< K, V, C >::end()
{
  return iterator(nullptr);
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::const_iterator kuznetsov::BSTree< K, V, C >::end() const
{
  return const_iterator(nullptr);
}

template< class K, class V, class C >
typename kuznetsov::BSTree< K, V, C >::const_iterator kuznetsov::BSTree< K, V, C >::cend() const
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator kuznetsov::BSTree< Key, Value, Compare >::root() const
{
  return const_iterator(root_);
}

template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator
  kuznetsov::BSTree< Key, Value, Compare >::leftChild(const_iterator it) const
{
  return const_iterator(it.curr_->lt);
}
template< class Key, class Value, class Compare >
typename kuznetsov::BSTree< Key, Value, Compare >::const_iterator
  kuznetsov::BSTree< Key, Value, Compare >::rightChild(const_iterator it) const
{
  return const_iterator(it.curr_->rt);
}

#endif
