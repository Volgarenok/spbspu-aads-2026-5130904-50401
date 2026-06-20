#include "commands.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sstream>

bool ulanova::IntCompare::operator()(int lhs, int rhs) const
{
  return lhs < rhs;
}

bool ulanova::StringCompare::operator()(const std::string & lhs, const std::string & rhs) const
{
  return lhs < rhs;
}

void ulanova::print(std::ostream & out, std::istream & in, Storage & storage)
{
  std::string name;
  in >> name;

  Dictionary dict = storage.get(name);

  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << name;
  for (Dictionary::const_iterator it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
  out << '\n';
}

namespace ulanova
{
  ulanova::Dictionary makeComplement(
    const ulanova::Dictionary & lhs,
    const ulanova::Dictionary & rhs
  )
  {
    ulanova::Dictionary result;

    for (ulanova::Dictionary::const_iterator it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      try
      {
        rhs.get(it->first);
      }
      catch (const std::out_of_range &)
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  ulanova::Dictionary makeIntersect(
    const ulanova::Dictionary & lhs,
    const ulanova::Dictionary & rhs
  )
  {
    ulanova::Dictionary result;

    for (ulanova::Dictionary::const_iterator it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      try
      {
        rhs.get(it->first);
        result.push(it->first, it->second);
      }
      catch (const std::out_of_range &)
      {}
    }

    return result;
  }

  ulanova::Dictionary makeUnion(
    const ulanova::Dictionary & lhs,
    const ulanova::Dictionary & rhs
  )
  {
    ulanova::Dictionary result(lhs);

    for (ulanova::Dictionary::const_iterator it = rhs.cbegin(); it != rhs.cend(); ++it)
    {
      try
      {
        result.get(it->first);
      }
      catch (const std::out_of_range &)
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }
}

void ulanova::loadStorage(Storage & storage, const std::string & filename)
{
  std::ifstream input(filename);
  if (!input)
  {
    throw std::runtime_error("file open error");
  }

  std::string name;
  while (input >> name)
  {
    Dictionary dict;

    std::string line;
    std::getline(input, line);

    std::istringstream line_stream(line);

    int key = 0;
    std::string value;
    while (line_stream >> key >> value)
    {
      dict.push(key, value);
    }

    storage.push(name, dict);
  }
}

void ulanova::complement(std::ostream &, std::istream & in, Storage & storage)
{
  std::string new_name;
  std::string lhs_name;
  std::string rhs_name;

  if (!(in >> new_name >> lhs_name >> rhs_name))
  {
    throw std::runtime_error("invalid command");
  }

  Dictionary lhs = storage.get(lhs_name);
  Dictionary rhs = storage.get(rhs_name);

  storage.push(new_name, makeComplement(lhs, rhs));
}

void ulanova::intersect(std::ostream &, std::istream & in, Storage & storage)
{
  std::string new_name;
  std::string lhs_name;
  std::string rhs_name;

  if (!(in >> new_name >> lhs_name >> rhs_name))
  {
    throw std::runtime_error("invalid command");
  }

  Dictionary lhs = storage.get(lhs_name);
  Dictionary rhs = storage.get(rhs_name);

  storage.push(new_name, makeIntersect(lhs, rhs));
}

void ulanova::unionDicts(std::ostream &, std::istream & in, Storage & storage)
{
  std::string new_name;
  std::string lhs_name;
  std::string rhs_name;

  if (!(in >> new_name >> lhs_name >> rhs_name))
  {
    throw std::runtime_error("invalid command");
  }

  Dictionary lhs = storage.get(lhs_name);
  Dictionary rhs = storage.get(rhs_name);

  storage.push(new_name, makeUnion(lhs, rhs));
}

