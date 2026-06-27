#include "commands.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
  ulanova::Dictionary makeComplement(const ulanova::Dictionary & lhs, const ulanova::Dictionary & rhs)
  {
    ulanova::Dictionary result;

    for (ulanova::Dictionary::const_iterator it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      if (rhs.find(it->first) == rhs.cend())
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  ulanova::Dictionary makeIntersect(const ulanova::Dictionary & lhs, const ulanova::Dictionary & rhs)
  {
    ulanova::Dictionary result;

    for (ulanova::Dictionary::const_iterator it = lhs.cbegin(); it != lhs.cend(); ++it)
    {
      if (rhs.find(it->first) != rhs.cend())
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  ulanova::Dictionary makeUnion(const ulanova::Dictionary & lhs, const ulanova::Dictionary & rhs)
  {
    ulanova::Dictionary result(lhs);

    for (ulanova::Dictionary::const_iterator it = rhs.cbegin(); it != rhs.cend(); ++it)
    {
      if (result.find(it->first) == result.cend())
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }
}


void ulanova::print(std::ostream & out, std::istream & in, Storage & storage)
{
  std::string name;
  in >> name;

  Dictionary dict = storage.at(name);

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
}

void ulanova::loadStorage(Storage & storage, std::istream & input)
{
  std::string name;
  while (input >> name)
  {
    ulanova::Dictionary dict;

    int key = 0;
    std::string value;
    while (input.peek() != '\n' && input.peek() != EOF && input >> key >> value)
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

  Dictionary lhs = storage.at(lhs_name);
  Dictionary rhs = storage.at(rhs_name);
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

  Dictionary lhs = storage.at(lhs_name);
  Dictionary rhs = storage.at(rhs_name);

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

  Dictionary lhs = storage.at(lhs_name);
  Dictionary rhs = storage.at(rhs_name);

  storage.push(new_name, makeUnion(lhs, rhs));
}

