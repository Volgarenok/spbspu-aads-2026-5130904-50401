#ifndef BSTREE_CONST_ITERATOR
#define BSTREE_CONST_ITERATOR
#include <functional>
#include <cstddef>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BSTreeCIter {
  public:
    BSTreeCIter();
    BSTreeCIter< Key, Value >& operator++();
    BSTreeCIter< Key, Value > operator++(int);
    BSTreeCIter< Key, Value > operator+(size_t s);

    BSTreeCIter< Key, Value >& operator--();
    BSTreeCIter< Key, Value > operator--(int);
    BSTreeCIter< Key, Value > operator-(size_t s);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator!=(const BSTreeCIter< Key, Value >& other) const;
    bool operator==(const BSTreeCIter< Key, Value >& other) const;
  private:
    Node< Key, Value >* node_;
    Node< Key, Value >* fakeLeaf_;
  };
}
#endif