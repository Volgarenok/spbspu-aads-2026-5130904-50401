#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace studilova
{
  template< class T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
  };

  template< class T >
  class List
  {
    private:
      Node< T >* head;
      size_t size_;
    public:
      List();
      ~List();

      bool empty() const;
  };

  template< class T >
  class LIter;

  template< class T >
  class CLIter; 
}

#endif