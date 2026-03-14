#ifndef RLITER_HPP
#define RLITER_HPP

#include <stdexcept>

namespace khasnulin
{
  template < class T > class BiList;
  template < class T > class RLCIter;

  template < class T > class RLIter
  {
  public:
    RLIter() noexcept;

    RLIter< T > &operator++();
    RLIter< T > operator++(int);

    RLIter< T > &operator--();
    RLIter< T > operator--(int);

    bool operator==(const RLIter< T > &it) const noexcept;
    bool operator!=(const RLIter< T > &it) const noexcept;

    T &operator*();
    T *operator->();

    friend class BiList< T >;
    friend class RLCIter< T >;

  private:
    using LNode = typename BiList< T >::template LNode< T >;

    RLIter(BiList< T > *list, LNode *node, bool is_begin) noexcept:
        list_(list),
        curr_(node),
        is_begin_(is_begin)
    {
    }

    BiList< T > *list_;
    LNode *curr_;
    bool is_begin_;
  };

  template < class T >
  RLIter< T >::RLIter() noexcept:
      list_(nullptr),
      curr_(nullptr),
      is_begin_(true)
  {
  }

  template < class T > RLIter< T > &RLIter< T >::operator++()
  {

    if (is_begin_ || !curr_)
    {
      throw std::out_of_range("reverse iterator can't move to next, its already in reverse end");
    }
    if (curr_ == list_->h_)
    {
      is_begin_ = true;
    }
    else
    {
      curr_ = curr_->prev;
    }
    return *this;
  }

  template < class T > RLIter< T > RLIter< T >::operator++(int)
  {
    RLIter< T > it(*this);
    this->operator++();
    return it;
  }

  template < class T > RLIter< T > &RLIter< T >::operator--()
  {

    if (is_begin_)
    {
      is_begin_ = false;
    }
    else
    {
      if (curr_ == list_->h_->prev || !curr_)
      {
        throw std::out_of_range("reverse iterator can't move to previous from reverse begin");
      }
      curr_ = curr_->next;
    }
    return *this;
  }

  template < class T > RLIter< T > RLIter< T >::operator--(int)
  {
    RLIter< T > it(*this);
    this->operator--();
    return it;
  }

  template < class T > bool RLIter< T >::operator==(const RLIter< T > &it) const noexcept
  {
    return (list_ == it.list_ && is_begin_ == it.is_begin_ && curr_ == it.curr_);
  }
  template < class T > bool RLIter< T >::operator!=(const RLIter< T > &it) const noexcept
  {
    return !(*this == it);
  }

  template < class T > T &RLIter< T >::operator*()
  {
    if (is_begin_ || !curr_)
    {
      throw std::runtime_error("can't get value of the reverse iterator end or empty element");
    }
    return curr_->val;
  }
  template < class T > T *RLIter< T >::operator->()
  {
    if (is_begin_ || !curr_)
    {
      throw std::runtime_error("can't use arrow operator of the iterator end or empty element");
    }
    return &(curr_->val);
  }

}

#endif
