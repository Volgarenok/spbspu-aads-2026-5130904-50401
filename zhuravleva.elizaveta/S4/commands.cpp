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
    out << "<INVALID COMMAND>" << '\n';
    return;
  }
  const Dictionary& dict = dictIt->second;
  if(dict.empty())
  {
    out << "<EMPTY>" << '\n';
    return;
  }
  out << name;
  for (Dictionary::constIterator it = dict.cbegin(); it != dict.cend();
      ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
  out << '\n';
}

void zhuravleva::complement(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
  if (dictionaries.contains(newName))
  {
    throw std::runtime_error("dataset already exists");
  }
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
  dictionaries.push(newName, result);
}

void zhuravleva::intersect(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
  if (dictionaries.contains(newName))
  {
    throw std::runtime_error("dataset already exists");
  }
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
  dictionaries.push(newName, result);
}

void zhuravleva::unite(
  DictionaryStorage& dictionaries,
  const std::string& newName,
  const std::string& firstName,
  const std::string& secondName
)
{
    if (dictionaries.contains(newName))
  {
    throw std::runtime_error("dataset already exists");
  }
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
  dictionaries.push(newName, result);
}

void zhuravleva::processCommand(
  std::istream& in,
  std::ostream& out,
  DictionaryStorage& dictionaries
)
{
  std::string command;
  in >> command;
  if (!in)
  {
    return;
  }
  try
  {
    if (command == "print")
    {
      std::string name;
      in >> name;
      if (!in)
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }
      print(out, name, dictionaries);
    }
    else if (command == "complement")
    {
      std::string newName;
      in >> newName;
      std::string name1;
      in >> name1;
      std::string name2;
      in >> name2;
      if (!in)
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }
      complement(dictionaries, newName, name1, name2);
    }
    else if (command == "intersect")
    {
      std::string newName;
      in >> newName;
      std::string name1;
      in >> name1;
      std::string name2;
      in >> name2;
      if (!in)
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }
      intersect(dictionaries, newName, name1, name2);
    }
    else if (command == "union")
    {
      std::string newName;
      in >> newName;
      std::string name1;
      in >> name1;
      std::string name2;
      in >> name2;
      if (!in)
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }
      unite(dictionaries, newName, name1, name2);
    }
    else
    {
      out << "<INVALID COMMAND>" << '\n';
    }
  }
  catch(const std::exception& e)
  {
    out << "<INVALID COMMAND>" << '\n';
  }
}
