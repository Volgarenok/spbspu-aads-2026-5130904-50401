#include "functions.hpp"
#include <istream>
#include <memory>
#include <ostream>
#include <stdexcept>

namespace
{
  std::string readSubstr(const std::string& line, size_t& pos)
  {
    size_t end = line.find(' ', pos);
    if (end == std::string::npos)
    {
      std::string substr = line.substr(pos);
      pos = line.size();
      return substr;
    }
    std::string substr = line.substr(pos, end - pos);
    pos = end + 1;
    return substr;
  }
}

tarasenko::Datasets tarasenko::getFromFile(std::istream& input)
{
  Datasets datasets;
  std::string line;
  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }
    size_t pos = 0;
    std::string name = readSubstr(line, pos);
    if (name.empty())
    {
      throw std::runtime_error("Incorrect dataset description");
    }

    Dataset dataset;
    while (pos < line.size())
    {
      std::string key = readSubstr(line, pos);
      std::string value = readSubstr(line, pos);
      if (key.empty() || value.empty())
      {
        throw std::runtime_error("Incorrect dataset description");
      }
      size_t read = 0;
      int keyNumber = std::stoi(key, std::addressof(read));
      if (read != key.size())
      {
        throw std::runtime_error("Incorrect dataset description");
      }
      dataset.add(keyNumber, value);
    }

    datasets.add(name, dataset);
  }

  return datasets;
}

void tarasenko::print(std::istream& in, std::ostream& out, Datasets& datasets)
{
  std::string name;
  in >> name;
  if (!in || !datasets.has(name))
  {
    throw std::runtime_error("Invalid command");
  }

  Dataset& dataset = datasets.at(name);
  if (dataset.empty())
  {
    out << "<EMPTY>";
    return;
  }
  out << name;
  for (auto it = dataset.begin(); it != dataset.end(); ++it)
  {
    out << ' ' << it->first << ' ' << it->second;
  }
}

void tarasenko::complement(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    if (!rhs.has(it->first))
    {
      result.add(it->first, it->second);
    }
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}

void tarasenko::intersect(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    if (rhs.has(it->first))
    {
      result.add(it->first, it->second);
    }
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}

void tarasenko::unionFunc(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string newName;
  std::string lhsName;
  std::string rhsName;
  in >> newName >> lhsName >> rhsName;
  if (!in || !datasets.has(lhsName) || !datasets.has(rhsName))
  {
    throw std::runtime_error("Invalid command");
  }

  const Dataset& lhs = datasets.at(lhsName);
  const Dataset& rhs = datasets.at(rhsName);
  Dataset result;
  for (auto it = lhs.begin(); it != lhs.end(); ++it)
  {
    result.add(it->first, it->second);
  }
  for (auto it = rhs.begin(); it != rhs.end(); ++it)
  {
    result.add(it->first, it->second);
  }
  if (datasets.has(newName))
  {
    datasets.at(newName) = result;
  }
  else
  {
    datasets.add(newName, result);
  }
}
