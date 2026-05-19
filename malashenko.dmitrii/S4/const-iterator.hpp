#ifndef BSTREE_CONST_ITERATOR
#define BSTREE_CONST_ITERATOR
#include <functional>
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value >
  class BStreeCIter {
  public:
    BStreeCIter();
    BStreeCIter< Key, Value >& operator++();
    BStreeCIter< Key, Value > operator++(int);
    BStreeCIter< Key, Value > operator+(size_t s);

    BStreeCIter< Key, Value >& operator--();
    BStreeCIter< Key, Value > operator--(int);
    BStreeCIter< Key, Value > operator-(size_t s);

    const std::pair< Key, Value >& operator*() const;
    const std::pair< Key, Value >* operator->() const;

    bool operator!=(const BStreeCIter< Key, Value >& other) const;
    bool operator==(const BStreeCIter< Key, Value >& other) const;
  private:
    Node< Key, Value >* node_;
    Node< Key, Value >* fakeLeaf_;
  };
}
#endif