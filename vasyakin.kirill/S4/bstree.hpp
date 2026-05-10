#ifndef BSTTREE_HPP
#define BSTTREE_HPP
#include "treenode.hpp"
#include "bstiterators.hpp"
#include <functional>
#include <stdexcept>
#include <utility>
#include <cstddef>
#include <algorithm>
#include <memory>

namespace vasyakin
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:

    BSTree(const Compare& cmp = Compare());
    ~BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    void push(const Key& k, const Value& v);
    Value& get(const Key& k);
    const Value& get(const Key& k) const;
    Value drop(Key k);

    using const_iterator = BSTConstIterator< Key, Value >;
    using iterator = BSTIterator< Key, Value >;

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it);
    size_t height();

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const;

  private:
    using Node = ::vasyakin::Node< Key, Value >;

    Node* root_;
    Node* fake_leaf_;
    Compare cmp_;

    void clear(Node* node);
    void swap(BSTree& other) noexcept;
    Node* cloneNode(const Node* src, Node* parent, const Node* src_fake_leaf);
    Node* findNode(const Key& k);
    const Node* findNode(const Key& k) const;
    size_t calcHeight(const Node* node) const;
    Node* fallLeft(Node* node) const;
  };
}

#endif
