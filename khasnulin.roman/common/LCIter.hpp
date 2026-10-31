#ifndef LCITER_HPP
#define LCITER_HPP

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
        it_(list, node, is_end) {};

    LIter< T > it_;
  };

  template < class T >
  LCIter< T >::LCIter() noexcept:
      it_()
  {
  }

  template < class T >
  LCIter< T >::LCIter(const LIter< T > &it) noexcept:
      it_(it)
  {
  }

  template < class T > LCIter< T > &LCIter< T >::operator++()
  {
    ++it_;
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator++(int)
  {
    return it_++;
  }

  template < class T > LCIter< T > &LCIter< T >::operator--()
  {
    --it_;
    return *this;
  }

  template < class T > LCIter< T > LCIter< T >::operator--(int)
  {
    return it_--;
  }

  template < class T > bool LCIter< T >::operator==(const LCIter< T > &it) const noexcept
  {
    return it_ == it.it_;
  }

  template < class T > bool LCIter< T >::operator!=(const LCIter< T > &it) const noexcept
  {
    return !(*this == it);
  }

  template < class T > const T &LCIter< T >::operator*()
  {
    return *it_;
  }

  template < class T > const T *LCIter< T >::operator->()
  {
    return it_.operator->();
  }
}

#endif
