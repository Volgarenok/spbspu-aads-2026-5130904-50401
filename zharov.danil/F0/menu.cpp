#include "menu.hpp"
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include "cafe.hpp"

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

void zharov::menuAdd(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName, name, description;
  int price = 0, prepTime = 0;
  in >> menuName >> std::quoted(name) >> price;
  if (price <= 0)
  {
    throw std::invalid_argument("Price must be positive");
  }
  in >> prepTime;
  if (prepTime <= 0)
  {
    throw std::invalid_argument("Prep time must be positive");
  }
  if (hasMoreArgs(in))
  {
    in >> std::quoted(description);
  }
  if (!cafe.menus.has(menuName))
  {
    cafe.menus.add(menuName, zharov::MenuTable{});
  }
  zharov::MenuTable& menu = cafe.menus.at(menuName);
  if (menu.has(name))
  {
    throw std::invalid_argument("Item already exists");
  }
  menu.add(name, zharov::MenuItem{price, prepTime, description, true});
  out << "<OK: Added item \"" << name << "\">\n";
}

void zharov::menuRemove(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string name;
  in >> std::quoted(name);
  if (!cafe.menus.at(menuName).has(name))
  {
    throw std::invalid_argument("Item not found");
  }
  cafe.menus.at(menuName).remove(name);
  out << "<OK: Removed item \"" << name << "\">\n";
}

void zharov::menuPrice(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string name;
  in >> std::quoted(name);
  if (!cafe.menus.at(menuName).has(name))
  {
    throw std::invalid_argument("Item not found");
  }
  int newPrice = 0;
  in >> newPrice;
  if (newPrice <= 0)
  {
    throw std::invalid_argument("Price must be positive");
  }
  zharov::MenuItem& item = cafe.menus.at(menuName).at(name);
  out << "<OK: Price changed: " << item.price << " -> " << newPrice << ">\n";
  item.price = newPrice;
}

void zharov::menuTime(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string name;
  in >> std::quoted(name);
  if (!cafe.menus.at(menuName).has(name))
  {
    throw std::invalid_argument("Item not found");
  }
  int newTime = 0;
  in >> newTime;
  if (newTime <= 0)
  {
    throw std::invalid_argument("Prep time must be positive");
  }
  zharov::MenuItem& item = cafe.menus.at(menuName).at(name);
  out << "<OK: Prep time changed: " << item.prep_time << " -> " << newTime << " min>\n";
  item.prep_time = newTime;
}

void zharov::menuDesc(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string name;
  in >> std::quoted(name);
  if (!cafe.menus.at(menuName).has(name))
  {
    throw std::invalid_argument("Item not found");
  }
  std::string description;
  in >> std::quoted(description);
  cafe.menus.at(menuName).at(name).description = description;
  out << "<OK: Description updated>\n";
}

void zharov::menuAvailable(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string name;
  in >> std::quoted(name);
  if (!cafe.menus.at(menuName).has(name))
  {
    throw std::invalid_argument("Item not found");
  }
  std::string flag;
  in >> flag;
  if (flag != "yes" && flag != "no")
  {
    throw std::invalid_argument("Expected yes or no");
  }
  zharov::MenuItem& item = cafe.menus.at(menuName).at(name);
  item.available = (flag == "yes");
  out << "<OK: Item \"" << name << "\" " << (item.available ? "available" : "unavailable") << ">\n";
}

void zharov::menuShow(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  const zharov::MenuTable& menu = cafe.menus.at(menuName);
  if (hasMoreArgs(in))
  {
    std::string name;
    in >> std::quoted(name);
    if (!menu.has(name))
    {
      throw std::invalid_argument("Item not found");
    }
    const zharov::MenuItem& item = menu.at(name);
    out << name << " | " << item.price << " rub | " << item.prep_time << " min | "
        << (item.available ? "available" : "unavailable") << "\n";
  }
  else
  {
    std::ios::fmtflags savedFlags = out.flags();
    for (auto it = menu.cbegin(); it != menu.cend(); ++it)
    {
      out << std::left << std::setw(20) << it->first << std::right << " | " << std::setw(5)
          << it->second.price << " rub"
          << " | " << std::setw(3) << it->second.prep_time << " min"
          << " | " << (it->second.available ? "available" : "unavailable") << "\n";
    }
    out.flags(savedFlags);
  }
}

void zharov::menuExport(std::ostream& out, std::istream& in, const zharov::CafeSystem& cafe)
{
  std::string filename;
  in >> filename;
  std::string menuName;
  in >> menuName;
  if (!cafe.menus.has(menuName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::ofstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }
  const zharov::MenuTable& menu = cafe.menus.at(menuName);
  for (auto it = menu.cbegin(); it != menu.cend(); ++it)
  {
    file << it->first << "|" << it->second.price << "|" << it->second.prep_time << "|"
         << it->second.description << "|" << (it->second.available ? "yes" : "no") << "\n";
  }
  out << "<OK: Exported menu \"" << menuName << "\" to " << filename << ">\n";
}

void zharov::menuImport(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string filename, menuName;
  in >> filename >> menuName;
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }
  if (!cafe.menus.has(menuName))
  {
    cafe.menus.add(menuName, zharov::MenuTable{});
  }
  zharov::MenuTable& menu = cafe.menus.at(menuName);
  size_t count = 0;
  std::string line;
  while (std::getline(file, line))
  {
    std::string name, description, availFlag;
    int price = 0, prepTime = 0;
    size_t p1 = line.find('|');
    size_t p2 = line.find('|', p1 + 1);
    size_t p3 = line.find('|', p2 + 1);
    size_t p4 = line.find('|', p3 + 1);
    if (p1 == std::string::npos || p2 == std::string::npos || p3 == std::string::npos ||
      p4 == std::string::npos)
    {
      continue;
    }
    name = line.substr(0, p1);
    price = std::stoi(line.substr(p1 + 1, p2 - p1 - 1));
    prepTime = std::stoi(line.substr(p2 + 1, p3 - p2 - 1));
    description = line.substr(p3 + 1, p4 - p3 - 1);
    availFlag = line.substr(p4 + 1);
    if (!menu.has(name))
    {
      menu.add(name, zharov::MenuItem{price, prepTime, description, availFlag == "yes"});
      ++count;
    }
  }
  out << "<OK: Loaded " << count << " items>\n";
}

void zharov::menuCopy(std::ostream& out, std::istream& in, zharov::CafeSystem& cafe)
{
  std::string srcName;
  in >> srcName;
  if (!cafe.menus.has(srcName))
  {
    throw std::invalid_argument("Menu not found");
  }
  std::string dstName;
  in >> dstName;
  if (cafe.menus.has(dstName))
  {
    throw std::invalid_argument("Menu already exists");
  }
  cafe.menus.add(dstName, cafe.menus.at(srcName));
  out << "<OK: Menu \"" << srcName << "\" copied to \"" << dstName << "\">\n";
}

void zharov::menuList(std::ostream& out, std::istream&, const zharov::CafeSystem& cafe)
{
  for (auto it = cafe.menus.cbegin(); it != cafe.menus.cend(); ++it)
  {
    out << it->first << "\n";
  }
}
