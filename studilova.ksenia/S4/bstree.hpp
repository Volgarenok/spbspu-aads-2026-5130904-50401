#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>

namespace studilova
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    public:
      BSTree();
      ~BSTree();

      bool empty() const;

    private:
      struct Node;
      Node* root_;
      Compare cmp_
  };
}

#endif
