#ifndef BSTREE
#define BSTREE
#include "node.hpp"
namespace malashenko
{
  template< class Key, class Value, class Compare >
  class BSTree {
  public:
    BSTree();
    ~BSTree();

    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(Key k, Value v);
    Value& get(Key k);
    const Value& get(Key k) const;

    Value drop(Key k);
    using const_iterator = BSTConstIterator< Key, Value >;
    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);
    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);
    size_t height(const_iterator it);
    size_t height();
  private:
    Node< Key, Value >* node_;
    Node< Key, Value >* fakeLeaf_;
  };
}

#endif