#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace kondrat
{
  template< class T >
  class Node
  {
    T val;
    Node< T > * next;
    Node< T > * prev;
  };

  template< class T >
  class List
  {
    public:
      List();
      ~List();
      List(const List< T > & list);
      List< T > & operator=(const List< T > & list);
      List(const List< T > && list);
      List< T > & operator=(const List< T > && list);

      size_t size();
    private:
      Node< T > * fake_;
      size_t size_;
  };
}


#endif