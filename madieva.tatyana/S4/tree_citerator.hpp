#ifndef TREE_CITERATOR_HPP
#define TREE_CITERATOR_HPP

#include "bstree.hpp"

namespace madieva {
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class TCIter{
    const Node< Key, Value, Compare > * it_;
    friend class BSTree< Key, Value, Compare >;
  public:
    TCIter(const Node< Key, Value, Compare > * root);
    const TCIter & operator++();
    const std::pair< Key, Value > & operator*() const;
    bool operator!=(const TCIter< Key, Value, Compare > & other) const;
    bool operator==(const TCIter< Key, Value, Compare > & other) const;
  };

  template< class Key, class Value, class Compare >
  TCIter< Key, Value, Compare >::TCIter(const Node< Key, Value, Compare > * root) :
    it_(root)
  {}

  template< class Key, class Value, class Compare >
  const TCIter< Key, Value, Compare > &
    TCIter< Key, Value, Compare >::operator++()
  {
    if (!it_) {
      return *this;
    }
    if (it_->right) {
      it_ = it_->right;
      while (it_->left) {
        it_ = it_->left;
      }
      return *this;
    }
    const Node< Key, Value, Compare > * child = it_;
    it_ = it_->parent;
    while (it_ && it_->right == child) {
      child = it_;
      it_ = it_->parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  const std::pair< Key, Value > & TCIter< Key, Value, Compare >::operator*() const
  {
    if (!it_) {
      throw std::logic_error("Dereferencing null iterator");
    }
    return it_->data;
  }

  template< class Key, class Value, class Compare >
  bool TCIter< Key, Value, Compare >::operator!=(const TCIter< Key, Value, Compare > & other) const
  {
    return it_ != other.it_;
  }

  template< class Key, class Value, class Compare >
  bool TCIter< Key, Value, Compare >::operator==(const TCIter< Key, Value, Compare > & other) const
  {
    return it_ == other.it_;
  }

}

#endif
