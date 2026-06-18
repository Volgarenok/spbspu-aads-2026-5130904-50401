#include "dataset.hpp"

#include <ostream>

void lukashevich::DatasetTable::set(
    const std::string & name, const DataSet & dataSet)
{
  dataSets_.push(name, dataSet);
}

const lukashevich::DataSet & lukashevich::DatasetTable::at(
    const std::string & name) const
{
  return dataSets_.at(name);
}

void lukashevich::DatasetTable::print(
    const std::string & name, std::ostream & output) const
{
  const DataSet & dataSet = at(name);
  if (dataSet.empty())
  {
    output << "<EMPTY>\n";
    return;
  }

  output << name;
  DataSet::const_iterator iterator = dataSet.begin();
  while (iterator != dataSet.end())
  {
    output << ' ' << iterator->first << ' ' << iterator->second;
    ++iterator;
  }
  output << '\n';
}

void lukashevich::DatasetTable::complement(
    const std::string & newName,
    const std::string & lhsName,
    const std::string & rhsName)
{
  const DataSet & lhs = at(lhsName);
  const DataSet & rhs = at(rhsName);
  DataSet result;

  DataSet::const_iterator iterator = lhs.begin();
  while (iterator != lhs.end())
  {
    if (!rhs.contains(iterator->first))
    {
      result.push(iterator->first, iterator->second);
    }
    ++iterator;
  }
  set(newName, result);
}

void lukashevich::DatasetTable::intersect(
    const std::string & newName,
    const std::string & lhsName,
    const std::string & rhsName)
{
  const DataSet & lhs = at(lhsName);
  const DataSet & rhs = at(rhsName);
  DataSet result;

  DataSet::const_iterator iterator = lhs.begin();
  while (iterator != lhs.end())
  {
    if (rhs.contains(iterator->first))
    {
      result.push(iterator->first, iterator->second);
    }
    ++iterator;
  }
  set(newName, result);
}

void lukashevich::DatasetTable::unite(
    const std::string & newName,
    const std::string & lhsName,
    const std::string & rhsName)
{
  const DataSet & lhs = at(lhsName);
  const DataSet & rhs = at(rhsName);
  DataSet result;

  DataSet::const_iterator lhsIterator = lhs.begin();
  while (lhsIterator != lhs.end())
  {
    result.push(lhsIterator->first, lhsIterator->second);
    ++lhsIterator;
  }

  DataSet::const_iterator rhsIterator = rhs.begin();
  while (rhsIterator != rhs.end())
  {
    if (!result.contains(rhsIterator->first))
    {
      result.push(rhsIterator->first, rhsIterator->second);
    }
    ++rhsIterator;
  }
  set(newName, result);
}
