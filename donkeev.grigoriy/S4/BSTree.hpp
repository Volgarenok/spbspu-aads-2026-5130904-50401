#ifndef BSTREE_HPP
#define BSTREE_HPP

#include "donkeev.grigoriy/S4/BSTNode.hpp"

namespace donkeev
{
  template< class Key, class Value, class Compare >
  class BSTree
  {
  public:
    BSTree();

    BSTree(const BSTree< Key, Value, Compare >&);
    BSTree(BSTree< Key, Value, Compare >&&);

    BSTree(const Key, const Value&);
    BSTree(const Key, Value&&);

    ~BSTree();

    BSTree< Key, Value, Compare >& operator=(const BSTree< Key, Value, Compare >&);
    BSTree< Key, Value, Compare >& operator=(BSTree< Key, Value, Compare >&&);

    void push(Key, const Value&);
    void push(Key, Value&&);
    Value& get(Key);
    Value drop(Key);
  private:
    BSTNode< Key, Value >* root;
    size_t size;
  };
}

#endif
