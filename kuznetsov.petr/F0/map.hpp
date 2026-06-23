#ifndef KUZNETSOV_MAP_HPP
#define KUZNETSOV_MAP_HPP
#include <cstddef>
#include <functional>
#include <BSTree.hpp>

namespace kuznetsov {
  template< class Key, class Value, class Compare = std::less< Key > >
  struct map {
    using tree_t = BSTree< Key, Value, Compare >;
    using iterator = typename tree_t::iterator;
    using const_iterator = typename tree_t::const_iterator;

    void insert(const Key& k, const Value& v);
    void insert(const Key& k, Value&& v);
    Value& at(const Key& k);
    const Value& at(const Key& k) const;
    bool contains(const Key& k) const noexcept;
    void erase(const Key& k);
    size_t size() const noexcept;
    bool empty() const noexcept;
    size_t height() const noexcept;

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

  private:
    tree_t tree_;
    void rebalanceAt(const_iterator z);
    void balanceSubtree(const_iterator it);
    void balance();
  };
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::insert(const Key& k, const Value& v)
{
  if (tree_.contain(k)) {
    tree_.at(k) = v;
    return;
  }
  tree_.push(k, v);
  balance();
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::insert(const Key& k, Value&& v)
{
  if (tree_.contain(k)) {
    tree_.at(k) = v;
    return;
  }
  tree_.push(k, std::forward< Value >(v));
  balance();
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::balance()
{
  balanceSubtree(tree_.root());
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::balanceSubtree(const_iterator it)
{
  if (it == tree_.cend()) {
    return;
  }
  balanceSubtree(tree_.leftChild(it));
  balanceSubtree(tree_.rightChild(it));
  rebalanceAt(it);
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::rebalanceAt(const_iterator z)
{
  long long lh = tree_.height(tree_.leftChild(z));
  long long rh = tree_.height(tree_.rightChild(z));
  if (lh - rh > 1) {
    const_iterator l = tree_.leftChild(z);
    long long llh = tree_.height(tree_.leftChild(l));
    long long lrh = tree_.height(tree_.rightChild(l));
    if (llh >= lrh) {
      tree_.rotateRight(l);
    } else {
      tree_.rotateLargeRight(tree_.rightChild(l));
    }
  } else if (rh - lh > 1) {
    const_iterator r = tree_.rightChild(z);
    long rlh = static_cast< long >(tree_.height(tree_.leftChild(r)));
    long rrh = static_cast< long >(tree_.height(tree_.rightChild(r)));
    if (rrh >= rlh) {
      tree_.rotateLeft(r);
    } else {
      tree_.rotateLargeLeft(tree_.leftChild(r));
    }
  }
}

template< class Key, class Value, class Compare >
Value& kuznetsov::map< Key, Value, Compare >::at(const Key& k)
{
  return tree_.at(k);
}

template< class Key, class Value, class Compare >
const Value& kuznetsov::map< Key, Value, Compare >::at(const Key& k) const
{
  return tree_.at(k);
}

template< class Key, class Value, class Compare >
bool kuznetsov::map< Key, Value, Compare >::contains(const Key& k) const noexcept
{
  return tree_.contain(k);
}

template< class Key, class Value, class Compare >
void kuznetsov::map< Key, Value, Compare >::erase(const Key& k)
{
  tree_.drop(k);
  balance();
}

template< class Key, class Value, class Compare >
size_t kuznetsov::map< Key, Value, Compare >::size() const noexcept
{
  return tree_.size();
}

template< class Key, class Value, class Compare >
bool kuznetsov::map< Key, Value, Compare >::empty() const noexcept
{
  return tree_.empty();
}

template< class Key, class Value, class Compare >
size_t kuznetsov::map< Key, Value, Compare >::height() const noexcept
{
  return tree_.height();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::iterator kuznetsov::map< Key, Value, Compare >::begin()
{
  return tree_.begin();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::iterator kuznetsov::map< Key, Value, Compare >::end()
{
  return tree_.end();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::const_iterator kuznetsov::map< Key, Value, Compare >::begin() const
{
  return tree_.cbegin();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::const_iterator kuznetsov::map< Key, Value, Compare >::end() const
{
  return tree_.cend();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::const_iterator kuznetsov::map< Key, Value, Compare >::cbegin() const
{
  return tree_.cbegin();
}

template< class Key, class Value, class Compare >
typename kuznetsov::map< Key, Value, Compare >::const_iterator kuznetsov::map< Key, Value, Compare >::cend() const
{
  return tree_.cend();
}

#endif
