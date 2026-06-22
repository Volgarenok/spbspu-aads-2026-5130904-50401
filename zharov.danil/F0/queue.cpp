#include "queue.hpp"
#include <stdexcept>
#include <string>
#include "cafe.hpp"
#include "history.hpp"
#include "order.hpp"

void zharov::queueCreate(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string name;
  in >> name;
  if (cafe.queues.has(name))
  {
    throw std::invalid_argument("Queue already exists");
  }
  cafe.queues.add(name, zharov::QueueTable{});
  out << "<OK: Queue \"" << name << "\" created>\n";
}

void zharov::queueRemove(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string name;
  in >> name;
  if (!cafe.queues.has(name))
  {
    throw std::invalid_argument("Queue not found");
  }
  zharov::QueueTable copy = cafe.queues.at(name);
  for (auto it = copy.cbegin(); it != copy.cend(); ++it)
  {
    zharov::cancelOrder(cafe, name, it->first);
  }
  cafe.queues.remove(name);
  out << "<OK: Queue \"" << name << "\" removed>\n";
}

void zharov::queueComplete(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string name;
  in >> name;
  if (!cafe.queues.has(name))
  {
    throw std::invalid_argument("Queue not found");
  }
  zharov::QueueTable copy = cafe.queues.at(name);
  for (auto it = copy.cbegin(); it != copy.cend(); ++it)
  {
    zharov::HistoryEntry entry;
    entry.queue_name = name;
    entry.order_id = it->first;
    entry.status = "completed";
    entry.order = it->second;
    zharov::writeHistory(cafe, entry);
    cafe.queues.at(name).remove(it->first);
    out << "<OK: Order #" << it->first << " completed>\n";
  }
}

void zharov::queueReject(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string name;
  in >> name;
  if (!cafe.queues.has(name))
  {
    throw std::invalid_argument("Queue not found");
  }
  zharov::QueueTable copy = cafe.queues.at(name);
  for (auto it = copy.cbegin(); it != copy.cend(); ++it)
  {
    zharov::HistoryEntry entry;
    entry.queue_name = name;
    entry.order_id = it->first;
    entry.status = "rejected";
    entry.order = it->second;
    zharov::writeHistory(cafe, entry);
    cafe.queues.at(name).remove(it->first);
    out << "<OK: Order #" << it->first << " rejected>\n";
  }
}

void zharov::queueShow(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string name;
  in >> name;
  if (!cafe.queues.has(name))
  {
    throw std::invalid_argument("Queue not found");
  }
  const zharov::QueueTable& queue = cafe.queues.at(name);
  out << "=== QUEUE \"" << name << "\" ===\n";
  for (auto it = queue.cbegin(); it != queue.cend(); ++it)
  {
    out << "Order #" << it->first << " (" << it->second.items.size() << " items)\n";
  }
}

void zharov::queueList(std::ostream& out, std::istream&, const zharov::CafeSystem& cafe)
{
  for (auto it = cafe.queues.cbegin(); it != cafe.queues.cend(); ++it)
  {
    out << it->first << " (" << it->second.size() << " orders)\n";
  }
}
