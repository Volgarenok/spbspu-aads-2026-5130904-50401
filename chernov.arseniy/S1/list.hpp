#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace chernov {
  template< class T >
  class Node {
    T data;
    Node< T > * next;
  };

  template< class T >
  class List {
    Node< T > * fake_;
    size_t size_;
    Node< T > * createFake();
    void removeFake() noexcept;
  public:
    List();
    ~List() noexcept;
    size_t size() noexcept;
  };
}

#endif
