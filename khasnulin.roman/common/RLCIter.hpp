#ifndef RLCIter_HPP
#define RLCIter_HPP

namespace khasnulin
{
  template < class T > class BiList;
  template < class T > class RLIter;

  template < class T > class RLCIter
  {
  public:
    RLCIter() noexcept;
    RLCIter(const RLIter< T > &it) noexcept;

    RLCIter< T > &operator++();
    RLCIter< T > operator++(int);

    RLCIter< T > &operator--();
    RLCIter< T > operator--(int);

    bool operator==(const RLCIter< T > &it) const noexcept;
    bool operator!=(const RLCIter< T > &it) const noexcept;

    const T &operator*();
    const T *operator->();

    friend class BiList< T >;

  private:
    using LNode = typename BiList< T >::template LNode< T >;

    RLCIter(const BiList< T > *list, LNode *node, bool is_end):
        it_(list, node, is_end) {};

    RLIter< T > it_;
  };

  template < class T >
  RLCIter< T >::RLCIter() noexcept:
      it_()
  {
  }

  template < class T >
  RLCIter< T >::RLCIter(const RLIter< T > &it) noexcept:
      it_(it)
  {
  }

  template < class T > RLCIter< T > &RLCIter< T >::operator++()
  {
    ++it_;
    return *this;
  }

  template < class T > RLCIter< T > RLCIter< T >::operator++(int)
  {
    return it_++;
  }

  template < class T > RLCIter< T > &RLCIter< T >::operator--()
  {
    --it_;
    return *this;
  }

  template < class T > RLCIter< T > RLCIter< T >::operator--(int)
  {
    return it_--;
  }

  template < class T > bool RLCIter< T >::operator==(const RLCIter< T > &it) const noexcept
  {
    return it_ == it.it_;
  }

  template < class T > bool RLCIter< T >::operator!=(const RLCIter< T > &it) const noexcept
  {
    return !(*this == it);
  }

  template < class T > const T &RLCIter< T >::operator*()
  {
    return *it_;
  }

  template < class T > const T *RLCIter< T >::operator->()
  {
    return it_.operator->();
  }
}

#endif
