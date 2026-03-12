#ifndef LCITER_HPP
#define LCITER_HPP

#include <stdexcept>
namespace khasnulin
{
  template < class T > class BiList;
  template < class T > class LIter;

  template < class T > class LCIter
  {
  public:
    LCIter() noexcept;
    LCIter(const LIter< T > &it) noexcept;

    LCIter< T > &operator++();
    LCIter< T > operator++(int);

    LCIter< T > &operator--();
    LCIter< T > operator--(int);

    bool operator==(const LCIter< T > &it) const noexcept;
    bool operator!=(const LCIter< T > &it) const noexcept;

    const T &operator*();
    const T *operator->();

    friend class BiList< T >;

  private:
    using LNode = typename BiList< T >::template LNode< T >;

    LCIter(const BiList< T > *list, LNode *node, bool is_end):
        list_(list),
        curr_(node),
        is_end_(is_end) {};

    const BiList< T > *list_;
    LNode *curr_;
    bool is_end_;
  };

  template < class T >
  LCIter< T >::LCIter() noexcept:
      list_(nullptr),
      curr_(nullptr),
      is_end_(true)
  {
  }

  template < class T >
  LCIter< T >::LCIter(const LIter< T > &it) noexcept:
      list_(it.list_),
      curr_(it.curr_),
      is_end_(it.is_end_)
  {
  }

  template < class T > LCIter< T > &LCIter< T >::operator++()
  {

    if (curr_->next == list_->h_)
    {
      is_end_ = true;
    }
    else
    {
      if (is_end_ || !curr_)
      {
        throw std::out_of_range("iterator can't move to next from end");
      }
      curr_ = curr_->next;
    }
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter< T > it(*this);
    this->operator++();
    return it;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--()
  {
    if (curr_ == list_->h_ || !curr_)
    {
      throw std::out_of_range("iterator can't move to previous from begin");
    }
    if (is_end_)
    {
      is_end_ = false;
    }
    else
    {
      curr_ = curr_->prev;
    }
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator--(int)
  {
    LCIter< T > it(*this);
    this->operator--();
    return it;
  }

  template < class T > bool LCIter< T >::operator==(const LCIter< T > &it) const noexcept
  {
    return (list_ == it.list_ && is_end_ == it.is_end_ && curr_ == it.curr_);
  }

  template < class T > bool LCIter< T >::operator!=(const LCIter< T > &it) const noexcept
  {
    return !(*this == it);
  }

  template < class T > const T &LCIter< T >::operator*()
  {
    if (is_end_ || !curr_)
    {
      throw std::runtime_error("can't get value of the iterator end element");
    }
    return curr_->val;
  }

  template < class T > const T *LCIter< T >::operator->()
  {
    if (is_end_ || !curr_)
    {
      throw std::runtime_error("can't use arrow operator of the iterator end element");
    }
    return &(curr_->val);
  }
}

#endif