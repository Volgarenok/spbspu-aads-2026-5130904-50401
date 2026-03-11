#ifndef LCITER_HPP
#define LCITER_HPP

namespace khasnulin
{
  template < class T > class BiList;
  template < class T > class LIter;
  template < class T > struct LNode;

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
    LCIter(const LNode< T > *node);

    const LNode< T > *curr_;
  };

  template < class T > LCIter< T >::LCIter() noexcept
  {
  }

  template < class T > LCIter< T >::LCIter(const LIter< T > &it) noexcept
  {
  }

  template < class T > LCIter< T > &LCIter< T >::operator++()
  {
    return *this;
  }
  template < class T > LCIter< T > LCIter< T >::operator++(int)
  {
    return *this;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--()
  {
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator--(int)
  {
    return *this;
  }

  template < class T > bool LCIter< T >::operator==(const LCIter< T > &it) const noexcept
  {
    return false;
  }
  template < class T > bool LCIter< T >::operator!=(const LCIter< T > &it) const noexcept
  {
    return false;
  }

  template < class T > const T &LCIter< T >::operator*()
  {
    return *(new T());
  }
  template < class T > const T *LCIter< T >::operator->()
  {
    return new T();
  }
}

#endif