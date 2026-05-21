#ifndef MY_CITER_TREE_HPP
#define MY_CITER_TREE_HPP

#include "my_node_tree.hpp"
namespace zubarev
{

  template < class Key, class Value >
  class BSTIterator;

  template < class Key, class Value >
  class BSTConstIterator
  {
    using Node = BSTreeNode< Key, Value >;

  private:
    const Node* ptr_;

  public:
    BSTConstIterator() noexcept;
    BSTConstIterator(const BSTIterator< Key, Value >& other) noexcept;
    BSTConstIterator(const Node*);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;
    BSTConstIterator& operator++();
    bool operator!=(const BSTConstIterator&) const;
    bool operator==(const BSTConstIterator&) const;
  };
  template < class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator() noexcept:
    ptr_(nullptr)
  {}
  template < class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const Node* node):
    ptr_(node)
  {}

  template < class Key, class Value >
  const std::pair< Key, Value >& BSTConstIterator< Key, Value >::operator*() const
  {
    if (!ptr_) {
      throw std::out_of_range("Dereference or increment of end() iterator");
    }
    return ptr_->data_;
  }

  template < class Key, class Value >
  const std::pair< Key, Value >* BSTConstIterator< Key, Value >::operator->() const
  {
    if (!ptr_) {
      throw std::out_of_range("Dereference or increment of end() iterator");
    }
    return &ptr_->data_;
  }
  template < class Key, class Value >
  BSTConstIterator< Key, Value >& BSTConstIterator< Key, Value >::operator++()
  {
    if (!ptr_) {
      throw std::out_of_range("Dereference or increment of end() iterator");
    }
    if (ptr_->right_) {
      ptr_ = ptr_->right_;
      while (ptr_->left_ != nullptr) {
        ptr_ = ptr_->left_;
      }
    } else {
      while (ptr_->parent_ && ptr_->parent_->right_ == ptr_) {
        ptr_ = ptr_->parent_;
      }
      ptr_ = ptr_->parent_;
    }
    return *this;
  }
  template < class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator& rhs) const
  {
    return !(*this == rhs);
  }
  template < class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator& rhs) const
  {
    return ptr_ == rhs.ptr_;
  }

}

#endif
