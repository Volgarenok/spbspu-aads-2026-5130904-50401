#ifndef QUEUE_HPP
#define QUEUE_HPP
#include <iostream>
#include "order.hpp"

namespace zharov
{
  struct CafeSystem;

  using QueueTable = RHHashTable< int, Order, std::hash< int >, std::equal_to< int > >;
  using Queues =
    RHHashTable< std::string, QueueTable, std::hash< std::string >, std::equal_to< std::string > >;

  void queueCreate(std::ostream&, std::istream&, CafeSystem&);
  void queueRemove(std::ostream&, std::istream&, CafeSystem&);
  void queueComplete(std::ostream&, std::istream&, CafeSystem&);
  void queueReject(std::ostream&, std::istream&, CafeSystem&);

  void queueShow(std::ostream&, std::istream&, const CafeSystem&);
  void queueList(std::ostream&, std::istream&, const CafeSystem&);
}

#endif
