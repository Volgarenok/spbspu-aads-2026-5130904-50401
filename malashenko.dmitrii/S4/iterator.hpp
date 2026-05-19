#ifndef BSTREE_ITERATOR
#define BSTREE_ITERATOR
#include <functional>
#include <cstddef>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BSTreeIter {
  public:
    BSTreeIter();
    BSTreeIter< Key, Value >& operator++();
    BSTreeIter< Key, Value > operator++(int);
    BSTreeIter< Key, Value > operator+(size_t s);

    BSTreeIter< Key, Value >& operator--();
    BSTreeIter< Key, Value > operator--(int);
    BSTreeIter< Key, Value > operator-(size_t s);

    std::pair< Key, Value >& operator*();
    std::pair< Key, Value >* operator->();

    bool operator!=(const BSTreeIter< Key, Value >& other) const;
    bool operator==(const BSTreeIter< Key, Value >& other) const;
  private:
    Node< Key, Value >* node_;
    Node< Key, Value >* fakeLeaf_;
  };
}
#endif