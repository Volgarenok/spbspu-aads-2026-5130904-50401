#ifndef TREEITERS_HPP
#define TREEITERS_HPP
#include <cassert>
#include <memory>
#include "node.hpp"

namespace vasyakin
{
  template< class Key, class Value >
  class AVLConstIterator;

  template< class Key, class Value >
  class AVLIterator
  {
  public:
    using Node = vasyakin::detail::Node< Key, Value >;

    AVLIterator() noexcept;
    explicit AVLIterator(Node* node) noexcept;

    std::pair< const Key, Value >& operator*() const;
    std::pair< const Key, Value >* operator->() const;

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
  };

  template< class Key, class Value >
  class AVLConstIterator
  {
  public:
    using Node = vasyakin::detail::Node< Key, Value >;

    AVLConstIterator() noexcept;
    explicit AVLConstIterator(const Node* node) noexcept;

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;

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
  std::pair< const Key, Value >& AVLIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->value_;
  }

  template< class Key, class Value >
  std::pair< const Key, Value >* AVLIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->value_);
  }

  template< class Key, class Value >
  void AVLIterator< Key, Value >::findNext() noexcept
  {
    return vasyakin::detail::findNextNode(node_);
  }

  template< class Key, class Value >
  void AVLIterator< Key, Value >::findPrev() noexcept
  {
    return vasyakin::detail::findPrevNode(node_);
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
  const std::pair< const Key, Value >& AVLConstIterator< Key, Value >::operator*() const
  {
    assert(node_ != nullptr);
    return node_->value_;
  }

  template< class Key, class Value >
  const std::pair< const Key, Value >* AVLConstIterator< Key, Value >::operator->() const
  {
    assert(node_ != nullptr);
    return std::addressof(node_->value_);
  }

  template< class Key, class Value >
  void AVLConstIterator< Key, Value >::findNext() noexcept
  {
    return vasyakin::detail::findNextNode(node_);
  }

  template< class Key, class Value >
  void AVLConstIterator< Key, Value >::findPrev() noexcept
  {
    vasyakin::detail::findPrevNode(node_);
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
