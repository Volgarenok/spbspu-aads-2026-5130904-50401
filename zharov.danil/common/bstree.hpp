#ifndef BSTREE_HPP
#define BSTREE_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zharov
{
  namespace detail
  {
    template< class Key, class Value >
    struct Node
    {
      std::pair< const Key, Value > data_;
      Node< Key, Value >* left_;
      Node< Key, Value >* right_;
      Node< Key, Value >* parent_;

      static Node< Key, Value >* fake;

      Node(const Key& key, const Value& val, Node< Key, Value >* parent);
      Node(Key&& key, Value&& val, Node< Key, Value >* parent);

      bool isFake() const;
    };

    template< class Key, class Value >
    detail::Node< Key, Value >* detail::Node< Key, Value >::fake = nullptr;
  }

  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    template< class, class, class >
    friend class BSTree;

    BSTConstIterator(detail::Node< Key, Value >* node = nullptr);
    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;
    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);
    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);
    bool operator==(const BSTConstIterator& other) const;
    bool operator!=(const BSTConstIterator& other) const;

  private:
    detail::Node< Key, Value >* curr_;
  };

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    template< class, class, class >
    friend class BSTree;

    BSTIterator(detail::Node< Key, Value >* node = nullptr);
    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();
    BSTIterator& operator++();
    BSTIterator operator++(int);
    BSTIterator& operator--();
    BSTIterator operator--(int);
    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

  private:
    detail::Node< Key, Value >* curr_;
  };

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree() noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    size_t getSize() const;
    void swap(BSTree& other) noexcept;
    void clear() noexcept;

    void push(const Key& k, const Value& v);
    void push(Key&& k, Value&& v);
    detail::Node< Key, Value >* findNode(const Key& k) const;
    bool has(const Key& k) const;
    Value get(const Key& k) const;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    detail::Node< Key, Value >* fallLeft(detail::Node< Key, Value >* node) const;
    void pop(const Key& k);
    bool isStructEqual(const BSTree& other) const;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    size_t height(const_iterator it) const;
    size_t height() const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    detail::Node< Key, Value >* root_;
    size_t size_;
    Compare comp_;

    detail::Node< Key, Value >* initFake();
    detail::Node< Key, Value >* clone(detail::Node< Key, Value >* root,
      detail::Node< Key, Value >* parent);
    void deleteNodes(detail::Node< Key, Value >* node) noexcept;
    template< class K, class V >
    void pushImpl(K&& k, V&& v);
    size_t getHight(detail::Node< Key, Value >* node) const;
    bool isStructEqualImpl(const detail::Node< Key, Value >* a,
      const detail::Node< Key, Value >* b) const;
  };
}

template< class Key, class Value >
zharov::detail::Node< Key, Value >::Node(const Key& key,
  const Value& val,
  Node< Key, Value >* parent):
  data_(key, val),
  left_(fake),
  right_(fake),
  parent_(parent)
{}

template< class Key, class Value >
zharov::detail::Node< Key, Value >::Node(Key&& key, Value&& val, Node< Key, Value >* parent):
  data_(key, val),
  left_(fake),
  right_(fake),
  parent_(parent)
{}

template< class Key, class Value >
bool zharov::detail::Node< Key, Value >::isFake() const
{
  return this == fake;
}

template< class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::initFake()
{
  using Node = detail::Node< Key, Value >;
  if (Node::fake == nullptr)
  {
    Node::fake = new detail::Node< Key, Value >(Key(), Value(), nullptr);
    Node::fake->left_ = detail::Node< Key, Value >::fake;
    Node::fake->right_ = detail::Node< Key, Value >::fake;
    Node::fake->parent_ = detail::Node< Key, Value >::fake;
  }
  return detail::Node< Key, Value >::fake;
}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree():
  root_(initFake()),
  size_(0),
  comp_()
{}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::~BSTree() noexcept
{
  clear();
}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree(const BSTree& other):
  root_(initFake()),
  size_(0),
  comp_(other.comp_)
{
  root_ = clone(other.root_, root_);
  size_ = other.size_;
}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >::BSTree(BSTree&& other) noexcept:
  root_(std::exchange(other.root_, detail::Node< Key, Value >::fake)),
  size_(std::exchange(other.size_, 0)),
  comp_(std::move(other.comp_))
{}

template< class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::getSize() const
{
  return size_;
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::deleteNodes(detail::Node< Key, Value >* node) noexcept
{
  if (node->isFake())
  {
    return;
  }
  deleteNodes(node->left_);
  deleteNodes(node->right_);
  delete node;
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::clear() noexcept
{
  deleteNodes(root_);
  root_ = detail::Node< Key, Value >::fake;
  size_ = 0;
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
{
  std::swap(other.root_, root_);
  std::swap(other.size_, size_);
  std::swap(other.comp_, comp_);
}

template< class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::clone(
  detail::Node< Key, Value >* root,
  detail::Node< Key, Value >* parent)
{
  using Node = detail::Node< Key, Value >;
  if (root->isFake())
  {
    return Node::fake;
  }
  Node* node = new Node(root->data_.first, root->data_.second, parent);
  node->left_ = clone(root->left_, node);
  node->right_ = clone(root->right_, node);
  return node;
}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >& zharov::BSTree< Key, Value, Compare >::operator=(
  const BSTree& other)
{
  if (this != std::addressof(other))
  {
    BSTree< Key, Value, Compare > copy(other);
    swap(copy);
  }
  return *this;
}

template< class Key, class Value, class Compare >
zharov::BSTree< Key, Value, Compare >& zharov::BSTree< Key, Value, Compare >::operator=(
  BSTree&& other) noexcept
{
  if (this != std::addressof(other))
  {
    BSTree< Key, Value, Compare > copy(std::move(other));
    swap(copy);
  }
  return *this;
}

template< class Key, class Value, class Compare >
template< class K, class V >
void zharov::BSTree< Key, Value, Compare >::pushImpl(K&& k, V&& v)
{
  using Node = detail::Node< Key, Value >;

  if (root_->isFake())
  {
    root_ = new Node(std::forward< K >(k), std::forward< V >(v), Node::fake);
    ++size_;
    return;
  }

  Node* curr = root_;
  while (true)
  {
    if (comp_(k, curr->data_.first))
    {
      if (curr->left_->isFake())
      {
        curr->left_ = new Node(std::forward< K >(k), std::forward< V >(v), curr);
        ++size_;
        return;
      }
      curr = curr->left_;
    }
    else if (comp_(curr->data_.first, k))
    {
      if (curr->right_->isFake())
      {
        curr->right_ = new Node(std::forward< K >(k), std::forward< V >(v), curr);
        ++size_;
        return;
      }
      curr = curr->right_;
    }
    else
    {
      curr->data_.second = std::forward< V >(v);
      return;
    }
  }
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::push(const Key& k, const Value& v)
{
  pushImpl(k, v);
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::push(Key&& k, Value&& v)
{
  pushImpl(std::move(k), std::move(v));
}

template< class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::findNode(
  const Key& k) const
{
  detail::Node< Key, Value >* curr = root_;
  while (!curr->isFake())
  {
    if (comp_(k, curr->data_.first))
    {
      curr = curr->left_;
    }
    else if (comp_(curr->data_.first, k))
    {
      curr = curr->right_;
    }
    else
    {
      return curr;
    }
  }
  return nullptr;
}

template< class Key, class Value, class Compare >
bool zharov::BSTree< Key, Value, Compare >::has(const Key& k) const
{
  return findNode(k) != nullptr;
}

template< class Key, class Value, class Compare >
Value zharov::BSTree< Key, Value, Compare >::get(const Key& k) const
{
  return at(k);
}

template< class Key, class Value, class Compare >
Value& zharov::BSTree< Key, Value, Compare >::at(const Key& k)
{
  const BSTree< Key, Value, Compare >* const_tree = this;
  return const_cast< Value& >((*const_tree).at(k));
}

template< class Key, class Value, class Compare >
const Value& zharov::BSTree< Key, Value, Compare >::at(const Key& k) const
{
  const detail::Node< Key, Value >* node = findNode(k);
  if (node == nullptr)
  {
    throw std::logic_error("Key no found");
  }
  return node->data_.second;
}

template< class Key, class Value, class Compare >
zharov::detail::Node< Key, Value >* zharov::BSTree< Key, Value, Compare >::fallLeft(
  detail::Node< Key, Value >* node) const
{
  while (!node->left_->isFake())
  {
    node = node->left_;
  }
  return node;
}

template< class Key, class Value, class Compare >
void zharov::BSTree< Key, Value, Compare >::pop(const Key& k)
{
  detail::Node< Key, Value >* node = findNode(k);
  if (node == nullptr)
  {
    throw std::out_of_range("Key not found");
  }

  if (!node->left_->isFake() && !node->right_->isFake())
  {
    detail::Node< Key, Value >* n = fallLeft(node->right_);
    const_cast< Key& >(node->data_.first) = std::move(const_cast< Key& >(n->data_.first));
    node->data_.second = std::move(n->data_.second);
    node = n;
  }

  detail::Node< Key, Value >* child = (!node->left_->isFake()) ? node->left_ : node->right_;
  if (!child->isFake())
  {
    child->parent_ = node->parent_;
  }

  if (node->parent_->isFake())
  {
    root_ = child;
  }
  else if (node->parent_->left_ == node)
  {
    node->parent_->left_ = child;
  }
  else
  {
    node->parent_->right_ = child;
  }

  delete node;
  --size_;
}

template< class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::getHight(detail::Node< Key, Value >* node) const
{
  if (node->isFake())
  {
    return 0;
  }
  size_t left = getHight(node->left_);
  size_t right = getHight(node->right_);
  return 1 + (left > right ? left : right);
}

template< class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::height(const_iterator it) const
{
  return getHight(it.curr_);
}

template< class Key, class Value, class Compare >
size_t zharov::BSTree< Key, Value, Compare >::height() const
{
  return getHight(root_);
}

template< class Key, class Value >
zharov::BSTIterator< Key, Value >::BSTIterator(detail::Node< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
std::pair< const Key, Value >& zharov::BSTIterator< Key, Value >::operator*()
{
  return curr_->data_;
}

template< class Key, class Value >
std::pair< const Key, Value >* zharov::BSTIterator< Key, Value >::operator->()
{
  return std::addressof(curr_->data_);
}

template< class Key, class Value >
zharov::BSTIterator< Key, Value >& zharov::BSTIterator< Key, Value >::operator++()
{
  if (!curr_)
  {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (!next->right_->isFake())
  {
    next = next->right_;
    while (!next->left_->isFake())
    {
      next = next->left_;
    }
  }
  else
  {
    detail::Node< Key, Value >* parent = next->parent_;
    while (!parent->isFake() && parent->left_ != next)
    {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
    if (next->isFake())
    {
      next = nullptr;
    }
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
zharov::BSTIterator< Key, Value > zharov::BSTIterator< Key, Value >::operator++(int)
{
  BSTIterator< Key, Value > temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value >
zharov::BSTIterator< Key, Value >& zharov::BSTIterator< Key, Value >::operator--()
{
  if (!curr_)
  {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (!next->left_->isFake())
  {
    next = next->left_;
    while (!next->right_->isFake())
    {
      next = next->right_;
    }
  }
  else
  {
    detail::Node< Key, Value >* parent = next->parent_;
    while (!parent->isFake() && parent->right_ != next)
    {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
    if (next->isFake())
    {
      next = nullptr;
    }
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
zharov::BSTIterator< Key, Value > zharov::BSTIterator< Key, Value >::operator--(int)
{
  BSTIterator< Key, Value > temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value >
bool zharov::BSTIterator< Key, Value >::operator==(const BSTIterator& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool zharov::BSTIterator< Key, Value >::operator!=(const BSTIterator& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value >
zharov::BSTConstIterator< Key, Value >::BSTConstIterator(detail::Node< Key, Value >* node):
  curr_(node)
{}

template< class Key, class Value >
const std::pair< const Key, Value >& zharov::BSTConstIterator< Key, Value >::operator*() const
{
  return curr_->data_;
}

template< class Key, class Value >
const std::pair< const Key, Value >* zharov::BSTConstIterator< Key, Value >::operator->() const
{
  return std::addressof(curr_->data_);
}

template< class Key, class Value >
zharov::BSTConstIterator< Key, Value >& zharov::BSTConstIterator< Key, Value >::operator++()
{
  if (!curr_)
  {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (!next->right_->isFake())
  {
    next = next->right_;
    while (!next->left_->isFake())
    {
      next = next->left_;
    }
  }
  else
  {
    detail::Node< Key, Value >* parent = next->parent_;
    while (!parent->isFake() && parent->left_ != next)
    {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
    if (next->isFake())
    {
      next = nullptr;
    }
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
zharov::BSTConstIterator< Key, Value > zharov::BSTConstIterator< Key, Value >::operator++(int)
{
  BSTConstIterator temp = *this;
  ++(*this);
  return temp;
}

template< class Key, class Value >
zharov::BSTConstIterator< Key, Value >& zharov::BSTConstIterator< Key, Value >::operator--()
{
  if (!curr_)
  {
    return *this;
  }
  detail::Node< Key, Value >* next = curr_;
  if (!next->left_->isFake())
  {
    next = next->left_;
    while (!next->right_->isFake())
    {
      next = next->right_;
    }
  }
  else
  {
    detail::Node< Key, Value >* parent = next->parent_;
    while (!parent->isFake() && parent->right_ != next)
    {
      next = parent;
      parent = next->parent_;
    }
    next = parent;
    if (next->isFake())
    {
      next = nullptr;
    }
  }
  curr_ = next;
  return *this;
}

template< class Key, class Value >
zharov::BSTConstIterator< Key, Value > zharov::BSTConstIterator< Key, Value >::operator--(int)
{
  BSTConstIterator temp = *this;
  --(*this);
  return temp;
}

template< class Key, class Value >
bool zharov::BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& other) const
{
  return curr_ == other.curr_;
}

template< class Key, class Value >
bool zharov::BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& other) const
{
  return curr_ != other.curr_;
}

template< class Key, class Value, class Compare >
zharov::BSTIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::begin()
{
  if (root_->isFake())
  {
    return iterator(nullptr);
  }
  return iterator(fallLeft(root_));
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::begin() const
{
  if (root_->isFake())
  {
    return const_iterator(nullptr);
  }
  return const_iterator(fallLeft(root_));
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::cbegin() const
{
  if (root_->isFake())
  {
    return const_iterator(nullptr);
  }
  return const_iterator(fallLeft(root_));
}

template< class Key, class Value, class Compare >
zharov::BSTIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::end()
{
  return iterator(nullptr);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::end() const
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::cend() const
{
  return const_iterator(nullptr);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::rotateLeft(
  const_iterator it)
{
  using Node = detail::Node< Key, Value >;
  Node* x = it.curr_;
  if (!x || x->right_->isFake())
  {
    return it;
  }
  Node* y = x->right_;
  x->right_ = y->left_;
  if (!y->left_->isFake())
  {
    y->left_->parent_ = x;
  }
  y->parent_ = x->parent_;
  if (x->parent_->isFake())
  {
    root_ = y;
  }
  else if (x == x->parent_->left_)
  {
    x->parent_->left_ = y;
  }
  else
  {
    x->parent_->right_ = y;
  }
  y->left_ = x;
  x->parent_ = y;
  return const_iterator(y);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::rotateRight(
  const_iterator it)
{
  using Node = detail::Node< Key, Value >;
  Node* y = it.curr_;
  if (!y || y->left_->isFake())
  {
    return it;
  }
  Node* x = y->left_;
  y->left_ = x->right_;
  if (!x->right_->isFake())
  {
    x->right_->parent_ = y;
  }
  x->parent_ = y->parent_;
  if (y->parent_->isFake())
  {
    root_ = x;
  }
  else if (y == y->parent_->left_)
  {
    y->parent_->left_ = x;
  }
  else
  {
    y->parent_->right_ = x;
  }
  x->right_ = y;
  y->parent_ = x;
  return const_iterator(x);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::rotateLargeLeft(
  const_iterator it)
{
  using Node = detail::Node< Key, Value >;
  Node* x = it.curr_;
  if (!x || x->right_->isFake() || x->right_->left_->isFake())
  {
    return it;
  }
  rotateRight(const_iterator(x->right_));
  return rotateLeft(it);
}

template< class Key, class Value, class Compare >
zharov::BSTConstIterator< Key, Value > zharov::BSTree< Key, Value, Compare >::rotateLargeRight(
  const_iterator it)
{
  using Node = detail::Node< Key, Value >;
  Node* x = it.curr_;
  if (!x || x->left_->isFake() || x->left_->right_->isFake())
  {
    return it;
  }
  rotateLeft(const_iterator(x->left_));
  return rotateRight(it);
}

template< class Key, class Value, class Compare >
bool zharov::BSTree< Key, Value, Compare >::isStructEqualImpl(const detail::Node< Key, Value >* a,
  const detail::Node< Key, Value >* b) const
{
  if (a->isFake() && b->isFake())
  {
    return true;
  }
  if (a->isFake() || b->isFake())
  {
    return false;
  }
  if (a->data_.first != b->data_.first || a->data_.second != b->data_.second)
  {
    return false;
  }
  return isStructEqualImpl(a->left_, b->left_) && isStructEqualImpl(a->right_, b->right_);
}

template< class Key, class Value, class Compare >
bool zharov::BSTree< Key, Value, Compare >::isStructEqual(
  const BSTree< Key, Value, Compare >& other) const
{
  return isStructEqualImpl(root_, other.root_);
}

#endif
