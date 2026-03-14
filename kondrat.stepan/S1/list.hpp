#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace kondrat
{
  template< class T >
  class Node
  {
    public:
      Node(T value, Node< T > * n, Node< T > * p) : val(value), next(n), prev(p) {}
      T val;
      Node< T > * next;
      Node< T > * prev;
  };

  template< class T >
  class LIter;

  template< class T >
  class LCIter;

  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T > & list);
      List< T > & operator=(const List< T > & list);
      List(List< T > && list);
      List< T > & operator=(List< T > && list);

      LIter< T > begin();
      LIter< T > end();
      LCIter< T > begin();
      LCIter< T > end();

      T & front();
      T & back();
      const T & front() const;
      const T & back() const;

      void pushFront(const T & value);
      void pushBack(const T & value);

      void popFront();
      void popBack();

      void clear();

      size_t size() const;
      bool empty() const;
    private:
      Node< T > * fake_;
      size_t size_;
  };
}


#endif