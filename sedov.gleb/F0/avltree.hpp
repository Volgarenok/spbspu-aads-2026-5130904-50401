#ifndef AVLTREE_HPP
#define AVLTREE_HPP
#include <iostream>

namespace sedov
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class AVLTree
  {
  public:
      using valueType = std::pair< const Key, Value >;
      using iter = AVLiter< Key, Value >;
      using constIter = AVLConstiter< Key, Value >;

      AVLTree() noexcept;
      AVLTree(const AVLTree & h);
      AVLTree(AVLTree && h) noexcept;
      ~AVLTree();

      AVLTree& operator=(const AVLTree & h);
      AVLTree& operator=(AVLTree && h) noexcept;

      bool empty() const noexcept;
      size_t size() const noexcept;
      void swap(AVLTree & h) noexcept;
      void clear() noexcept;

      bool find(const Key & k, Value & outValue) const;
      const Value & at(const Key & k) const;
      Value & at(const Key & k);
      bool contains(const Key & k) const;

      bool insert(const Key & k, const Value & v);
      bool insert(Key && k, Value && v);
      bool erase(const Key & k);
      Value drop(const Key & k);

      void push(const Key & k, const Value & v)
      void push(Key && k, Value && v)

      iter begin() noexcept;
      iter end() noexcept;
      constIter begin() const noexcept;
      constIter end() const noexcept;
      constIter cbegin() const noexcept;
      constIter cend() const noexcept;

      size_t height() const noexcept;
      void collectInRange(const Key & from, const Key & to, List< valueType > & result) const;
      void getAll(List< value_type > & result) const;

  private:
      AVLTreeNode< Key, Value > * root_;
      size_t size_;
      Key comp_;

      AVLTreeNode< Key, Value > * clone(AVLTreeNode< Key, Value > * src, AVLTreeNode< Key, Value > * parent);
      void clearImpl(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * fallLeft(AVLTreeNode< Key, Value > * node) const noexcept;
      AVLTreeNode< Key, Value > * fallRight(AVLTreeNode< Key, Value > * node) const noexcept;

      void updateHeight(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * balanceNode(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * rotateLeft(AVLTreeNode< Key, Value > * x) noexcept;
      AVLTreeNode< Key, Value > * rotateRight(AVLTreeNode< Key, Value > * y) noexcept;
      AVLTreeNode< Key, Value > * rotateLargeLeft(AVLTreeNode< Key, Value > * node) noexcept;
      AVLTreeNode< Key, Value > * rotateLargeRight(AVLTreeNode< Key, Value > * node) noexcept;

      std::pair< AVLTreeNode< Key, Value > *, bool > insertNode(AVLTreeNode< Key, Value > * node, const Key & k,
        const Value & v);
      std::pair< AVLTreeNode< Key, Value > *, bool > insertNode(AVLTreeNode< Key, Value > * node, Key && k,
        Value && v);
      AVLTreeNode< Key, Value > * removeNode(AVLTreeNode< Key, Value > * node, const Key & k, bool & found) noexcept;
      const AVLTreeNode< Key, Value > * findNode(const Key & k) const noexcept;

      void collectInRangeImpl(AVLTreeNode< Key, Value > * node, const Key & from, const Key & to,
        List< valueType> & result) const;
      void getAllImpl(AVLTreeNode< Key, Value > * node, List< valueType> & result) const;
  };

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree() noexcept:
    root_(nullptr),
    size_(0),
    comp_()
  {}

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(const AVLTree & h):
    root_(nullptr),
    size_(0),
    comp_(h.comp_)
  {
    root_ = clone(h.root_, nullptr);
    size_ = h.size_;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::AVLTree(AVLTree && h) noexcept:
    root_(h.root_),
    size_(h.size_),
    comp_(std::move(h.comp_))
  {
    h.root_ = nullptr;
    h.size_ = 0;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare >::~AVLTree()
  {
    clear();
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare > & AVLTree< Key, Value, Compare >::operator=(const AVLTree & h)
  {
    if (this != std::addressof(h))
    {
      AVLTree temp(h);
      swap(temp);
    }
    return *this;
  }

  template < class Key, class Value, class Compare >
  AVLTree< Key, Value, Compare > & AVLTree< Key, Value, Compare >::operator=(AVLTree && h) noexcept
  {
    if (this != std::addressof(h))
    {
      clear();
      root_ = h.root_;
      size_ = h.size_;
      comp_ = std::move(h.comp_);
      h.root_ = nullptr;
      h.size_ = 0;
    }
    return *this;
  }
}

#endif
