#ifndef LIST_HPP
#define LIST_HPP
#include <cstddef>
namespace kuznetsov {

  namespace {
    template< class T >
    struct Node {
      T val;
      Node< T >* next;
      Node< T >* prev;
    };
  }

  template< class T >
  class List;

  template< class T >
  class LCIter {
    friend class List< T >;
    Node< T >* curr;
  public:
    LCIter& operator++()
    {
      this->curr = this->curr->next;
      return this;
    }
    LCIter& operator++(int)
    {
      LCIter temp(*this);
      ++(*this);
      return temp;
    }
    LCIter& operator--()
    {
      this->curr = this->curr->prev;
      return this;
    }
    LCIter& operator--(int)
    {
      LCIter temp(*this);
      --(*this);
      return temp;
    }
    const T& operator*()
    {
      return curr->val;
    }
    bool operator==(const LCIter& y)
    {
      return this->curr == y.curr;
    }

    bool operator!=(const LCIter& y)
    {
      return !(this == y);
    }
  };

  template< class T >
  class LIter : LCIter< T > {
    T& operator*()
    {
      return curr->val;
    }
  };

}

#endif