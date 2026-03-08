#ifndef ITER_HPP
#define ITER_HPP
namespace zubarev

{
  template < class T >
  class List;
  template < class T >
  class LIter
  {
    friend class List< T >;

  private:
    typename List< T >::Node* ptr;
    LIter(typename List< T >::Node* p);

  public:
    T& operator*();
    LIter< T > operator++();
    bool operator!=(const LIter&) const;
    bool operator==(const LIter&) const;
  };
  template < class T >
  LIter< T >::LIter(typename List< T >::Node* p) :
    ptr(p)
  {}

  template < class T >
  T& LIter< T >::operator*()
  {
    return ptr->val;
  }
  template < class T >
  LIter< T > LIter< T >::operator++()
  {
    ptr = ptr->next;
    return *this;
  }
  template < class T >
  bool LIter< T >::operator!=(const LIter& iter) const
  {
    return ptr != iter.ptr;
  }
  template < class T >
  bool LIter< T >::operator==(const LIter& iter) const
  {
    return ptr == iter.ptr;
  }
}

#endif
