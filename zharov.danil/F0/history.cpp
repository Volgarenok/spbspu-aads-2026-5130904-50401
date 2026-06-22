#include "history.hpp"
#include <fstream>
#include <stdexcept>
#include <string>

void zharov::writeHistory(const zharov::CafeSystem& cafe, const zharov::HistoryEntry& entry)
{
  std::ofstream file(cafe.history_file, std::ios::app);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open history file");
  }
  file << entry.queue_name << "|" << entry.order_id << "|" << entry.status << "|"
       << entry.order.getProfit(cafe) << "|";
  bool first = true;
  for (auto it = entry.order.items.cbegin(); it != entry.order.items.cend(); ++it)
  {
    if (!first)
    {
      file << ",";
    }
    file << it->second.menu_name << ":" << it->first << ":" << it->second.count;
    first = false;
  }
  file << "\n";
}

void zharov::cmdHistory(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  int n = 0;
  while (in.peek() == ' ')
  {
    in.get();
  }
  if (in.peek() != '\n' && in.peek() != EOF)
  {
    in >> n;
  }
  std::ifstream file(cafe.history_file);
  if (!file.is_open())
  {
    return;
  }
  int total = 0;
  std::string line;
  while (std::getline(file, line))
  {
    ++total;
  }
  file.clear();
  file.seekg(0);
  int skip = (n > 0 && n < total) ? total - n : 0;
  int current = 0;
  while (std::getline(file, line))
  {
    if (current >= skip)
    {
      out << line << "\n";
    }
    ++current;
  }
}
