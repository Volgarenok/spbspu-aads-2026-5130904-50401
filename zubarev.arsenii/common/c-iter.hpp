#ifndef CITER_HPP
#define CITER_HPP

namespace zubarev
{
  template < class T >
  class List;
  template < class T >
  struct Node;

  template < class T >
  class LCIter
  {
    friend class List< T >;

  private:
    const Node< T >* ptr;
    LCIter(const Node< T >* p);

  public:
    const T& operator*() const;
    LCIter< T > operator++();
    bool operator!=(const LCIter&) const;
    bool operator==(const LCIter&) const;
  };

  template < class T >
  LCIter< T >::LCIter(const Node< T >* p) :
    ptr(p)
  {}

  template < class T >
  const T& LCIter< T >::operator*() const
  {
    return ptr->val;
  }

  template < class T >
  LCIter< T > LCIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }

  template < class T >
  bool LCIter< T >::operator!=(const LCIter& iter) const
  {
    return ptr != iter.ptr;
  }

  template < class T >
  bool LCIter< T >::operator==(const LCIter& iter) const
  {
    return ptr == iter.ptr;
  }
}

#endif
