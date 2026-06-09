#ifndef AVLITERATORS_HPP
#define AVLITERATORS_HPP
#include <utility>
#include "avltreenode.hpp"

namespace sedov
{
  template< class Key, class Value, class Compare >
  class AVLTree;

  template< class Key, class Value >
  class AVLConstIterator
  {
  public:
    using valueType = std::pair< const Key, Value >;

    explicit AVLConstIterator(AVLTreeNode< Key, Value > * node = nullptr);
    valueType operator*() const;

    AVLConstIterator & operator++() noexcept;
    AVLConstIterator operator++(int) noexcept;
    AVLConstIterator & operator--() noexcept;
    AVLConstIterator operator--(int) noexcept;
    bool operator==(const AVLConstIterator & other) const noexcept;
    bool operator!=(const AVLConstIterator & other) const noexcept;

  private:
    AVLTreeNode< Key, Value > * node_;

    template< class Key, class Value, class Compare >
    friend class AVLTree;
  };

  template< class Key, class Value >
  class AVLIterator
  {
  public:
    using valueType = std::pair<const Key, Value>;

    explicit AVLIterator(AVLTreeNode< Key, Value > * node = nullptr);
    valueType operator*() const;
    
    AVLIterator & operator++() noexcept;
    AVLIterator operator++(int) noexcept;
    AVLIterator & operator--() noexcept;
    AVLIterator operator--(int) noexcept;
    bool operator==(const AVLIterator & other) const noexcept;
    bool operator!=(const AVLIterator & other) const noexcept;

  private:
    AVLTreeNode< Key, Value > * node_;
    template< class Key, class Value, class Compare >
    friend class AVLTree;
  };
}

#endif
