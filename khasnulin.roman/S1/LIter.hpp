#ifndef LITER_HPP
#define LITER_HPP

#include <iostream>
#include <stdexcept>

#include "LCIter.hpp"

namespace khasnulin
{
  template < class T > class BiList;

  template < class T > class LIter
  {
  public:
    LIter() noexcept;

    LIter< T > &operator++();
    LIter< T > operator++(int);

    LIter< T > &operator--();
    LIter< T > operator--(int);

    bool operator==(const LIter< T > &it) const noexcept;
    bool operator!=(const LIter< T > &it) const noexcept;

    T &operator*();
    T *operator->();

    friend class BiList< T >;
    friend class LCIter< T >;

  private:
    using LNode = typename BiList< T >::template LNode< T >;

    LIter(const BiList< T > *list, LNode *node, bool is_end):
        list_(list),
        curr_(node),
        is_end_(is_end) {};

    const BiList< T > *list_;
    LNode *curr_;
    bool is_end_;
  };

  template < class T >
  LIter< T >::LIter() noexcept:
      list_(nullptr),
      curr_(nullptr),
      is_end_(true)
  {
  }

  template < class T > LIter< T > &LIter< T >::operator++()
  {

    if (is_end_ || !curr_)
    {
      throw std::out_of_range("iterator can't move to next from end");
    }
    std::cout << "Node: " << this->operator*() << " is_end: " << is_end_ << std::endl;
    if (curr_->next == list_->h_)
    {
      is_end_ = true;
    }
    else
    {
      curr_ = curr_->next;
    }
    return *this;
  }
  template < class T > LIter< T > LIter< T >::operator++(int)
  {
    LIter< T > it(*this);
    this->operator++();
    return it;
  }

  template < class T > LIter< T > &LIter< T >::operator--()
  {

    if (is_end_)
    {
      is_end_ = false;
      std::cout << "Node: " << this->operator*() << " is_end: " << is_end_ << std::endl;
    }
    else
    {
      if (curr_ == list_->h_ || !curr_)
      {
        throw std::out_of_range("iterator can't move to previous from begin");
      }
      std::cout << "Node: " << this->operator*() << " is_end: " << is_end_ << std::endl;
      curr_ = curr_->prev;
    }
    return *this;
  }

  template < class T > LIter< T > LIter< T >::operator--(int)
  {
    LIter< T > it(*this);
    this->operator--();
    return it;
  }

  template < class T > bool LIter< T >::operator==(const LIter< T > &it) const noexcept
  {
    return (list_ == it.list_ && is_end_ == it.is_end_ && curr_ == it.curr_);
  }
  template < class T > bool LIter< T >::operator!=(const LIter< T > &it) const noexcept
  {
    return !(*this == it);
  }

  template < class T > T &LIter< T >::operator*()
  {
    if (is_end_ || !curr_)
    {
      throw std::runtime_error("can't get value of the iterator end element");
    }
    return curr_->val;
  }
  template < class T > T *LIter< T >::operator->()
  {
    if (is_end_ || !curr_)
    {
      throw std::runtime_error("can't use arrow operator of the iterator end element");
    }
    return &(curr_->val);
  }
}

#endif