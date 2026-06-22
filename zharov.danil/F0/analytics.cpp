#include "analytics.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector.hpp>
#include "cafe.hpp"
#include "order.hpp"

void zharov::cmdOptimize(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string srcName, acceptName, rejectName;
  int availableTime = 0;
  in >> srcName;
  if (!cafe.queues.has(srcName))
  {
    throw std::invalid_argument("Source queue not found");
  }
  in >> acceptName;
  if (cafe.queues.has(acceptName))
  {
    throw std::invalid_argument("Accepted queue already exists");
  }
  in >> rejectName;
  if (cafe.queues.has(rejectName))
  {
    throw std::invalid_argument("Rejected queue already exists");
  }
  in >> availableTime;
  if (availableTime <= 0)
  {
    throw std::invalid_argument("Available time must be positive");
  }

  const zharov::QueueTable& src = cafe.queues.at(srcName);
  size_t n = src.size();

  zharov::Vector< int > profits(n, 0);
  zharov::Vector< int > times(n, 0);
  zharov::Vector< int > orderIds(n, 0);

  size_t idx = 0;
  for (auto it = src.cbegin(); it != src.cend(); ++it, ++idx)
  {
    orderIds[idx] = it->first;
    profits[idx] = it->second.getProfit(cafe);
    times[idx] = it->second.getTime(cafe);
  }

  zharov::Vector< zharov::Vector< int > > dp(n + 1, zharov::Vector< int >(availableTime + 1, 0));

  for (size_t i = 1; i <= n; ++i)
  {
    for (int w = 0; w <= availableTime; ++w)
    {
      dp[i][w] = dp[i - 1][w];
      if (times[i - 1] <= w)
      {
        dp[i][w] = std::max(dp[i][w], dp[i - 1][w - times[i - 1]] + profits[i - 1]);
      }
    }
  }

  zharov::Vector< bool > selected(n, false);
  int w = availableTime;
  for (size_t i = n; i >= 1; --i)
  {
    if (dp[i][w] != dp[i - 1][w])
    {
      selected[i - 1] = true;
      w -= times[i - 1];
    }
  }

  cafe.queues.add(acceptName, zharov::QueueTable{});
  cafe.queues.add(rejectName, zharov::QueueTable{});

  std::ios::fmtflags savedFlags = out.flags();
  out << std::right;

  out << "SOURCE QUEUE: " << srcName << "\n";
  out << "AVAILABLE TIME: " << availableTime << " min\n";
  out << "PROCESSING QUEUE:\n";
  int totalProfit = 0;
  for (size_t i = 0; i < n; ++i)
  {
    const zharov::Order& order = src.at(orderIds[i]);
    if (selected[i])
    {
      cafe.queues.at(acceptName).add(orderIds[i], order);
      out << "#" << std::setw(3) << orderIds[i] << " | " << std::setw(5) << profits[i] << " rub"
          << " | " << std::setw(3) << times[i] << " min\n";
      totalProfit += profits[i];
    }
  }
  out << "TOTAL PROFIT: " << totalProfit << " rub\n";
  out << "REJECTED QUEUE:\n";
  for (size_t i = 0; i < n; ++i)
  {
    const zharov::Order& order = src.at(orderIds[i]);
    if (!selected[i])
    {
      cafe.queues.at(rejectName).add(orderIds[i], order);
      out << "#" << std::setw(3) << orderIds[i] << " | " << std::setw(5) << profits[i] << " rub"
          << " | " << std::setw(3) << times[i] << " min\n";
    }
  }

  out.flags(savedFlags);
  cafe.queues.remove(srcName);
}

namespace
{
  std::string tokenMenu(const std::string& token)
  {
    size_t c1 = token.find(':');
    return (c1 == std::string::npos) ? "" : token.substr(0, c1);
  }

  std::string tokenItem(const std::string& token)
  {
    size_t c1 = token.find(':');
    if (c1 == std::string::npos)
    {
      return "";
    }
    size_t c2 = token.find(':', c1 + 1);
    return (c2 == std::string::npos) ? "" : token.substr(c1 + 1, c2 - c1 - 1);
  }

  int tokenCount(const std::string& token)
  {
    size_t c1 = token.find(':');
    if (c1 == std::string::npos)
    {
      return 0;
    }
    size_t c2 = token.find(':', c1 + 1);
    if (c2 == std::string::npos)
    {
      return 0;
    }
    try
    {
      return std::stoi(token.substr(c2 + 1));
    }
    catch (...)
    {
      return 0;
    }
  }
}

void zharov::analyzeItem(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string menuName, itemName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  in >> std::quoted(itemName);
  if (!cafe.menus.at(menuName).has(itemName))
  {
    throw std::invalid_argument("Item not found");
  }

  const zharov::MenuItem& item = cafe.menus.at(menuName).at(itemName);

  int completedOrders = 0, completedUnits = 0, completedProfit = 0, completedTime = 0;
  int rejectedOrders = 0, rejectedUnits = 0;

  std::ifstream file(cafe.history_file);
  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      size_t p1 = line.find('|');
      size_t p2 = (p1 != std::string::npos) ? line.find('|', p1 + 1) : std::string::npos;
      size_t p3 = (p2 != std::string::npos) ? line.find('|', p2 + 1) : std::string::npos;
      size_t p4 = (p3 != std::string::npos) ? line.find('|', p3 + 1) : std::string::npos;
      if (p4 == std::string::npos)
      {
        continue;
      }
      std::string status = line.substr(p2 + 1, p3 - p2 - 1);
      if (status != "completed" && status != "rejected")
      {
        continue;
      }
      int orderProfit = 0;
      try
      {
        orderProfit = std::stoi(line.substr(p3 + 1, p4 - p3 - 1));
      }
      catch (...)
      {
        continue;
      }

      std::string itemsStr = line.substr(p4 + 1);
      int countInOrder = 0;
      size_t pos = 0;
      while (pos < itemsStr.size())
      {
        size_t comma = itemsStr.find(',', pos);
        std::string token =
          (comma == std::string::npos) ? itemsStr.substr(pos) : itemsStr.substr(pos, comma - pos);
        pos = (comma == std::string::npos) ? itemsStr.size() : comma + 1;

        if (tokenMenu(token) == menuName && tokenItem(token) == itemName)
        {
          countInOrder += tokenCount(token);
        }
      }

      if (countInOrder == 0)
      {
        continue;
      }

      if (status == "completed")
      {
        ++completedOrders;
        completedUnits += countInOrder;
        completedProfit += orderProfit;
        completedTime += item.prep_time * countInOrder;
      }
      else
      {
        ++rejectedOrders;
        rejectedUnits += countInOrder;
      }
    }
  }

  out << "=== ITEM ANALYSIS: " << itemName << " ===\n";
  out << "Price: " << item.price << " rub | Prep time: " << item.prep_time << " min\n";
  out << "Completed orders: " << completedOrders << " (" << completedUnits << " units)\n";
  out << "Rejected orders:  " << rejectedOrders << " (" << rejectedUnits << " units)\n";
  out << "Profit earned:    " << completedProfit << " rub\n";
  out << "Time spent:       " << completedTime << " min\n";
}
