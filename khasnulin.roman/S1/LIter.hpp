#ifndef LITER_HPP
#define LITER_HPP

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

  private:
    using LNode = typename BiList< T >::template LNode< T >;

    LIter(LNode *node, bool is_end):
        curr_(node),
        is_end_(is_end) {};

    LNode *curr_;
    bool is_end_;
  };

  template < class T > LIter< T >::LIter() noexcept
  {
  }

  template < class T > LIter< T > &LIter< T >::operator++()
  {
    return *this;
  }
  template < class T > LIter< T > LIter< T >::operator++(int)
  {
    return *this;
  }

  template < class T > LIter< T > &LIter< T >::operator--()
  {
    return *this;
  }
  template < class T > LIter< T > LIter< T >::operator--(int)
  {
    return *this;
  }

  template < class T > bool LIter< T >::operator==(const LIter< T > &it) const noexcept
  {
    return false;
  }
  template < class T > bool LIter< T >::operator!=(const LIter< T > &it) const noexcept
  {
    return false;
  }

  template < class T > T &LIter< T >::operator*()
  {
    return *(new T());
  }
  template < class T > T *LIter< T >::operator->()
  {
    return new T();
  }
}

#endif