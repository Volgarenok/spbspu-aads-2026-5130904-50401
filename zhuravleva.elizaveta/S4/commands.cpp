#include "commands.hpp"
#include <iostream>
#include <string>
#include <stdexcept>

void zhuravleva::print(
  std::ostream& out,
  const std::string& name,
  const DictionaryStorage& dictionaries
)
{
  DictionaryStorage::constIterator dictIt = dictionaries.find(name);
  if (dictIt == dictionaries.cend())
  {
    throw std::runtime_error("dataset not found");
  }
  const Dictionary& dict = dictIt->second;
  if (dict.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (Dictionary::constIterator it = dict.cbegin(); it != dict.cend(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
}

void zhuravleva::complement(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
  DictionaryStorage::iterator firstIt = dictionaries.find(firstName);
  DictionaryStorage::iterator secondIt = dictionaries.find(secondName);
  if (firstIt == dictionaries.end() || secondIt == dictionaries.end())
  {
    throw std::runtime_error("dataset not found");
  }
  Dictionary result;
  for (Dictionary::constIterator it1 = firstIt->second.cbegin();
      it1 != firstIt->second.cend(); ++it1)
  {
    if (!secondIt->second.contains(it1->first))
    {
      result.push(it1->first, it1->second);
    }
  }
  if (dictionaries.contains(newName))
  {
    dictionaries.drop(newName);
  }
  dictionaries.push(newName, result);
}

void zhuravleva::intersect(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
  DictionaryStorage::iterator firstIt = dictionaries.find(firstName);
  DictionaryStorage::iterator secondIt = dictionaries.find(secondName);
  if (firstIt == dictionaries.end() || secondIt == dictionaries.end())
  {
    throw std::runtime_error("dataset not found");
  }
  Dictionary result;
  for (Dictionary::constIterator it1 = firstIt->second.cbegin(); it1 != firstIt->second.cend(); ++it1)
  {
    if (secondIt->second.contains(it1->first))
    {
      result.push(it1->first, it1->second);
    }
  }
  if (dictionaries.contains(newName))
  {
    dictionaries.drop(newName);
  }
  dictionaries.push(newName, result);
}

void zhuravleva::unite(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
  DictionaryStorage::iterator firstIt = dictionaries.find(firstName);
  DictionaryStorage::iterator secondIt = dictionaries.find(secondName);
  if (firstIt == dictionaries.end() || secondIt == dictionaries.end())
  {
    throw std::runtime_error("dataset not found");
  }
  Dictionary result;
  for (Dictionary::constIterator it1 = firstIt->second.cbegin(); it1 != firstIt->second.cend(); ++it1)
  {
    result.push(it1->first, it1->second);
  }
  for (Dictionary::constIterator it2 = secondIt->second.cbegin(); it2 != secondIt->second.cend(); ++it2)
  {
    if (!result.contains(it2->first))
    {
      result.push(it2->first, it2->second);
    }
  }
  if (dictionaries.contains(newName))
  {
    dictionaries.drop(newName);
  }
  dictionaries.push(newName, result);
}
