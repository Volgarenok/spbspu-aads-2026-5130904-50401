#include "commands.hpp"
#include <stdexcept>

void burukov::cmdPrint(std::istream& in, std::ostream& out, Dicts& dicts)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dict& d = dicts.at(name);
  if (d.empty())
  {
    out << "<EMPTY>" << '\n';
    return;
  }

  out << name;
  for (auto it = d.begin(); it != d.end(); ++it)
  {
    auto p = *it;
    out << ' ' << p.first << ' ' << p.second;
  }
  out << '\n';
}

void burukov::cmdComplement(std::istream& in, std::ostream&, Dicts& dicts)
{
  std::string new_name, a_name, b_name;
  if (!(in >> new_name >> a_name >> b_name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dict& a = dicts.at(a_name);
  const Dict& b = dicts.at(b_name);
  Dict res;

  for (auto it = a.begin(); it != a.end(); ++it)
  {
    auto p = *it;
    try
    {
      b.at(p.first);
    }
    catch (const std::out_of_range&)
    {
      res.push(p.first, p.second);
    }
  }

  dicts.push(new_name, std::move(res));
}

void burukov::cmdIntersect(std::istream& in, std::ostream&, Dicts& dicts)
{
  std::string new_name, a_name, b_name;
  if (!(in >> new_name >> a_name >> b_name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dict& a = dicts.at(a_name);
  const Dict& b = dicts.at(b_name);
  Dict res;

  for (auto it = a.begin(); it != a.end(); ++it)
  {
    auto p = *it;
    try
    {
      b.at(p.first);
      res.push(p.first, p.second);
    }
    catch (const std::out_of_range&)
    {}
  }

  dicts.push(new_name, std::move(res));
}

void burukov::cmdUnion(std::istream& in, std::ostream&, Dicts& dicts)
{
  std::string new_name, a_name, b_name;
  if (!(in >> new_name >> a_name >> b_name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dict& a = dicts.at(a_name);
  const Dict& b = dicts.at(b_name);
  Dict res;

  for (auto it = a.begin(); it != a.end(); ++it)
  {
    auto p = *it;
    res.push(p.first, p.second);
  }

  for (auto it = b.begin(); it != b.end(); ++it)
  {
    auto p = *it;
    try
    {
      res.at(p.first);
    }
    catch (const std::out_of_range&)
    {
      res.push(p.first, p.second);
    }
  }

  dicts.push(new_name, std::move(res));
}
