#ifndef AVLTREE_HPP
#define AVLTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <algorithm>
#include <list.hpp>

namespace burukov
{
  namespace detail
  {
    template< class Key, class Value >
    struct AVLNode
    {
      Key key_;
      Value value_;
      AVLNode* left_;
      AVLNode* right_;
      AVLNode* parent_;
      int height_;

      static AVLNode* nilNode;

      AVLNode(const Key& key, const Value& value, AVLNode* parent):
        key_(key),
        value_(value),
        left_(nilNode),
        right_(nilNode),
        parent_(parent),
        height_(1)
      {}

      AVLNode(Key&& key, Value&& value, AVLNode* parent):
        key_(std::forward< Key >(key)),
        value_(std::forward< Value >(value)),
        left_(nilNode),
        right_(nilNode),
        parent_(parent),
        height_(1)
      {}

      bool isNil() const noexcept
      {
        return this == nilNode;
      }

      void updateHeight()
      {
        if (isNil())
        {
          height_ = 0;
          return;
        }
        height_ = 1 + std::max(left_->height_, right_->height_);
      }
    };

    template< class Key, class Value >
    AVLNode< Key, Value >* AVLNode< Key, Value >::nilNode = nullptr;
  }

  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
    using Node = detail::AVLNode< Key, Value >;

    AVLTree();
    ~AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree(AVLTree&& other) noexcept;
    AVLTree& operator=(const AVLTree& other);
    AVLTree& operator=(AVLTree&& other) noexcept;

    bool empty() const;
    size_t size() const;
    void clear();

    const Value& at(const Key& k) const;
    Value& at(const Key& k);
    void push(const Key& k, const Value& v);
    void push(Key&& k, Value&& v);
    Value drop(const Key& k);
    bool hasKey(const Key& k) const;

    template< class Func >
    void traverseInOrder(Func f) const;

  private:
    Node* root_;
    Node* nilNode_;
    size_t size_;
    Compare comp_;

    static void initNil();
    Node* findNode(const Key& k) const;
    void deleteTree(Node* node);
    void copyTree(Node*& dest, Node* src, Node* parent);
    int getBalance(Node* n) const;
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* balance(Node* z);
    Node* findMinNode(Node* node) const;
    void insertAndBalance(Node* n, Node* parent, const Key& k);
  };

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::initNil()
  {
    if (Node::nilNode == nullptr)
    {
      Node::nilNode = new Node(Key(), Value(), nullptr);
      Node::nilNode->left_ = Node::nilNode;
      Node::nilNode->right_ = Node::nilNode;
      Node::nilNode->parent_ = Node::nilNode;
      Node::nilNode->height_ = 0;
    }
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree():
    root_(nullptr),
    nilNode_(nullptr),
    size_(0),
    comp_()
  {
    initNil();
    nilNode_ = Node::nilNode;
    root_ = new Node(Key(), Value(), nullptr);
    root_->left_ = nilNode_;
    root_->right_ = nilNode_;
    root_->parent_ = nilNode_;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    if (root_ != nullptr)
    {
      if (root_->right_ != nullptr && !root_->right_->isNil())
      {
        deleteTree(root_->right_);
      }
      delete root_;
    }
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::copyTree(Node*& dest, Node* src, Node* parent)
  {
    if (src == nullptr || src->isNil())
    {
      dest = nilNode_;
      return;
    }
    dest = new Node(src->key_, src->value_, parent);
    copyTree(dest->left_, src->left_, dest);
    copyTree(dest->right_, src->right_, dest);
    dest->updateHeight();
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree& other):
    root_(nullptr),
    nilNode_(nullptr),
    size_(other.size_),
    comp_(other.comp_)
  {
    initNil();
    nilNode_ = Node::nilNode;
    root_ = new Node(Key(), Value(), nullptr);
    root_->left_ = nilNode_;
    root_->right_ = nilNode_;
    root_->parent_ = nilNode_;
    if (other.root_ != nullptr && other.root_->right_ != nullptr
      && !other.root_->right_->isNil())
    {
      copyTree(root_->right_, other.root_->right_, root_);
    }
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree&& other) noexcept:
    root_(std::exchange(other.root_, nullptr)),
    nilNode_(std::exchange(other.nilNode_, nullptr)),
    size_(std::exchange(other.size_, 0)),
    comp_(std::move(other.comp_))
  {
    initNil();
    other.nilNode_ = Node::nilNode;
    other.root_ = new Node(Key(), Value(), nullptr);
    other.root_->left_ = other.nilNode_;
    other.root_->right_ = other.nilNode_;
    other.root_->parent_ = other.nilNode_;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >&
  AVLTree< Key, Value, Compare >::operator=(const AVLTree& other)
  {
    if (this != &other)
    {
      AVLTree temp(other);
      std::swap(root_, temp.root_);
      std::swap(nilNode_, temp.nilNode_);
      std::swap(size_, temp.size_);
      std::swap(comp_, temp.comp_);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >&
  AVLTree< Key, Value, Compare >::operator=(AVLTree&& other) noexcept
  {
    if (this != &other)
    {
      std::swap(root_, other.root_);
      std::swap(nilNode_, other.nilNode_);
      std::swap(size_, other.size_);
      std::swap(comp_, other.comp_);
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::deleteTree(Node* node)
  {
    if (node == nullptr || node->isNil())
    {
      return;
    }
    if (node->left_ != nullptr && !node->left_->isNil())
    {
      deleteTree(node->left_);
    }
    if (node->right_ != nullptr && !node->right_->isNil())
    {
      deleteTree(node->right_);
    }
    delete node;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::clear()
  {
    if (root_ != nullptr && root_->right_ != nullptr
      && !root_->right_->isNil())
    {
      deleteTree(root_->right_);
      root_->right_ = nilNode_;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  size_t AVLTree< Key, Value, Compare >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::findNode(const Key& k) const
  {
    if (root_ == nullptr || root_->right_ == nullptr || root_->right_->isNil())
    {
      return nullptr;
    }
    Node* cur = root_->right_;
    while (cur != nullptr && !cur->isNil())
    {
      if (comp_(k, cur->key_))
      {
        cur = cur->left_;
      }
      else if (comp_(cur->key_, k))
      {
        cur = cur->right_;
      }
      else
      {
        return cur;
      }
    }
    return nullptr;
  }

  template< class Key, class Value, class Compare >
  bool AVLTree< Key, Value, Compare >::hasKey(const Key& k) const
  {
    return findNode(k) != nullptr;
  }

  template< class Key, class Value, class Compare >
  const Value& AVLTree< Key, Value, Compare >::at(const Key& k) const
  {
    Node* n = findNode(k);
    if (n == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return n->value_;
  }

  template< class Key, class Value, class Compare >
  Value& AVLTree< Key, Value, Compare >::at(const Key& k)
  {
    Node* n = findNode(k);
    if (n == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return n->value_;
  }

  template< class Key, class Value, class Compare >
  int AVLTree< Key, Value, Compare >::getBalance(Node* n) const
  {
    if (n == nullptr || n->isNil())
    {
      return 0;
    }
    return n->left_->height_ - n->right_->height_;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::rotateRight(Node* y)
  {
    Node* x = y->left_;
    Node* t2 = x->right_;
    x->right_ = y;
    y->left_ = t2;
    if (!t2->isNil())
    {
      t2->parent_ = y;
    }
    Node* parentNode = y->parent_;
    x->parent_ = parentNode;
    y->parent_ = x;
    if (parentNode == root_)
    {
      root_->right_ = x;
    }
    else if (y == parentNode->left_)
    {
      parentNode->left_ = x;
    }
    else
    {
      parentNode->right_ = x;
    }
    y->updateHeight();
    x->updateHeight();
    return x;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::rotateLeft(Node* x)
  {
    Node* y = x->right_;
    Node* t2 = y->left_;
    y->left_ = x;
    x->right_ = t2;
    if (!t2->isNil())
    {
      t2->parent_ = x;
    }
    Node* parentNode = x->parent_;
    y->parent_ = parentNode;
    x->parent_ = y;
    if (parentNode == root_)
    {
      root_->right_ = y;
    }
    else if (x == parentNode->left_)
    {
      parentNode->left_ = y;
    }
    else
    {
      parentNode->right_ = y;
    }
    x->updateHeight();
    y->updateHeight();
    return y;
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::balance(Node* z)
  {
    z->updateHeight();
    int bal = getBalance(z);
    if (bal > 1)
    {
      if (getBalance(z->left_) >= 0)
      {
        return rotateRight(z);
      }
      else
      {
        z->left_ = rotateLeft(z->left_);
        return rotateRight(z);
      }
    }
    if (bal < -1)
    {
      if (getBalance(z->right_) <= 0)
      {
        return rotateLeft(z);
      }
      else
      {
        z->right_ = rotateRight(z->right_);
        return rotateLeft(z);
      }
    }
    return z;
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::insertAndBalance(Node* n, Node* parent, const Key& k)
  {
    if (parent == root_)
    {
      root_->right_ = n;
    }
    else if (comp_(k, parent->key_))
    {
      parent->left_ = n;
    }
    else
    {
      parent->right_ = n;
    }
    ++size_;
    Node* fix = n;
    while (fix != nullptr && !fix->isNil() && fix != root_)
    {
      fix = balance(fix);
      fix = fix->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(const Key& k, const Value& v)
  {
    Node* parent = root_;
    Node* cur = root_->right_;
    while (cur != nullptr && !cur->isNil())
    {
      parent = cur;
      if (comp_(k, cur->key_))
      {
        cur = cur->left_;
      }
      else if (comp_(cur->key_, k))
      {
        cur = cur->right_;
      }
      else
      {
        cur->value_ = v;
        return;
      }
    }
    Node* n = new Node(k, v, parent);
    insertAndBalance(n, parent, k);
  }

  template< class Key, class Value, class Compare >
  void AVLTree< Key, Value, Compare >::push(Key&& k, Value&& v)
  {
    Node* parent = root_;
    Node* cur = root_->right_;
    while (cur != nullptr && !cur->isNil())
    {
      parent = cur;
      if (comp_(k, cur->key_))
      {
        cur = cur->left_;
      }
      else if (comp_(cur->key_, k))
      {
        cur = cur->right_;
      }
      else
      {
        cur->value_ = std::forward< Value >(v);
        return;
      }
    }
    Key keyCopy(k);
    Node* n = new Node(std::forward< Key >(k), std::forward< Value >(v), parent);
    insertAndBalance(n, parent, keyCopy);
  }

  template< class Key, class Value, class Compare >
  typename AVLTree< Key, Value, Compare >::Node*
  AVLTree< Key, Value, Compare >::findMinNode(Node* node) const
  {
    Node* cur = node;
    while (!cur->left_->isNil())
    {
      cur = cur->left_;
    }
    return cur;
  }

  template< class Key, class Value, class Compare >
  Value AVLTree< Key, Value, Compare >::drop(const Key& k)
  {
    Node* target = findNode(k);
    if (target == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    Value res = target->value_;
    Node* removable = nullptr;
    if (target->left_->isNil() || target->right_->isNil())
    {
      removable = target;
    }
    else
    {
      removable = findMinNode(target->right_);
    }
    Node* child = nullptr;
    if (!removable->left_->isNil())
    {
      child = removable->left_;
    }
    else
    {
      child = removable->right_;
    }
    child->parent_ = removable->parent_;
    if (removable->parent_ == root_)
    {
      root_->right_ = child;
    }
    else if (removable == removable->parent_->left_)
    {
      removable->parent_->left_ = child;
    }
    else
    {
      removable->parent_->right_ = child;
    }
    if (removable != target)
    {
      target->key_ = removable->key_;
      target->value_ = std::move(removable->value_);
    }
    Node* fixStart = removable->parent_;
    if (fixStart == root_)
    {
      fixStart = child->isNil() ? nullptr : child;
    }
    delete removable;
    --size_;
    while (fixStart != nullptr && !fixStart->isNil() && fixStart != root_)
    {
      fixStart = balance(fixStart);
      fixStart = fixStart->parent_;
    }
    return res;
  }

  template< class Key, class Value, class Compare >
  template< class Func >
  void AVLTree< Key, Value, Compare >::traverseInOrder(Func f) const
  {
    if (root_ == nullptr || root_->right_ == nullptr || root_->right_->isNil())
    {
      return;
    }
    List< Node* > stack;
    Node* cur = root_->right_;
    while ((cur != nullptr && !cur->isNil()) || !stack.empty())
    {
      while (cur != nullptr && !cur->isNil())
      {
        stack.pushFront(cur);
        cur = cur->left_;
      }
      cur = stack.front();
      stack.popFront();
      f(cur->key_, cur->value_);
      cur = cur->right_;
    }
  }
}

#endif
