#ifndef BINARY_TREE_HPP
#define BINARY_TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <initializer_list>
#include <algorithm>
#include <functional>
#include <memory>

namespace tarasenko
{
  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  namespace detail
  {
    template< class Key, class Value >
    struct Node
    {
      std::pair< Key, Value > data;
      Node< Key, Value >* left;
      Node< Key, Value >* right;
      Node< Key, Value >* parent;
    };

    template< class Key, class Value >
    Node< Key, Value >* next(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* prev(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* fallLeft(Node< Key, Value >* node);
    template< class Key, class Value >
    Node< Key, Value >* fallRight(Node< Key, Value >* node);
    template< class Key, class Value >
    size_t height(Node< Key, Value >* node);
  }

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;
    using Node = detail::Node< Key, Value >;

    BSTree();

    BSTree(const BSTree< Key, Value, Compare >& rhs);
    BSTree(BSTree< Key, Value, Compare >&& rhs) noexcept;

    BSTree(std::initializer_list< std::pair< Key, Value > > list);
    ~BSTree();

    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >& rhs);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&& rhs) noexcept;
    Value& operator[](const Key& key);
    Value& operator[](Key&& key);

    std::pair< iterator, bool > add(const Key& key, const Value& val);
    std::pair< iterator, bool > add(const Key& key, Value&& val);
    size_t drop(const Key& key);
    const Value& at(const Key& key) const;
    Value& at(const Key& key);
    bool has(const Key& key) const;

    void clear();

    size_t size() const;
    bool empty() const;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    size_t height(const_iterator it) const;
    size_t height() const;

  private:
    Node* root_;
    size_t size_;
    Compare comp_;

    Node* find(const Key& key) const;
    template< class K, class T >
    std::pair< iterator, bool > addInner(K&& key, T&& val);
    void replace(Node* node, Node* child);
    void clear(Node* node);
    Node* copy(Node* node, Node* parent);
    void swap(BSTree< Key, Value, Compare >& rhs) noexcept;
  };

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using Node = detail::Node< Key, Value >;
    using Pair = std::pair< Key, Value >;

    BSTIterator();

    Pair& operator*();
    Pair* operator->();

    BSTIterator& operator++();
    BSTIterator operator++(int);

    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& it) const;
    bool operator!=(const BSTIterator& it) const;

  private:
    Node* node_ = nullptr;
    Node* root_ = nullptr;

    friend class BSTree< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    BSTIterator(Node* node, Node* root) noexcept;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using Node = detail::Node< Key, Value >;
    using Pair = std::pair< Key, Value >;

    BSTConstIterator();
    BSTConstIterator(const BSTIterator< Key, Value >& it);

    const Pair& operator*() const;
    const Pair* operator->() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator& it) const;
    bool operator!=(const BSTConstIterator& it) const;

  private:
    Node* node_ = nullptr;
    Node* root_ = nullptr;

    friend class BSTree< Key, Value >;
    BSTConstIterator(Node* node, Node* root) noexcept;
  };

  template< class Key, class Value, class Compare >
  std::pair< BSTIterator< Key, Value >, bool > BSTree< Key, Value, Compare >::add(const Key& key, const Value& val)
  {
    return addInner(key, val);
  }

  template< class Key, class Value, class Compare >
  std::pair< BSTIterator< Key, Value >, bool > BSTree< Key, Value, Compare >::add(const Key& key, Value&& val)
  {
    return addInner(key, std::move(val));
  }

  template< class Key, class Value, class Compare >
  template< class K, class T >
  std::pair< BSTIterator< Key, Value >, bool > BSTree< Key, Value, Compare >::addInner(K&& key, T&& val)
  {
    if (!root_)
    {
      root_ = new detail::Node< Key, Value >{{std::forward< K >(key), std::forward< T >(val)}, nullptr, nullptr, nullptr};
      ++size_;
      return {BSTIterator< Key, Value >(root_, root_), true};
    }
    detail::Node< Key, Value >* cur = root_;
    while (cur)
    {
      if (comp_(cur->data.first, key))
      {
        if (!cur->right)
        {
          detail::Node< Key, Value >* node = new detail::Node< Key, Value >{{std::forward< K >(key), std::forward< T >(val)}, nullptr, nullptr, cur};
          cur->right = node;
          ++size_;
          return {BSTIterator< Key, Value >(node, root_), true};
        }
        cur = cur->right;
      }
      else if (comp_(key, cur->data.first))
      {
        if (!cur->left)
        {
          detail::Node< Key, Value >* node = new detail::Node< Key, Value >{{std::forward< K >(key), std::forward< T >(val)}, nullptr, nullptr, cur};
          cur->left = node;
          ++size_;
          return {BSTIterator< Key, Value >(node, root_), true};
        }
        cur = cur->left;
      }
      else
      {
        return {BSTIterator< Key, Value >(cur, root_), false};
      }
    }
    return {end(), false};
  }

  template< class Key, class Value, class Compare >
  detail::Node< Key, Value >* BSTree< Key, Value, Compare >::find(const Key& key) const
  {
    detail::Node< Key, Value >* cur = root_;
    while (cur)
    {
      if (comp_(cur->data.first, key))
      {
        cur = cur->right;
      }
      else if (comp_(key, cur->data.first))
      {
        cur = cur->left;
      }
      else
      {
        return cur;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::replace(detail::Node< Key, Value >* node, detail::Node< Key, Value >* child)
  {
    if (node->parent == nullptr)
    {
      root_ = child;
    }
    else if (node == node->parent->left)
    {
      node->parent->left = child;
    }
    else
    {
      node->parent->right = child;
    }
    if (child)
    {
      child->parent = node->parent;
    }
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::drop(const Key& key)
  {
    detail::Node< Key, Value >* node = find(key);
    if (!node)
    {
      return 0;
    }
    if (!node->left)
    {
      replace(node, node->right);
      delete node;
      --size_;
      return 1;
    }
    if (!node->right)
    {
      replace(node, node->left);
      delete node;
      --size_;
      return 1;
    }
    detail::Node< Key, Value >* next = detail::fallLeft(node->right);
    if (next->parent != node)
    {
      replace(next, next->right);
      next->right = node->right;
      next->right->parent = next;
    }
    replace(node, next);
    next->left = node->left;
    next->left->parent = next;
    delete node;
    --size_;
    return 1;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr),
    size_(0),
    comp_(Compare{})
  {}

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return !size_;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](const Key& key)
  {
    return addInner(key, Value{}).first->second;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](Key&& key)
  {
    return addInner(std::move(key), Value{}).first->second;
  }

  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::at(const Key& key) const
  {
    detail::Node< Key, Value >* node = find(key);
    if (!node)
    {
      throw std::runtime_error("Key not found");
    }
    return node->data.second;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::at(const Key& key)
  {
    return const_cast< Value& >(const_cast< const BSTree< Key, Value, Compare >* >(this)->at(key));
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::has(const Key& key) const
  {
    return find(key);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    clear(root_);
    root_ = nullptr;
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(detail::Node< Key, Value >* node)
  {
    if (node)
    {
      clear(node->left);
      clear(node->right);
      delete node;
    }
  }

  template< class Key, class Value, class Compare >
  detail::Node< Key, Value >* BSTree< Key, Value, Compare >::copy(detail::Node< Key, Value >* node, detail::Node< Key, Value >* parent)
  {
    if (!node)
    {
      return nullptr;
    }
    detail::Node< Key, Value >* cur = new detail::Node< Key, Value >{node->data, nullptr, nullptr, parent};
    try
    {
      cur->left = copy(node->left, cur);
      cur->right = copy(node->right, cur);
    }
    catch (...)
    {
      clear(cur);
      throw;
    }

    return cur;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree< Key, Value, Compare >& rhs):
    root_(copy(rhs.root_, nullptr)),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree< Key, Value, Compare >&& rhs) noexcept:
    root_(rhs.root_),
    size_(rhs.size_),
    comp_(rhs.comp_)
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree< Key, Value, Compare >& rhs) noexcept
  {
    std::swap(root_, rhs.root_);
    std::swap(size_, rhs.size_);
    std::swap(comp_, rhs.comp_);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree< Key, Value, Compare >& rhs)
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    BSTree< Key, Value, Compare > copy = rhs;
    swap(copy);

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree< Key, Value, Compare >&& rhs) noexcept
  {
    if (this == std::addressof(rhs))
    {
      return *this;
    }
    swap(rhs);

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(std::initializer_list< std::pair< Key, Value > > list):
    BSTree()
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      add(it->first, it->second);
    }
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator():
    node_(nullptr),
    root_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(detail::Node< Key, Value >* node, detail::Node< Key, Value >* root) noexcept:
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(detail::Node< Key, Value >* node, detail::Node< Key, Value >* root) noexcept:
    node_(node),
    root_(root)
  {}

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*()
  {
    return node_->data;
  }

  template< class Key, class Value >
  const std::pair< Key, Value >& BSTConstIterator< Key, Value >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->()
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value >
  const std::pair< Key, Value >* BSTConstIterator< Key, Value >::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value >
  detail::Node< Key, Value >* detail::fallLeft(detail::Node< Key, Value >* node)
  {
    while (node && node->left)
    {
      node = node->left;
    }
    return node;
  }

  template< class Key, class Value >
  detail::Node< Key, Value >* detail::next(detail::Node< Key, Value >* node)
  {
    if (node->right)
    {
      return fallLeft(node->right);
    }
    else
    {
      while (node->parent && node->parent->right == node)
      {
        node = node->parent;
      }
      return node->parent;
    }
  }

  template< class Key, class Value >
  detail::Node< Key, Value >* detail::fallRight(detail::Node< Key, Value >* node)
  {
    while (node && node->right)
    {
      node = node->right;
    }
    return node;
  }

  template< class Key, class Value >
  detail::Node< Key, Value >* detail::prev(detail::Node< Key, Value >* node)
  {
    if (node->left)
    {
      return fallRight(node->left);
    }
    else
    {
      while (node->parent && node->parent->left == node)
      {
        node = node->parent;
      }
      return node->parent;
    }
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
  {
    node_ = detail::next(node_);
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >& BSTConstIterator< Key, Value >::operator++()
  {
    node_ = detail::next(node_);
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator++(int)
  {
    BSTIterator< Key, Value > copy = *this;
    ++(*this);
    return copy;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator++(int)
  {
    BSTConstIterator< Key, Value > copy = *this;
    ++(*this);
    return copy;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator--()
  {
    if (!node_)
    {
      node_ = detail::fallRight(root_);
      return *this;
    }
    node_ = detail::prev(node_);
    return *this;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >& BSTConstIterator< Key, Value >::operator--()
  {
    if (!node_)
    {
      node_ = detail::fallRight(root_);
      return *this;
    }
    node_ = detail::prev(node_);
    return *this;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > BSTIterator< Key, Value >::operator--(int)
  {
    BSTIterator< Key, Value > copy = *this;
    --(*this);
    return copy;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > BSTConstIterator< Key, Value >::operator--(int)
  {
    BSTConstIterator< Key, Value > copy = *this;
    --(*this);
    return copy;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator< Key, Value >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator< Key, Value >& rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator< Key, Value >& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator< Key, Value >& rhs) const
  {
    return !(*this == rhs);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::begin()
  {
    return BSTIterator< Key, Value >(detail::fallLeft(root_), root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::begin() const
  {
    return BSTConstIterator< Key, Value >(detail::fallLeft(root_), root_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::end()
  {
    return BSTIterator< Key, Value >(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::end() const
  {
    return BSTConstIterator< Key, Value >(nullptr, root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cbegin() const
  {
    return BSTConstIterator< Key, Value >(detail::fallLeft(root_), root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cend() const
  {
    return BSTConstIterator< Key, Value >(nullptr, root_);
  }

  template< class Key, class Value >
  size_t detail::height(detail::Node< Key, Value >* node)
  {
    if (!node)
    {
      return 0;
    }
    return std::max(height(node->left), height(node->right)) + 1;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(BSTConstIterator< Key, Value > it) const
  {
    return detail::height(it.node_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return detail::height(root_);
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const BSTIterator< Key, Value >& it):
    node_(it.node_),
    root_(it.root_)
  {}

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLeft(BSTConstIterator< Key, Value > it)
  {
    detail::Node< Key, Value >* p = it.node_;
    detail::Node< Key, Value >* q = p->parent;
    q->right = p->left;
    if (p->left)
    {
      p->left->parent = q;
    }
    p->parent = q->parent;
    if (!q->parent)
    {
      root_ = p;
    }
    else if (q == q->parent->left)
    {
      q->parent->left = p;
    }
    else
    {
      q->parent->right = p;
    }
    q->parent = p;
    p->left = q;
    return BSTConstIterator< Key, Value >(p, root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(BSTConstIterator< Key, Value > it)
  {
    detail::Node< Key, Value >* p = it.node_;
    detail::Node< Key, Value >* q = p->parent;
    q->left = p->right;
    if (p->right)
    {
      p->right->parent = q;
    }
    p->parent = q->parent;
    if (!q->parent)
    {
      root_ = p;
    }
    else if (q == q->parent->left)
    {
      q->parent->left = p;
    }
    else
    {
      q->parent->right = p;
    }
    q->parent = p;
    p->right = q;
    return BSTConstIterator< Key, Value >(p, root_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(BSTConstIterator< Key, Value > it)
  {
    it = rotateRight(it);
    return rotateLeft(it);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(BSTConstIterator< Key, Value > it)
  {
    it = rotateLeft(it);
    return rotateRight(it);
  }

}

#endif
