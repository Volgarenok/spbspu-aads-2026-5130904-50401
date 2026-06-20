#ifndef BSTIT_HPP
#define BSTIT_HPP

#include <BSNode.hpp>

namespace donkeev
{
  template< class Key, class Value >
  class BSTIterator
  {
  public:
    BSTIterator();

    BSTIterator(const BSTIterator< Key, Value >&);
    BSTIterator(BSTIterator< Key, Value >&&);

    BSTIterator(NodeBST< Key, Value >*);

    ~BSTIterator() = default;
    
    BSTIterator< Key, Value > operator=(const BSTIterator< Key, Value >&);
    BSTIterator< Key, Value > operator=(BSTIterator< Key, Value >&&);

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    BSTIterator< Key, Value > operator++();

    bool operator==(const BSTIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTIterator< Key, Value >&) const noexcept;
  private:
    BSTNode< Key, Value >* node;
  };

  template< class Key, class Value >
  class BSTCIterator
  {
  public:
    BSTCIterator();

    BSTCIterator(const BSTCIterator< Key, Value >&);
    BSTCIterator(BSTCIterator< Key, Value >&&);

    BSTCIterator(NodeBST< Key, Value >*);

    ~BSTCIterator() = default;
    
    BSTCIterator< Key, Value > operator=(const BSTCIterator< Key, Value >&);
    BSTCIterator< Key, Value > operator=(BSTCIterator< Key, Value >&&);

    std::pair< Key, Value >& operator*() const;
    std::pair< Key, Value >* operator->() const;
    BSTCIterator< Key, Value > operator++();

    bool operator==(const BSTCIterator< Key, Value >&) const noexcept;
    bool operator!=(const BSTCIterator< Key, Value >&) const noexcept;
  private:
    BSTNode< Key, Value >* node;
  };
}

#endif
