#ifndef MY_ITER_TREE_HPP
#define MY_ITER_TREE_HPP

#include "my_citer_tree.hpp"
#include "my_node_tree.hpp"
namespace zubarev
{

  template< class Key, class Value >
  class BSTIterator
  {
    friend class BSTConstIterator< Key, Value >;
    using Node = BSTreeNode< Key, Value >;

  private:
    Node* ptr_;

  public:
    BSTIterator() noexcept;
    BSTIterator(Node*);

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    BSTIterator& operator++();
    bool operator!=(const BSTIterator&) const;
    bool operator==(const BSTIterator&) const;
  };

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator() noexcept:
    ptr_(nullptr)
  {}

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node* node):
    ptr_(node)
  {}

  template< class Key, class Value >
  std::pair< Key, Value >& BSTIterator< Key, Value >::operator*() const
  {
    if (!ptr_) {
      throw std::out_of_range("Dereference or increment of end() iterator");
    }
    return ptr_->data_;
  }

  template< class Key, class Value >
  std::pair< Key, Value >* BSTIterator< Key, Value >::operator->() const
  {
    if (!ptr_) {
      throw std::out_of_range("Dereference or increment of end() iterator");
    }
    return &ptr_->data_;
  }
  template< class Key, class Value >
  BSTIterator< Key, Value >& BSTIterator< Key, Value >::operator++()
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
  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator& rhs) const
  {
    return !(*this == rhs);
  }
  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator& rhs) const
  {
    return ptr_ == rhs.ptr_;
  }
  template< class Key, class Value >
  zubarev::BSTConstIterator< Key, Value >::BSTConstIterator(const BSTIterator< Key, Value >& other) noexcept:
    ptr_(other.ptr_)
  {}

}

#endif
