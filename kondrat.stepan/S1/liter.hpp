#ifndef LITER_HPP
#define LITER_HPP

namespace kondrat
{
  template< class T >
  class LIter
  {
    friend class List< T >;
    public:
      LIter();
      LIter< T > & operator++();
      LIter< T > operator++(int);
      LIter< T > & operator--();
      LIter< T > operator--(int);
      bool operator==(LIter< T > rhs) const;
      bool operator!=(LIter< T > rhs) const;
      T & operator*();
      T * operator->();
    private:
      LIter(Node< T > * node) : node_(node) {}
      Node< T > * node_;
  };
}

#endif