#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include "BSTIt.hpp"

namespace donkeev
{
  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using constIterator = BSTCIterator< Key, Value >;

    BSTree();

    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);

    BSTree(const Key, const Value&);
    BSTree(const Key, Value&&);

    ~BSTree();

    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);

    void push(Key, const Value&);
    void push(Key, Value&&);
    Value& get(const Key);
    BSTNode< Key, Value >* find(const Key);
    Value drop(const Key);
    void clear();
    void swap(BSTree< Key, Value, Compare >&);

  private:
    BSTNode< Key, Value >* root_;
    size_t size_;
    Compare compareFunc_;

    BSTNode< Key, Value >* cloneRecursive(const BSTNode< Key, Value >*, BSTNode< Key, Value >*);
    void removeNode(BSTNode< Key, Value >*);
    void clearRecursive(BSTNode< Key, Value >*);
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

    BSTNode< Key, Value >* curr = cloneRecursive(other.root_, nullptr);
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
  BSTree< Key, Value, Compare >::BSTree(const Key key, const Value& value):
    root_(nullptr),
    size_(0),
    compareFunc_()
  {
    root_ = new BSTNode< Key, Value >{
      {key, value},
      nullptr,
      nullptr,
      nullptr
    };
    ++size_;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const Key key, Value&& value):
    root_(nullptr),
    size_(0),
    compareFunc_()
  {
    root_ = new BSTNode< Key, Value >{
      {key, std::move(value)},
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
    if (this != &other)
    {
      BSTree< Key, Value, Compare > newTree(other);
      swap(newTree);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >& BSTree< Key, Value, Compare >::operator=(BSTree< Key, Value, Compare >&& other)
  {
    if (this != &other)
    {
      BSTree< Key, Value, Compare > newTree(std::move(other));
      swap(newTree);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key key, const Value& value)
  {
    if (!root_)
    {
      root_ = new BSTNode<Key, Value>{
        {key, value},
        nullptr,
        nullptr,
        nullptr
      };
      ++size_;
      return;
    }

    BSTNode<Key, Value>* curr = root_;
    BSTNode<Key, Value>* parent = nullptr;

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
        throw std::runtime_error("Key already exists");
      }
    }

    BSTNode<Key, Value>* new_node = new BSTNode<Key, Value>{
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
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(Key key, Value&& value)
  {
    if (!root_)
    {
      root_ = new BSTNode<Key, Value>{
        {key, std::move(value)},
        nullptr,
        nullptr,
        nullptr
      };
      ++size_;
      return;
    }

    BSTNode<Key, Value>* curr = root_;
    BSTNode<Key, Value>* parent = nullptr;

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
        throw std::runtime_error("Invalid key");
      }
    }

    BSTNode<Key, Value>* new_node = new BSTNode<Key, Value>{
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
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::get(const Key key)
  {
    BSTNode< Key, Value >* curr = root_;
    while (curr)
    {
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
        return curr->data_.second;
      }
    }

    throw std::runtime_error("No such element");
  }

  template<class Key, class Value, class Compare>
  Value BSTree<Key, Value, Compare>::drop(const Key key)
  {
    BSTNode<Key, Value>* node = findNode(key);
    if (!node)
    {
      throw std::runtime_error("No such element");
    }
    
    Value result = std::move(node->data_.second);
    
    removeNode(node);
    --size_;

    return result;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree< Key, Value, Compare >& other)
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
  }

  template<class Key, class Value, class Compare>
  void BSTree< Key, Value, Compare >::clear()
  {
    clearRecursive(root_);
    size_ = 0;
  }

  template< class Key, class Value, class Compare >
  BSTNode< Key, Value >* BSTree< Key, Value, Compare >::cloneRecursive(const BSTNode< Key, Value >* node, BSTNode< Key, Value >* parent)
  {
    if (!node)
    {
      return nullptr;
    }

    BSTNode< Key, Value >* new_node = new BSTNode< Key, Value >{
      {node->data.first, node->data.second},
      nullptr,
      nullptr,
      nullptr
    };
    new_node->parent = parent;
    new_node->left = cloneRecursive(node->left, new_node);
    new_node->right = cloneRecursive(node->right, new_node);
    
    return new_node;
  }

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::removeNode(BSTNode<Key, Value>* node)
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
      BSTNode<Key, Value>* child = node->right_;
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
      BSTNode<Key, Value>* child = node->left_;
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
      BSTNode<Key, Value>* minNode = node->right_;
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

  template<class Key, class Value, class Compare>
  void BSTree<Key, Value, Compare>::clearRecursive(BSTNode< Key, Value >* node)
  {
    if (!node)
    {
      return;
    }

    clearRecursive(node->left_);
    clearRecursive(node->right_);

    delete node;
  }
}

#endif
