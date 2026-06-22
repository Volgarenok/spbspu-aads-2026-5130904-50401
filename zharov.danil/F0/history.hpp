#ifndef HISTORY_HPP
#define HISTORY_HPP
#include <iostream>
#include <string>
#include "cafe.hpp"

namespace zharov
{
  struct HistoryEntry
  {
    std::string queue_name;
    int order_id;
    std::string status;
    Order order;
  };

  void writeHistory(const CafeSystem& cafe, const HistoryEntry& entry);
  void cmdHistory(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
