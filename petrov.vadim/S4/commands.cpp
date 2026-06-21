#include "commands.hpp"

using Citerator = petrov::BSTCIterator< int, std::string >;

void petrov::print(std::istream& in, std::ostream& out, collection& coll)
{
  std::string map_name;
  if (!(in >> map_name))
  {
    throw std::runtime_error("invalid arguments");
  }

  const dataset& map = coll.get(map_name);
  if (map.empty())
  {
    out << "<EMPTY>" << "\n";
    return;
  }

  out << map_name;
  Citerator it = map.begin();
  for (; it != map.end(); ++it)
  {
    out << " " << (*it).first << " " << (*it).second;
  }
  out << "\n";
}

void petrov::complement(std::istream& in, std::ostream&, collection& coll)
{
  std::string n_data, data1, data2;
  if (!(in >> n_data >> data1 >> data2))
  {
    throw std::runtime_error("invalid arguments");
  }


  const dataset& map1 = coll.get(data1);
  const dataset& map2 = coll.get(data2);
  dataset new_map;

  for (Citerator it = map1.begin(); it != map1.end(); ++it)
  {
    try
    {
      map2.get((*it).first);
    }
    catch(const std::exception& e)
    {
      new_map.push((*it).first, (*it).second);
    }
  }

  coll.push(n_data, new_map);
}

void petrov::intersect(std::istream& in, std::ostream&, collection& coll)
{
  std::string n_data, data1, data2;
  if (!(in >> n_data >> data1 >> data2))
  {
    throw std::runtime_error("invalid arguments");
  }

  const dataset& map1 = coll.get(data1);
  const dataset& map2 = coll.get(data2);
  dataset new_map;

  for (Citerator it = map1.begin(); it != map1.end(); ++it)
  {
    try
    {
      map2.get((*it).first);
      new_map.push((*it).first, (*it).second);
    }
    catch(const std::exception& e)
    {}
  }
  coll.push(n_data, new_map);
}

void petrov::unionn(std::istream& in, std::ostream&, collection& coll)
{
  std::string n_data, data1, data2;
  if (!(in >> n_data >> data1 >> data2))
  {
    throw std::runtime_error("invalid arguments");
  }

  const dataset& map1 = coll.get(data1);
  const dataset& map2 = coll.get(data2);
  dataset new_map = map1;

  for (Citerator it = map2.begin(); it != map2.end(); ++it)
  {
    try
    {
      new_map.get((*it).first);
    }
    catch(const std::exception& e)
    {
      new_map.push((*it).first, (*it).second);
    }
  }

  coll.push(n_data, new_map);
}
