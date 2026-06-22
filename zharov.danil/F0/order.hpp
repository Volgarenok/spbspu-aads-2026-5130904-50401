#ifndef ORDER_HPP
#define ORDER_HPP
#include <iostream>
#include <string>
#include <rh_hash_table.hpp>

namespace zharov
{
  struct CafeSystem;

  struct OrderItem
  {
    std::string menu_name;
    int count;
  };

  using ItemTable =
    RHHashTable< std::string, OrderItem, std::hash< std::string >, std::equal_to< std::string > >;

  struct Order
  {
    int id;
    ItemTable items;

    int getProfit(const CafeSystem& cafe) const;
    int getTime(const CafeSystem& cafe) const;
  };

  void cancelOrder(CafeSystem&, const std::string& queueName, int orderId);

  void orderCreate(std::ostream&, std::istream&, CafeSystem&);
  void orderAdd(std::ostream&, std::istream&, CafeSystem&);
  void orderRemove(std::ostream&, std::istream&, CafeSystem&);
  void orderCancel(std::ostream&, std::istream&, CafeSystem&);

  void orderShow(std::ostream&, std::istream&, const CafeSystem&);
}
#endif
