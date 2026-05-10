#ifndef BSTITERATORS_HPP
#define BSTITERATORS_HPP
#include <utility>
#include "treenode.hpp"

namespace vasyakin
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value >
  class BSTIterator
  {
  private:
    using Node = vasyakin::Node< Key, Value >;
    Node* node_;
    Node* fake_leaf_;

    template< class K, class V, class C >
    friend class BSTree;

  public:
    BSTIterator();
    explicit BSTIterator(Node* node, Node* fake_leaf);

    std::pair< Key, Value > operator*() const;

    BSTIterator& operator++();
    BSTIterator operator++(int);

    BSTIterator& operator--();
    BSTIterator operator--(int);

    bool operator==(const BSTIterator& other) const;
    bool operator!=(const BSTIterator& other) const;

    operator BSTConstIterator< Key, Value >() const;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  private:
    using Node = vasyakin::Node< Key, Value >;
    const Node* node_;
    const Node* fake_leaf_;

    template< class K, class V, class C >
    friend class BSTree;

  public:
    BSTConstIterator();
    explicit BSTConstIterator(const Node* node, const Node* fake_leaf);

    const std::pair< Key, Value > operator*() const;

    BSTConstIterator& operator++();
    BSTConstIterator operator++(int);

    BSTConstIterator& operator--();
    BSTConstIterator operator--(int);

    bool operator==(const BSTConstIterator& other) const;
    bool operator!=(const BSTConstIterator& other) const;
  };
}

#endif
