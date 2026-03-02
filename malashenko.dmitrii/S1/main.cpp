#include <iostream>
#include <cstddef>
#include <utility>
#include <string>

namespace malashenko {
  template< class T > class List;
  template< class T >
  class LIter {
    friend class List< T >;
  };

  template< class T >
  class LCIter {
    friend class List< T >;
  };

  template< class T >
  class List {
  public:
      List();
      List(const Node< T >& other);
      List(Node< T >&& other);

      List& operator=(const List< T >& other);
      List& operator=(List< T >&& other);

  private:
    template< class T >
    class Node {
      T value_;
      Node< T >* next;
      Node< T >* prev;
    };
    
    
    Node< T > head_;
    Node< T > tail_;
  };

  template< class T >
  LCIter< T > begin(List< T >& list);

  template< class T >
  LCIter< T > end(List< T >& list);

  template< class T >
  T& front(List< T >& list);

  template< class T >
  T& back(List< T >& list);

  template< class T >
  void push_back(List< T >& list, const T& value);

  template< class T >
  void push_front(List< T >& list, const T& value);

  template< class T >
  void pop_back(List< T >& list);

  template< class T >
  void pop_front(List< T >& list);

  template< class T >
  void clear(List< T >& list);

  template< class T >
  void fake(List< T >& list);

  template< class T >
  void rmfake(List< T >& list);

};







int main()
{}
