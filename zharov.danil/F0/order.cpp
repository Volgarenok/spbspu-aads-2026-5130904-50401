#include "order.hpp"
#include <iomanip>
#include <stdexcept>
#include <string>
#include "cafe.hpp"
#include "history.hpp"

namespace
{
  bool hasMoreArgs(std::istream& in)
  {
    while (in.peek() == ' ')
    {
      in.get();
    }
    return in.peek() != '\n' && in.peek() != EOF;
  }
}

int zharov::Order::getProfit(const zharov::CafeSystem& cafe) const
{
  int profit = 0;
  for (auto it = items.cbegin(); it != items.cend(); ++it)
  {
    const zharov::OrderItem& item = it->second;
    if (cafe.menus.has(item.menu_name) && cafe.menus.at(item.menu_name).has(it->first))
    {
      profit += cafe.menus.at(item.menu_name).at(it->first).price * item.count;
    }
  }
  return profit;
}

int zharov::Order::getTime(const zharov::CafeSystem& cafe) const
{
  int total = 0;
  for (auto it = items.cbegin(); it != items.cend(); ++it)
  {
    const zharov::OrderItem& item = it->second;
    if (cafe.menus.has(item.menu_name) && cafe.menus.at(item.menu_name).has(it->first))
    {
      total += cafe.menus.at(item.menu_name).at(it->first).prep_time * item.count;
    }
  }
  return total;
}

void zharov::orderCreate(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  zharov::QueueTable& queue = cafe.queues.at(queueName);
  if (queue.has(orderId))
  {
    throw std::invalid_argument("Order already exists");
  }
  queue.add(orderId, zharov::Order{orderId, zharov::ItemTable{}});
  out << "<OK: Order #" << orderId << " created in queue \"" << queueName << "\">\n";
}

void zharov::orderAdd(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string itemName;
  in >> std::quoted(itemName);
  if (!cafe.menus.at(menuName).has(itemName))
  {
    throw std::invalid_argument("Item not found");
  }
  if (!cafe.menus.at(menuName).at(itemName).available)
  {
    throw std::invalid_argument("Item unavailable");
  }
  int count = 1;
  if (hasMoreArgs(in))
  {
    in >> count;
  }
  zharov::Order& order = cafe.queues.at(queueName).at(orderId);
  if (order.items.has(itemName))
  {
    order.items.at(itemName).count += count;
  }
  else
  {
    order.items.add(itemName, zharov::OrderItem{menuName, count});
  }
  out << "<OK: Added " << count << "x \"" << itemName << "\" to order #" << orderId << ">\n";
}

void zharov::orderRemove(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  std::string itemName;
  in >> std::quoted(itemName);
  zharov::Order& order = cafe.queues.at(queueName).at(orderId);
  if (!order.items.has(itemName))
  {
    throw std::invalid_argument("Item not found in order");
  }
  order.items.remove(itemName);
  out << "<OK: Removed \"" << itemName << "\" from order #" << orderId << ">\n";
}

namespace
{
  void printOrder(std::ostream& out, const zharov::Order& order, const zharov::CafeSystem& cafe)
  {
    out << "=== ORDER #" << order.id << " ===\n";
    int total = 0, totalTime = 0;
    for (auto it = order.items.cbegin(); it != order.items.cend(); ++it)
    {
      const zharov::OrderItem& item = it->second;
      int price = 0, prepTime = 0;
      if (cafe.menus.has(item.menu_name) && cafe.menus.at(item.menu_name).has(it->first))
      {
        price = cafe.menus.at(item.menu_name).at(it->first).price;
        prepTime = cafe.menus.at(item.menu_name).at(it->first).prep_time;
      }
      out << item.count << "x " << it->first << " - " << item.count * price << " rub - "
          << item.count * prepTime << " min\n";
      total += item.count * price;
      totalTime += item.count * prepTime;
    }
    out << "TOTAL: " << total << " rub\n";
    out << "TOTAL TIME: " << totalTime << " min\n";
  }
}

void zharov::orderShow(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  const zharov::QueueTable& queue = cafe.queues.at(queueName);
  if (hasMoreArgs(in))
  {
    int orderId = 0;
    in >> orderId;
    if (!queue.has(orderId))
    {
      throw std::invalid_argument("Order not found");
    }
    printOrder(out, queue.at(orderId), cafe);
  }
  else
  {
    for (auto it = queue.cbegin(); it != queue.cend(); ++it)
    {
      printOrder(out, it->second, cafe);
    }
  }
}

void zharov::cancelOrder(zharov::CafeSystem& cafe, const std::string& queueName, int orderId)
{
  zharov::HistoryEntry entry;
  entry.queue_name = queueName;
  entry.order_id = orderId;
  entry.status = "cancelled";
  entry.order = cafe.queues.at(queueName).at(orderId);
  zharov::writeHistory(cafe, entry);
  cafe.queues.at(queueName).remove(orderId);
}

void zharov::orderCancel(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string queueName;
  in >> queueName;
  if (!cafe.queues.has(queueName))
  {
    throw std::invalid_argument("Queue not found");
  }
  int orderId = 0;
  in >> orderId;
  if (!cafe.queues.at(queueName).has(orderId))
  {
    throw std::invalid_argument("Order not found");
  }
  zharov::cancelOrder(cafe, queueName, orderId);
  out << "<OK: Order #" << orderId << " cancelled>\n";
}
