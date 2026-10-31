#ifndef BOUNDED_PRIORITY_QUEUE_HPP
#define BOUNDED_PRIORITY_QUEUE_HPP

#include "MaxHeap.hpp"
#include "Vector.hpp"
#include <cassert>
#include <cstddef>
#include <utility>

namespace khasnulin
{
  // Compare(const T& a, const T& b), a лучше b; хранит limit лучших
  template < class T, class Compare > class BoundedPriorityQueue
  {
  public:
    using size_type = std::size_t;
    using value_compare = Compare;
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;

    explicit BoundedPriorityQueue(size_type limit, const Compare &better);

    // попал кандидат в отбор или нет. true -- попал
    bool suggest(const T &val);
    bool suggest(T &&val);

    size_type limit() const noexcept;
    size_type size() const noexcept;
    bool empty() const noexcept;

    void clear();

    // F(const T& val)
    template < class F > void traverseAndClear(F f);

  private:
    template < class Val > bool commonSuggest(Val &&val);

    size_type limit_;
    MaxHeap< T, Compare > heap_;
  };

  template < class T, class Compare >
  BoundedPriorityQueue< T, Compare >::BoundedPriorityQueue(size_type limit, const Compare &better):
      limit_(limit),
      heap_(better)
  {
    assert(limit > 0);
  }

  template < class T, class Compare >
  template < class Val >
  bool BoundedPriorityQueue< T, Compare >::commonSuggest(Val &&val)
  {
    if (heap_.size() < limit_)
    {
      heap_.push(std::forward< Val >(val));
      return true;
    }
    if (heap_.valueComp()(val, heap_.top()))
    {
      heap_.push(std::forward< Val >(val));
      heap_.pop();
      return true;
    }
    return false;
  }

  template < class T, class Compare > bool BoundedPriorityQueue< T, Compare >::suggest(const T &val)
  {
    return commonSuggest(val);
  }

  template < class T, class Compare > bool BoundedPriorityQueue< T, Compare >::suggest(T &&val)
  {
    return commonSuggest(std::move(val));
  }

  template < class T, class Compare >
  typename BoundedPriorityQueue< T, Compare >::size_type
  BoundedPriorityQueue< T, Compare >::limit() const noexcept
  {
    return limit_;
  }

  template < class T, class Compare >
  typename BoundedPriorityQueue< T, Compare >::size_type
  BoundedPriorityQueue< T, Compare >::size() const noexcept
  {
    return heap_.size();
  }

  template < class T, class Compare > bool BoundedPriorityQueue< T, Compare >::empty() const noexcept
  {
    return heap_.empty();
  }

  template < class T, class Compare > void BoundedPriorityQueue< T, Compare >::clear()
  {
    MaxHeap< T, Compare > temp(heap_.valueComp());
    heap_.swap(temp);
  }

  template < class T, class Compare >
  template < class F >
  void BoundedPriorityQueue< T, Compare >::traverseAndClear(F f)
  {
    if (empty())
    {
      return;
    }
    Vector< T > vec(heap_.size());
    size_t j = 0;
    while (!heap_.empty())
    {
      vec[j] = heap_.top();
      heap_.pop();
      ++j;
    }
    for (size_type i = vec.getSize(); i > 0; --i)
    {
      f(vec[i - 1]);
    }
  }
}

#endif
