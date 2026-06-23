#include "datasets.hpp"

#include <istream>
#include <ostream>
#include <stdexcept>

namespace
{
  using Dictionary = kondrat::Dictionary;

  Dictionary makeComplement(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      if (!rhs.contains(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary makeIntersect(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      if (rhs.contains(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }

  Dictionary makeUnion(const Dictionary & lhs, const Dictionary & rhs)
  {
    Dictionary result;

    for (Dictionary::const_iterator it = lhs.begin(); it != lhs.end(); ++it)
    {
      result.push(it->first, it->second);
    }

    for (Dictionary::const_iterator it = rhs.begin(); it != rhs.end(); ++it)
    {
      if (!result.contains(it->first))
      {
        result.push(it->first, it->second);
      }
    }

    return result;
  }
}

void kondrat::print(std::istream & in, std::ostream & out, Collection & datasets)
{
  std::string name;

  if (!(in >> name))
  {
    throw std::logic_error("invalid command");
  }

  const Dictionary & dict = datasets.at(name);

  if (dict.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << name;

  for (Dictionary::const_iterator it = dict.begin(); it != dict.end(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }

  out << '\n';
}

void kondrat::complement(std::istream & in, std::ostream &, Collection & datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;

  if (!(in >> newName >> lhsName >> rhsName))
  {
    throw std::logic_error("invalid command");
  }

  Dictionary result = makeComplement(datasets.at(lhsName), datasets.at(rhsName));
  datasets.push(newName, result);
}

void kondrat::intersect(std::istream & in, std::ostream &, Collection & datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;

  if (!(in >> newName >> lhsName >> rhsName))
  {
    throw std::logic_error("invalid command");
  }

  Dictionary result = makeIntersect(datasets.at(lhsName), datasets.at(rhsName));
  datasets.push(newName, result);
}

void kondrat::unionCollections(std::istream & in, std::ostream &, Collection & datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;

  if (!(in >> newName >> lhsName >> rhsName))
  {
    throw std::logic_error("invalid command");
  }

  Dictionary result = makeUnion(datasets.at(lhsName), datasets.at(rhsName));
  datasets.push(newName, result);
}
