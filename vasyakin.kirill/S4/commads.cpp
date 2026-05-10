#include "commands.hpp"
#include <stdexcept>
#include <utility>

void vasyakin::cmdPrint(std::istream& in, std::ostream& out, Datasets& datasets)
{
  std::string map_name;
  if (!(in >> map_name))
  {
    throw std::runtime_error("Invalid input");
  }

  const auto& ds = datasets.get(map_name);

  if (ds.empty())
  {
    out << "<EMPTY>" << '\n';
    return;
  }
  else
  {
    out << map_name;

    for (auto cit = ds.cbegin(); cit != ds.cend(); ++cit)
    {
      const auto& pair = *cit;
      out << ' ' << pair.first << ' ' << pair.second;
    }
    out << '\n';
  }
}

void vasyakin::cmdComplement(std::istream& in, std::ostream& out, vasyakin::Datasets& datasets)
{
  std::string new_map_name, map_name1, map_name2;
  if (!(in >> new_map_name >> map_name1 >> map_name2))
  {
    throw std::runtime_error("Invalid input");
  }

  const auto& ds1 = datasets.get(map_name1);
  const auto& ds2 = datasets.get(map_name2);
  vasyakin::Dataset new_ds;

  for (auto cit = ds1.cbegin(); cit != ds1.cend(); ++cit)
  {
    const auto& pair = *cit;

    try
    {
      ds2.get(pair.first);
    }
    catch (const std::out_of_range&)
    {
      new_ds.push(pair.first, pair.second);
    }
  }

  datasets.push(new_map_name, std::move(new_ds));
}

void vasyakin::cmdIntersect(std::istream& in, std::ostream& out, vasyakin::Datasets& datasets)
{
  std::string new_map_name, map_name1, map_name2;
  if (!(in >> new_map_name >> map_name1 >> map_name2))
  {
    throw std::runtime_error("Invalid input");
  }

  const auto& ds1 = datasets.get(map_name1);
  const auto& ds2 = datasets.get(map_name2);
  vasyakin::Dataset new_ds;

  for (auto cit = ds1.cbegin(); cit != ds1.cend(); ++cit)
  {
    const auto& pair = *cit;

    try
    {
      ds2.get(pair.first);
      new_ds.push(pair.first, pair.second);
    }
    catch (const std::out_of_range&)
    {
    }
  }

  datasets.push(new_map_name, new_ds);
}

void vasyakin::cmdUnion(std::istream& in, std::ostream& out, vasyakin::Datasets& datasets)
{
  std::string new_map_name, map_name1, map_name2;
  if (!(in >>new_map_name >> map_name1 >> map_name2))
  {
    throw std::runtime_error("Invalid input");
  }

  const auto& ds1 = datasets.get(map_name1);
  const auto& ds2 = datasets.get(map_name2);
  vasyakin::Dataset new_ds;

  for (auto cit = ds1.cbegin(); cit != ds1.cend(); ++cit)
  {
    const auto& pair = *cit;
    new_ds.push(pair.first, pair.second);
  }

  for (auto cit = ds2.cbegin(); cit != ds2.cend(); ++cit)
  {
    const auto& pair = *cit;
    try
    {
      new_ds.get(pair.first);
    }
    catch (const std::out_of_range&)
    {
      new_ds.push(pair.first, pair.second);
    }
  }

  datasets.push(new_map_name, new_ds);
}
