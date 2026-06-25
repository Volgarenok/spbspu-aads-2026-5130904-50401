#ifndef TREE_ITERATOR_HPP
#define TREE_ITERATOR_HPP

#include "bstree.hpp"

namespace madieva {
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class TIter{
    Node< Key, Value, Compare > * it_;
    friend class BSTree< Key, Value, Compare >;
  public:
    TIter(Node< Key, Value, Compare > * root);
    TIter & operator++();
    std::pair< Key, Value > & operator*();
    bool operator!=(const TIter< Key, Value, Compare > & other) const;
    bool operator==(const TIter< Key, Value, Compare > & other) const;
  };

  template< class Key, class Value, class Compare >
  TIter< Key, Value, Compare >::TIter(Node< Key, Value, Compare > * root) :
    it_(root)
  {}

  template< class Key, class Value, class Compare >
  TIter< Key, Value, Compare > &
    TIter< Key, Value, Compare >::operator++()
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
    Node< Key, Value, Compare > * child = it_;
    it_ = it_->parent;
    while (it_ && it_->right == child) {
      child = it_;
      it_ = it_->parent;
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  std::pair< Key, Value > & TIter< Key, Value, Compare >::operator*()
  {
    if (!it_) {
      throw std::logic_error("Dereferencing null iterator");
    }
    return it_->data;
  }

  template< class Key, class Value, class Compare >
  bool TIter< Key, Value, Compare >::operator!=(const TIter< Key, Value, Compare > & other) const
  {
    return it_ != other.it_;
  }

  template< class Key, class Value, class Compare >
  bool TIter< Key, Value, Compare >::operator==(const TIter< Key, Value, Compare > & other) const
  {
    return it_ == other.it_;
  }

}

#endif
