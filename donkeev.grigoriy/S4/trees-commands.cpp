#include <iostream>
#include <stdexcept>

#include "trees-commands.hpp"

void donkeev::printDicts(std::istream& input, std::ostream& output, donkeev::Datasets& dicts)
{
  std::string datasetName;
  if (!(input >> datasetName))
  {
    throw std::runtime_error("Bad input");
  }

  Datasets::iterator it = dicts.find(datasetName);
  if (it == dicts.end())
  {
    throw std::runtime_error("No such dictionary");
  }

  Dataset& tree = it->second;

  if (!tree.empty())
  {
    output << datasetName;
  }
  for (Dataset::iterator begin = tree.begin(); begin != tree.end(); ++begin)
  {
    output << " " << begin->first << " " << begin->second;
  }

  if (!tree.empty())
  {
    output << "\n";
  } 
}

void donkeev::complementDicts(std::istream& input, std::ostream&, donkeev::Datasets& dicts)
{
  std::string newName, name1, name2;
  if (!(input >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Bad input");
  }

  Datasets::iterator it1 = dicts.find(name1);
  Datasets::iterator it2 = dicts.find(name2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("Bad input");
  }

  const Dataset& dict1 = it1->second;
  const Dataset& dict2 = it2->second;

  Dataset result;

  Dataset::constIterator begin = dict1.begin();
  Dataset::constIterator end = dict1.end();
  while (begin != end)
  {
    int key = begin->first;
    const std::string& value = begin->second;

    Dataset::constIterator found = dict2.find(key);
    if (found == dict2.end())
    {
      result.push(key, value);
    }

    ++begin;
  }

  Datasets::iterator existing = dicts.find(newName);
  if (existing != dicts.end())
  {
    dicts.drop(newName);
  }

  dicts.push(newName, std::move(result));
}

void donkeev::intersectDicts(std::istream& input, std::ostream&, donkeev::Datasets& dicts)
{
  std::string newName, name1, name2;
  if (!(input >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Bad input");
  }

  Datasets::iterator it1 = dicts.find(name1);
  Datasets::iterator it2 = dicts.find(name2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("Bad input");
  }

  const Dataset& dict1 = it1->second;
  const Dataset& dict2 = it2->second;

  Dataset result;

  Dataset::constIterator begin1 = dict1.begin();
  Dataset::constIterator end1 = dict1.end();

  while (begin1 != end1)
  {
    int key = begin1->first;
    const std::string& value = begin1->second;

    Dataset::constIterator found = dict2.find(key);
    if (found != dict2.end())
    {
      result.push(key, value);
    }

    ++begin1;
  }

  Datasets::iterator existing = dicts.find(newName);
  if (existing != dicts.end())
  {
    dicts.drop(newName);
  }

  dicts.push(newName, std::move(result));
}

void donkeev::uniteDicts(std::istream& input, std::ostream&, donkeev::Datasets& dicts)
{
  std::string newName, name1, name2;
  if (!(input >> newName >> name1 >> name2))
  {
    throw std::runtime_error("Bad input");
  }

  Datasets::iterator it1 = dicts.find(name1);
  Datasets::iterator it2 = dicts.find(name2);
  if (it1 == dicts.end() || it2 == dicts.end())
  {
    throw std::runtime_error("Bad input");
  }

  const Dataset& dict1 = it1->second;
  const Dataset& dict2 = it2->second;

  Dataset result;

  Dataset::constIterator begin1 = dict1.begin();
  Dataset::constIterator end1 = dict1.end();

  while (begin1 != end1)
  {
    int key = begin1->first;
    const std::string& value = begin1->second;
    result.push(key, value);
    ++begin1;
  }

  Dataset::constIterator begin2 = dict2.begin();
  Dataset::constIterator end2 = dict2.end();

  while (begin2 != end2)
  {
    int key = begin2->first;
    const std::string& value = begin2->second;

    Dataset::iterator found = result.find(key);
    if (found == result.end())
    {
      result.push(key, value);
    }

    ++begin2;
  }

  Datasets::iterator existing = dicts.find(newName);
  if (existing != dicts.end())
  {
    dicts.drop(newName);
  }

  dicts.push(newName, std::move(result));
}
