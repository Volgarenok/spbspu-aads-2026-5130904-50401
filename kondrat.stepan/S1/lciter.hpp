#ifndef LCITER_HPP
#define LCITER_HPP

namespace kondrat
{
  template< class T >
  class Node;

  template< class T >
  class List;

  template< class T >
  class LCIter
  {
    friend class List;
    public:
      LCIter();
      LCIter< T > & operator++();
      LCIter< T > operator++(int);
      LCIter< T > & operator--();
      LCIter< T > operator--(int);
      bool operator==(const LCIter< T > & rhs) const;
      bool operator!=(const LCIter< T > & rhs) const;
      T & operator*() const;
      T * operator->() const;
    private:
      LCIter(Node< T > * node) : node_(node) {}
      Node< T > * node_;
  };
}

#endif