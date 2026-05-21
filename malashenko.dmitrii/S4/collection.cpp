#include "collection.hpp"

void malashenko::print(std::istream& in, std::ostream& out, Collection& datasets)
{
  std::string datasetName;
  if (!(in >> datasetName))
  {
    throw std::invalid_argument("input problems");
  }

  Dataset ds = datasets.get(datasetName);
  if (ds.empty())
  {
    out << "<EMPTY>\n";
    return;
  }

  out << datasetName;
  for (BSTreeIter< int, std::string > b = ds.begin(); b != ds.end(); ++b)
  {
    out << ' ' << b->first << ' ' << b->second;
  }
  out << '\n';
}

void malashenko::complement(std::istream& in, std::ostream&, Collection& datasets)
{
  std::string newDsName, dsName1, dsName2;
  if (!(in >> newDsName >> dsName1 >> dsName2))
  {
    throw std::invalid_argument("input problems");
  }

  Dataset ds1 = datasets.get(dsName1);
  Dataset ds2 = datasets.get(dsName2);

  Dataset newDs;

  for (BSTreeIter< int, std::string > b = ds1.begin(); b != ds1.end(); ++b)
  {
    try
    {
      ds2.get(b->first);
    }
    catch(...)
    {
      newDs.push(b->first, b->second);
    }
  }
  datasets.push(newDsName, newDs);
}

void malashenko::intersect(std::istream& in, std::ostream&, Collection& datasets)
{
  std::string newDsName, dsName1, dsName2;
  if (!(in >> newDsName >> dsName1 >> dsName2))
  {
    throw std::invalid_argument("input problems");
  }

  Dataset ds1 = datasets.get(dsName1);
  Dataset ds2 = datasets.get(dsName2);

  Dataset newDs;

  for (BSTreeIter< int, std::string > b = ds1.begin(); b != ds1.end(); ++b)
  {
    try
    {
      ds2.get(b->first);
      newDs.push(b->first, b->second);
    }
    catch(...)
    {
    }
  }
  datasets.push(newDsName, newDs);
}

void malashenko::unionCollections(std::istream& in, std::ostream&, Collection& datasets)
{
  std::string newDsName, dsName1, dsName2;
  if (!(in >> newDsName >> dsName1 >> dsName2))
  {
    throw std::invalid_argument("input problems");
  }

  Dataset ds1 = datasets.get(dsName1);
  Dataset ds2 = datasets.get(dsName2);

  Dataset newDs = ds2;

  for (BSTreeIter< int, std::string > b = ds1.begin(); b != ds1.end(); ++b)
  {
    newDs.push(b->first, b->second);
  }
  datasets.push(newDsName, newDs);
}

