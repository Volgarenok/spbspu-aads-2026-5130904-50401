#ifndef LITER_HPP
#define LITER_HPP

#include <cstddef>

namespace hvostov {

  template< class T >
  class List;

  template< class T >
  class Node;

  template< class T >
  class Liter {
  public:
    Liter();
    Liter(const Liter< T >& liter);
    Liter(Liter< T >&& liter);
    ~Liter();
    Liter< T >& operator=(const Liter< T >& liter);
    Liter< T >& operator=(Liter< T >&& liter);

    Liter< T >& operator++();
    T operator*();
    T operator->();
    bool operator==(const Liter< T >& liter);
    bool operator!=(const Liter< T >& liter);
    
  private:
    friend class List< T >;
    Node< T >* curr_;
    Node< T >* fake_;
  };
}

#endif
