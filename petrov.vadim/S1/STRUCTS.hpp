#ifndef STRUCTS_HPP
#define STRUCTS_HPP
#include <cstddef>
namespace petrov
{
  template< class T >
  struct Node
  {
    T val;
    Node< T >* next;
  };

  template< class T >
  class List;

  template< class T >
  class LIter
  {
    friend class List< T >;
    Node< T >* nd;

    public:
      LIter();
      LIter(Node< T >* p);
      bool hasNext() const;
      LIter< T > next() const;
      T& operator*() const;
      bool operator==(const LIter< T >& it) const;
      bool operator!=(const LIter< T >& it) const;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    Node< T >* nd;

    public:
      LCIter();
      LCIter(Node< T >* p);
      bool hasNext() const;
      LCIter< T > next() const;
      const T& operator*() const;
      bool operator==(const LCIter< T >& it) const;
      bool operator!=(const LCIter< T >& it) const;
  };

  template< class T >
  class List
  {
    Node< T >* fake;
    Node< T >* mkFake();
    void delFake();

    public:
      List();
      List(const List< T >& l);
      List(List< T >&& l);
      List< T >& operator=(const List< T >& l);
      List< T >& operator=(List< T >&& l);
      ~List();
      void clear();

      LIter< T > begin();
      LCIter< T > begin() const;
      LIter< T > end();
      LCIter< T > end() const;

      Node< T > getLast();
      Node< T > getLast() const;
      Node< T > getFirst();
      Node< T > getFirst() const;

      LIter< T > addStart(const T& a);
      LCIter< T > popStart();

      LIter< T > insert(LIter< T > id, const T& a);
      LIter< T > insert(LIter< T > id, const T&& a);
  };
}
#endif
