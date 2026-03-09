#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <limits>

namespace vasyakin
{
  template< class T > class Node;
  template< class T > class List;
  const int MAX = std::numeric_limits< int >::max();
}

#endif