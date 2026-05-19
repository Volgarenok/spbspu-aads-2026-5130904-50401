#include "commands.hpp"

#include <stdexcept>
#include <utility>

void studilova::Print(std::istream& in, std::ostream& out, Datasets& datasets)
{
  std::string name;
  if (!(in >> name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dataset& dataset = datasets.get(name);

  if (dataset.empty())
  {
    out << "EMPTY\n";
    return;
  }

  out << name;

  for (Dataset::CIt it = dataset.cbegin(); it != dataset.cend(); ++it)
  {
    std::pair< const int&, const std::string& > pair = *it;
    out << ' ' << pair.first << ' ' << pair.second;
  }

  out << '\n';
}

void studilova::Complement(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string new_name;
  std::string first_name;
  std::string second_name;

  if (!(in >> new_name >> first_name >> second_name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dataset& first = datasets.get(first_name);
  const Dataset& second = datasets.get(second_name);

  Dataset result;

  for (Dataset::CIt it = first.cbegin(); it != first.cend(); ++it)
  {
    std::pair< const int&, const std::string& > pair = *it;
    try
    {
      second.get(pair.first);
    }
    catch (const std::out_of_range&)
    {
      result.push(pair.first, pair.second);
    }
  }
  datasets.push(new_name, result);
}

void studilova::Intersect(std::istream& in, std::ostream&, Datasets& datasets)
{
  std::string new_name;
  std::string first_name;
  std::string second_name;

  if (!(in >> new_name >> first_name >> second_name))
  {
    throw std::runtime_error("invalid input");
  }

  const Dataset& first = datasets.get(first_name);
  const Dataset& second = datasets.get(second_name);

  Dataset result;

  for (Dataset::CIt it = first.cbegin(); it != first.cend(); ++it)
  {
    std::pair< const int&, const std::string& > pair = *it;
    try
    {
      second.get(pair.first);
      result.push(pair.first, pair.second);
    }
    catch(const std::out_of_range&)
    {}
  }
  datasets.push(new_name, result);
}