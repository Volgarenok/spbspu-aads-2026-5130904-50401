#ifndef BSTREE_ITERATOR
#define BSTREE_ITERATOR
#include <functional>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BStreeIter {
  public:
    BStreeIter();
    BStreeIter< Key, Value >& operator++();
    BStreeIter< Key, Value > operator++(int);
    BStreeIter< Key, Value > operator+(size_t s);

    BStreeIter< Key, Value >& operator--();
    BStreeIter< Key, Value > operator--(int);
    BStreeIter< Key, Value > operator-(size_t s);

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

    bool operator!=(const BStreeIter< Key, Value >& other) const;
    bool operator==(const BStreeIter< Key, Value >& other) const;
  private:
    Node< Key, Value >* node_;
    Node< Key, Value >* fakeLeaf_;
  };
}
#endif