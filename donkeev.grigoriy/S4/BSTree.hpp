#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include "BSTIt.hpp"

namespace donkeev
{
  template< class T >
  struct Comp
  {
    bool operator()(const T&, const T&) const;
  };

  template< class T >
  bool Comp< T >::operator()(const T& lhs, const T& rhs) const
  {
    return std::less<T>{}(lhs, rhs);
  }

  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using tree = BSTree< Key, Value, Compare >;
    using iterator = BSTIterator< Key, Value >;
    using constIterator = BSTCIterator< Key, Value >;

    BSTree();

    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);

    BSTree(const Key&, const Value&);
    BSTree(const Key&&, Value&&);

    ~BSTree();

    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);
    Value& operator[](const Key&);
    Value& operator[](Key&&);

    Value& at(const Key&);
    const Value& at(const Key&) const;
    iterator find(const Key&);
    constIterator find(const Key&) const;

    iterator begin();
    iterator end();
    constIterator begin() const;
    constIterator end() const;

    iterator rotateRight(iterator);
    iterator rotateLeft(iterator);
    iterator largeRotateRight(iterator);
    iterator largeRotateLeft(iterator);
    std::pair< iterator, bool > insert(const Key&, const Value&);
    std::pair< iterator, bool > insert(Key&&, Value&&);
    bool erase(const Key&);
    void clear();
    void swap(BSTree< Key, Value, Compare >&);
    bool empty() const;
    size_t size() const;
    size_t height() const;
    size_t height(constIterator) const;

  private:
    detail::BSTNode< Key, Value >* root_;
    size_t size_;
    Compare compareFunc_;

    detail::BSTNode< Key, Value >* cloneRecursive(
      const detail::BSTNode< Key, Value >*, detail::BSTNode< Key, Value >*);
    void removeNode(detail::BSTNode< Key, Value >*);
    void clearRecursive(detail::BSTNode< Key, Value >*);
    detail::BSTNode< Key, Value >* findNode(const Key) const;
    size_t heightRecursive(const detail::BSTNode<Key, Value>*) const;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    root_(nullptr),
    size_(0),
    compareFunc_()
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree< Key, Value, Compare >& other):
    root_(nullptr),
    size_(0),
    compareFunc_()
  {
    if (other.size_ == 0)
    {
      return;
    }

    detail::BSTNode< Key, Value >* curr = cloneRecursive(other.root_, nullptr);
    root_ = curr;
    size_ = other.size_;
    compareFunc_ = other.compareFunc_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(BSTree< Key, Value, Compare >&& other):
    root_(other.root_),
    size_(other.size_),
    compareFunc_(other.compareFunc_)
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const Key& key, const Value& value):
    root_(nullptr),
    size_(0),
    compareFunc_()
  {
    root_ = new detail::BSTNode< Key, Value >{
      {key, value},
      nullptr,
      nullptr,
      nullptr
    };
    ++size_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const Key&& key, Value&& value):
    root_(nullptr),
    size_(0),
    compareFunc_()
  {
    root_ = new detail::BSTNode< Key, Value >{
      {std::move(key), std::move(value)},
      nullptr,
      nullptr,
      nullptr
    };
    ++size_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(const BSTree< Key, Value, Compare >& other)
  {
    if (this != std::addressof(other))
    {
      BSTree< Key, Value, Compare > newTree(other);
      swap(newTree);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree< Key, Value, Compare >&& other)
  {
    if (this != std::addressof(other))
    {
      BSTree< Key, Value, Compare > newTree(std::move(other));
      swap(newTree);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](const Key& key)
  {
    std::pair< iterator, bool > result = insert(key, Value{});

    return result.first->second;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::operator[](Key&& key)
  {
    std::pair< iterator, bool > result = insert(std::move(key), Value{});

    return result.first->second;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::at(const Key& key)
  {
    detail::BSTNode< Key, Value >* node = findNode(key);
    if (!node)
    {
      throw std::out_of_range("No such element");
    }
    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::at(const Key& key) const
  {
    detail::BSTNode< Key, Value >* node = findNode(key);
    if (!node)
    {
      throw std::out_of_range("No such element");
    }
    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::find(const Key& key)
  {
    detail::BSTNode< Key, Value >* node = findNode(key);
    return iterator(node);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::constIterator BSTree< Key, Value, Compare >::find(const Key& key) const
  {
    detail::BSTNode< Key, Value >* node = findNode(key);
    return constIterator(node);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::begin()
  {
    return iterator(fallLeft(root_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::end()
  {
    return iterator(nullptr);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::constIterator BSTree< Key, Value, Compare >::begin() const
  {
    return constIterator(fallLeft(root_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::constIterator BSTree< Key, Value, Compare >::end() const
  {
    return constIterator(nullptr);
  }

  template<class Key, class Value, class Compare>
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::rotateRight(iterator it)
  {
    detail::BSTNode< Key, Value >* parent = it.getNode();
    if (!parent || !parent->left_)
    {
      return it;
    }

    detail::BSTNode< Key, Value >* leftChild = parent->left_;
    detail::BSTNode< Key, Value >* rightGrandSon = leftChild->right_;
    detail::BSTNode< Key, Value >* grandFather = parent->parent_;

    leftChild->parent_ = grandFather;
    if (grandFather)
    {
      if (grandFather->left_ == parent)
      {
        grandFather->left_ = leftChild;
      }
      else
      {
        grandFather->right_ = leftChild;
      }
    }
    else
    {
      root_ = leftChild;
    }

    leftChild->right_ = parent;
    parent->parent_ = leftChild;

    parent->left_ = rightGrandSon;
    if (rightGrandSon)
    {
      rightGrandSon->parent_ = parent;
    }

    return iterator(leftChild);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::rotateLeft(iterator it)
  {
    detail::BSTNode< Key, Value >* parent = it.getNode();
    if (!parent || !parent->right_)
    {
      return it;
    }

    detail::BSTNode< Key, Value >* rightChild = parent->right_;
    detail::BSTNode< Key, Value >* leftGrandSon = rightChild->left_;
    detail::BSTNode< Key, Value >* grandFather = parent->parent_;

    rightChild->parent_ = grandFather;
    if (grandFather)
    {
      if (grandFather->left_ == parent)
      {
        grandFather->left_ = rightChild;
      }
      else
      {
        grandFather->right_ = rightChild;
      }
    }
    else
    {
      root_ = rightChild;
    }

    rightChild->left_ = parent;
    parent->parent_ = rightChild;

    parent->right_ = leftGrandSon;
    if (leftGrandSon)
    {
      leftGrandSon->parent_ = parent;
    }

    return iterator(rightChild);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::largeRotateRight(iterator it)
  {
    detail::BSTNode< Key, Value >* parent = it.getNode();
    if (!parent || !parent->left_)
    {
      return it;
    }

    iterator leftIt(parent->left_);
    rotateLeft(leftIt);

    return rotateRight(it);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::largeRotateLeft(iterator it)
  {
    detail::BSTNode< Key, Value >* parent = it.getNode();
    if (!parent || !parent->right_)
    {
      return it;
    }

    iterator rightIt(parent->right_);
    rotateRight(rightIt);

    return rotateLeft(it);
  }

  template< class Key, class Value, class Compare >
  std::pair< BSTIterator< Key, Value >, bool >  BSTree< Key, Value, Compare >::insert(
    const Key& key, const Value& value)
  {
    if (!root_)
    {
      root_ = new detail::BSTNode< Key, Value >{
        {key, value},
        nullptr,
        nullptr,
        nullptr
      };
      ++size_;
      return std::make_pair(iterator(root_), true);
    }

    detail::BSTNode< Key, Value >* curr = root_;
    detail::BSTNode< Key, Value >* parent = nullptr;

    while (curr)
    {
      parent = curr;

      if (compareFunc_(key, curr->data_.first))
      {
        curr = curr->left_;
      }
      else if (compareFunc_(curr->data_.first, key))
      {
        curr = curr->right_;
      }
      else
      {
        return std::make_pair(iterator(curr), false);
      }
    }

    detail::BSTNode< Key, Value >* new_node = new detail::BSTNode< Key, Value >{
      {key, value},
      nullptr,
      nullptr,
      parent
    };

    if (compareFunc_(key, parent->data_.first))
    {
      parent->left_ = new_node;
    }
    else
    {
      parent->right_ = new_node;
    }

    ++size_;
    return std::make_pair(iterator(new_node), true);
  }

  template< class Key, class Value, class Compare >
  std::pair< BSTIterator< Key, Value >, bool > BSTree< Key, Value, Compare >::insert(
    Key&& key, Value&& value)
  {
    if (!root_)
    {
      root_ = new detail::BSTNode< Key, Value >{
        {key, std::move(value)},
        nullptr,
        nullptr,
        nullptr
      };
      ++size_;
      return std::make_pair(iterator(root_), true);
    }

    detail::BSTNode< Key, Value >* curr = root_;
    detail::BSTNode< Key, Value >* parent = nullptr;

    while (curr)
    {
      parent = curr;

      if (compareFunc_(key, curr->data_.first))
      {
        curr = curr->left_;
      }
      else if (compareFunc_(curr->data_.first, key))
      {
        curr = curr->right_;
      }
      else
      {
        return std::make_pair(iterator(curr), false);
      }
    }

    detail::BSTNode< Key, Value >* new_node = new detail::BSTNode< Key, Value >{
        {key, std::move(value)},
        nullptr,
        nullptr,
        parent
    };

    if (compareFunc_(key, parent->data_.first))
    {
        parent->left_ = new_node;
    }
    else
    {
        parent->right_ = new_node;
    }

    ++size_;
    return std::make_pair(iterator(new_node), true);
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::erase(const Key& key)
  {
    detail::BSTNode< Key, Value >* node = findNode(key);
    if (!node)
    {
      return false;
    }

    removeNode(node);
    --size_;

    return true;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree< Key, Value, Compare >& other)
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    clearRecursive(root_);
    size_ = 0;
    root_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return heightRecursive(root_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(constIterator it) const
  {
    detail::BSTNode< Key, Value >* node = it.getNode();
    return heightRecursive(node);
  }

  template< class Key, class Value, class Compare >
  detail::BSTNode< Key, Value >* BSTree< Key, Value, Compare >::cloneRecursive(
    const detail::BSTNode< Key, Value >* node, detail::BSTNode< Key, Value >* parent)
  {
    if (!node)
    {
      return nullptr;
    }

    detail::BSTNode< Key, Value >* new_node = new detail::BSTNode< Key, Value >{
      {node->data_.first, node->data_.second},
      nullptr,
      nullptr,
      nullptr
    };
    new_node->parent_ = parent;
    new_node->left_ = cloneRecursive(node->left_, new_node);
    new_node->right_ = cloneRecursive(node->right_, new_node);

    return new_node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::removeNode(detail::BSTNode< Key, Value >* node)
  {
    if (!node)
    {
      return;
    }

    if (!node->left_ && !node->right_)
    {
      if (node->parent_)
      {
        if (node->parent_->left_ == node)
        {
          node->parent_->left_ = nullptr;
        }
        else
        {
          node->parent_->right_ = nullptr;
        }
      }
      else
      {
        root_ = nullptr;
      }

      delete node;
    }
    else if (!node->left_)
    {
      detail::BSTNode< Key, Value >* child = node->right_;
      child->parent_ = node->parent_;

      if (node->parent_)
      {
        if (node->parent_->left_ == node)
        {
          node->parent_->left_ = child;
        }
        else
        {
          node->parent_->right_ = child;
        }
      }
      else
      {
        root_ = child;
      }

      delete node;
    }
    else if (!node->right_)
    {
      detail::BSTNode< Key, Value >* child = node->left_;
      child->parent_ = node->parent_;

      if (node->parent_)
      {
        if (node->parent_->left_ == node)
        {
          node->parent_->left_ = child;
        }
        else
        {
          node->parent_->right_ = child;
        }
      }
      else
      {
        root_ = child;
      }

      delete node;
    }
    else
    {
      detail::BSTNode< Key, Value >* minNode = node->right_;
      while (minNode->left_)
      {
        minNode = minNode->left_;
      }

      if (minNode->parent_->left_ == minNode)
      {
        minNode->parent_->left_ = minNode->right_;
        if (minNode->right_)
        {
          minNode->right_->parent_ = minNode->parent_;
        }
      }
      else
      {
        minNode->parent_->right_ = minNode->right_;
        if (minNode->right_)
        {
          minNode->right_->parent_ = minNode->parent_;
        }
      }

      minNode->left_ = node->left_;
      minNode->right_ = node->right_;
      if (minNode->left_)
      {
        minNode->left_->parent_ = minNode;
      }
      if (minNode->right_)
      {
        minNode->right_->parent_ = minNode;
      }
      minNode->parent_ = node->parent_;
      if (node->parent_)
      {
        if (node->parent_->left_ == node)
        {
          node->parent_->left_ = minNode;
        }
        else
        {
          node->parent_->right_ = minNode;
        }
      }

      delete node;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clearRecursive(detail::BSTNode< Key, Value >* node)
  {
    if (!node)
    {
      return;
    }

    clearRecursive(node->left_);
    clearRecursive(node->right_);

    delete node;
  }

  template< class Key, class Value, class Compare >
  detail::BSTNode< Key, Value >* BSTree< Key, Value, Compare >::findNode(const Key key) const
  {
    detail::BSTNode< Key, Value >* current = root_;

    while (current)
    {
      if (compareFunc_(key, current->data_.first))
      {
        current = current->left_;
      }
      else if (compareFunc_(current->data_.first, key))
      {
        current = current->right_;
      }
      else
      {
        return current;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::heightRecursive(const detail::BSTNode< Key, Value >* node) const
  {
    if (!node)
    {
      return 0;
    }

    size_t leftHeight = heightRecursive(node->left_);
    size_t rightHeight = heightRecursive(node->right_);

    return 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);
  }
}

#endif
