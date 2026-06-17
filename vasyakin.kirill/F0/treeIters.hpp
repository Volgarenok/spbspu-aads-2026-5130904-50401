#ifndef TREEITERS_HPP
#define TREEITERS_HPP
#include <utility>
#include <cassert>
#include "node.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Compare >
  class AVLTree;

  template< class Key, class Value >
  class AVLIterator
  {
    using Node = vasyakin::detail::Node< Key, Value >;

  public:
    AVLIterator() noexcept;
    explicit AVLIterator(Node* node) noexcept;

    std::pair< Key, Value > operator*() const;

    AVLIterator& operator++() noexcept;
    AVLIterator operator++(int) noexcept;

    AVLIterator& operator--() noexcept;
    AVLIterator operator--(int) noexcept;

    bool operator==(const AVLIterator& other) const noexcept;
    bool operator!=(const AVLIterator& other) const noexcept;

    operator AVLConstIterator< Key, Value >() const noexcept;
  private:
    Node* node_;

    void findNext() noexcept;
    void findPrev() noexcept;

    template< class K, class V, class C >
    friend class AVLTree;
  };

  template< class Key, class Value >
  class AVLConstIterator
  {
    using Node = vasyakin::detail::Node< Key, Value >;

  public:
    AVLConstIterator() noexcept;
    explicit AVLConstIterator(const Node* node) noexcept;

    std::pair< Key, Value > operator*() const;

    AVLConstIterator& operator++() noexcept;
    AVLConstIterator operator++(int) noexcept;

    AVLConstIterator& operator--() noexcept;
    AVLConstIterator operator--(int) noexcept;

    bool operator==(const AVLConstIterator& other) const noexcept;
    bool operator!=(const AVLConstIterator& other) const noexcept;
  private:
    const Node* node_;

    void findNext() noexcept;
    void findPrev() noexcept;

    template< class K, class V, class C >
    friend class AVLTree;
  };

  template< class Key, class Value >
  AVLIterator< Key, Value >::AVLIterator() noexcept:
    node_(nullptr)
  {}

  template< class Key, class Value >
  AVLIterator< Key, Value >::AVLIterator(Node* node) noexcept:
    node_(node)
  {}

  template< class Key, class Value >
  std::pair< Key, Value > AVLIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr && "Dereferencing end() iterator");
    return {node_->key_, node_->value_};
  }

  template< class Key, class Value >
  void AVLIterator< Key, Value >::findNext() noexcept
  {
    if (!node_)
    {
      return;
    }

    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      Node* parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent;
    }
  }

  template< class Key, class Value >
  void AVLIterator< Key, Value >::findPrev() noexcept
  {
    if (!node_)
    {
      return;
    }

    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      Node* parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent;
    }
  }

  template< class Key, class Value >
  AVLIterator< Key, Value >& AVLIterator< Key, Value >::operator++() noexcept
  {
    findNext();
    return *this;
  }

  template< class Key, class Value >
  AVLIterator< Key, Value > AVLIterator< Key, Value >::operator++(int) noexcept
  {
    AVLIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value >
  AVLIterator< Key, Value >& AVLIterator< Key, Value >::operator--() noexcept
  {
    findPrev();
    return *this;
  }

  template< class Key, class Value >
  AVLIterator< Key, Value > AVLIterator< Key, Value >::operator--(int) noexcept
  {
    AVLIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value >
  bool AVLIterator< Key, Value >::operator==(const AVLIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool AVLIterator< Key, Value >::operator!=(const AVLIterator& other) const noexcept
  {
    return !(node_ == other.node_);
  }

  template< class Key, class Value >
  AVLIterator< Key, Value >::operator vasyakin::AVLConstIterator< Key, Value >() const noexcept
  {
    return AVLConstIterator< Key, Value >(node_);
  }

  template< class Key, class Value >
  AVLConstIterator< Key, Value >::AVLConstIterator() noexcept:
    node_(nullptr)
  {}

  template< class Key, class Value >
  AVLConstIterator< Key, Value >::AVLConstIterator(const Node* node) noexcept:
    node_(node)
  {}

  template< class Key, class Value >
  std::pair< Key, Value > AVLConstIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr && "Dereferencing end() iterator");
    return {node_->key_, node_->value_};
  }

  template< class Key, class Value >
  void AVLConstIterator< Key, Value >::findNext() noexcept
  {
    if (!node_)
    {
      return;
    }

    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      const Node* parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent;
    }
  }

  template< class Key, class Value >
  void AVLConstIterator< Key, Value >::findPrev() noexcept
  {
    if (!node_)
    {
      return;
    }

    if (node_->left_)
    {
      node_ = node_->left_;
      while (node_->right_)
      {
        node_ = node_->right_;
      }
    }
    else
    {
      const Node* parent = node_->parent_;
      while (parent && node_ == parent->left_)
      {
        node_ = parent;
        parent = parent->parent_;
      }

      node_ = parent;
    }
  }

  template< class Key, class Value >
  AVLConstIterator< Key, Value >& AVLConstIterator< Key, Value >::operator++() noexcept
  {
    findNext();
    return *this;
  }

  template< class Key, class Value >
  AVLConstIterator< Key, Value > AVLConstIterator< Key, Value >::operator++(int) noexcept
  {
    AVLConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  template< class Key, class Value >
  AVLConstIterator< Key, Value >& AVLConstIterator< Key, Value >::operator--() noexcept
  {
    findPrev();
    return *this;
  }

  template< class Key, class Value >
  AVLConstIterator< Key, Value > AVLConstIterator< Key, Value >::operator--(int) noexcept
  {
    AVLConstIterator tmp = *this;
    --(*this);
    return tmp;
  }

  template< class Key, class Value >
  bool AVLConstIterator< Key, Value >::operator==(const AVLConstIterator& other) const noexcept
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  bool AVLConstIterator< Key, Value >::operator!=(const AVLConstIterator& other) const noexcept
  {
    return !(node_ == other.node_);
  }
}

#endif
