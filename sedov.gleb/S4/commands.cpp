#include "commands.hpp"
#include <stdexcept>
#include <utility>

void sedov::print(std::istream & in, std::ostream & out, sedov::trees & trees)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("Invalid input");
  }
  const sedov::pairs & ps = trees.at(name);
  if (ps.empty())
  {
    out << "<EMPTY>";
  }
  else
  {
    out << name;
    for (auto it = ps.cbegin(); it != ps.cend(); ++it)
    {
      auto pair = *it;
      out << ' ' << pair.first << ' ' << pair.second;
    }
  }
}

void sedov::complement(std::istream & in, std::ostream &, sedov::trees & trees)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }
  const sedov::pairs & ps1 = trees.at(name1);
  const sedov::pairs & ps2 = trees.at(name2);
  sedov::pairs new_ps;
  for (auto it = ps1.cbegin(); it != ps1.cend(); ++it)
  {
    auto pair = *it;
    try
    {
      ps2.at(pair.first);
    }
    catch (const std::out_of_range &)
    {
      new_ps.insert(pair.first, pair.second);
    }
  }
  trees.insert(newName, std::move(new_ps));
}

void sedov::intersect(std::istream & in, std::ostream &, sedov::trees & trees)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }
  const sedov::pairs & ps1 = trees.at(name1);
  const sedov::pairs & ps2 = trees.at(name2);
  sedov::pairs new_ps;
  for (auto it = ps1.cbegin(); it != ps1.cend(); ++it)
  {
    auto pair = *it;
    try
    {
      ps2.at(pair.first);
      new_ps.insert(pair.first, pair.second);
    }
    catch (const std::out_of_range &)
    {}
  }
  trees.insert(newName, std::move(new_ps));
}

void sedov::cUnion(std::istream & in, std::ostream &, sedov::trees & trees)
{
  std::string newName, name1, name2;
  if (!(in >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Invalid input");
  }
  const sedov::pairs & ps1 = trees.at(name1);
  const sedov::pairs & ps2 = trees.at(name2);
  sedov::pairs new_ps;
  for (auto it = ps1.cbegin(); it != ps1.cend(); ++it)
  {
    auto pair = *it;
    new_ps.insert(pair.first, pair.second);
  }
  for (auto it = ps2.cbegin(); it != ps2.cend(); ++it)
  {
    auto pair = *it;
    try
    {
      new_ps.at(pair.first);
    }
    catch (const std::out_of_range &)
    {
      new_ps.insert(pair.first, pair.second);
    }
  }
  trees.insert(newName, std::move(new_ps));
}
