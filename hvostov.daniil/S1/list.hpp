#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace hvostov {
  template< class T >
  class Liter;
  template< class T >
  class LCiter;

  template< class T >
  class Node {
    T val_;
    Node< T >* next_;
  };

  template< class T >
  class List {
    public:
      List();
      List(const List< T >& list);
      List(List< T >&& list);
      ~List();
      List< T >& operator=(const List< T >& list);
      List< T >& operator=(List< T >&& list);

      Liter< T > begin() const noexcept;
      Liter< T > end() const noexcept;
      LCiter< T > cend() const noexcept;
      LCiter< T > cbegin() const noexcept;

      Liter< T > insertAfter(Liter< T > it, const T val);
      void clear();
    private:
      Node< T >* fake_;
      Node< T >* createFake();
      void rmFake() noexcept;
  };
}

#endif
