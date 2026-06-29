#include "sets_functions.hpp"
#include <iostream>
#include <string>
#include "mybst.hpp"

namespace
{
  void skipSpaces(std::istream& in)
  {
    while(in.peek() == ' ')
    {
      char a;
      in.get(a);
    }
  }
  bool checkEndLine(std::istream& in)
  {
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      return true;
    }
    return false;
  }
  bool checkEndLineWithSkipSpaces(std::istream& in)
  {
    skipSpaces(in);
    return checkEndLine(in);
  }
}

void goltsov::readSets(std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string name_set;
  while (in >> name_set)
  {
    all_sets.insert(std::pair< std::string, goltsov::BSTree< long long, std::string,
      std::less< long long > > >{name_set, goltsov::BSTree< long long, std::string, std::less< long long > >()});
    while (!checkEndLineWithSkipSpaces(in))
    {
      long long key;
      std::string value;
      if (!(in >> key >> value))
      {
        break;
      }
      all_sets.at(name_set).insert(std::pair< long long, std::string >{key, value});
      if (in.peek() == '\n' || in.peek() == EOF)
      {
        break;
      }
    }
  }
}

void goltsov::printParsing(std::ostream& out, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string name_dataset;
  if (!(in >> name_dataset))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  print_ds(out, all_sets, name_dataset);
  out << '\n';
}
void goltsov::complementParsing(std::ostream&, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  if (!(in >> new_dataset >> dataset_1 >> dataset_2))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  complement_ds(all_sets, new_dataset, dataset_1, dataset_2);
}
void goltsov::intersectParsing(std::ostream&, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  if (!(in >> new_dataset >> dataset_1 >> dataset_2))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  intersect_ds(all_sets, new_dataset, dataset_1, dataset_2);
}
void goltsov::unionParsing(std::ostream&, std::istream& in, goltsov::bst_s_bst_is& all_sets)
{
  std::string new_dataset, dataset_1, dataset_2;
  if (!(in >> new_dataset >> dataset_1 >> dataset_2))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  union_ds(all_sets, new_dataset, dataset_1, dataset_2);
}

void goltsov::print_ds(std::ostream& out, goltsov::bst_s_bst_is& all_sets, std::string dataset)
{
  try
  {
    if (all_sets.at(dataset).height() == 0)
    {
      out << "<EMPTY>";
      return;
    }
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  out << dataset;
  for (goltsov::bst_is_iterator it = all_sets.at(dataset).begin(); it != all_sets.at(dataset).end(); ++it)
  {
    out << " " << (*it).first << " " << (*it).second;
  }
}
void goltsov::complement_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, std::less< long long > > new_set;
  try
  {
    all_sets.at(dataset_1);
    all_sets.at(dataset_2);
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::bst_is_iterator it_1 = all_sets.at(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.at(dataset_2).begin();
  while (it_1 != all_sets.at(dataset_1).end() || it_2 != all_sets.at(dataset_2).end())
  {
    if (it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first == (*it_2).first)
    {
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first < (*it_2).first)
      || (it_1 != all_sets.at(dataset_1).end() && it_2 == all_sets.at(dataset_2).end()))
    {
      new_set.insert(std::pair< long long, std::string >{(*it_1).first, (*it_1).second});
      ++it_1;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first > (*it_2).first)
      || (it_1 == all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()))
    {
      ++it_2;
    }
  }
  all_sets[new_dataset] = new_set;
}
void goltsov::intersect_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1,
  std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, std::less< long long > > new_set;
  try
  {
    all_sets.at(dataset_1);
    all_sets.at(dataset_2);
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::bst_is_iterator it_1 = all_sets.at(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.at(dataset_2).begin();
  while (it_1 != all_sets.at(dataset_1).end() || it_2 != all_sets.at(dataset_2).end())
  {
    if (it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first == (*it_2).first)
    {
      new_set.insert(std::pair< long long, std::string >{(*it_1).first, (*it_1).second});
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first < (*it_2).first)
      || (it_1 != all_sets.at(dataset_1).end() && it_2 == all_sets.at(dataset_2).end()))
    {
      ++it_1;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first > (*it_2).first)
      || (it_1 == all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()))
    {
      ++it_2;
    }
  }
  all_sets[new_dataset] = new_set;
}
void goltsov::union_ds(bst_s_bst_is& all_sets, std::string new_dataset, std::string dataset_1, std::string dataset_2)
{
  goltsov::BSTree< long long, std::string, std::less< long long > > new_set;
  try
  {
    all_sets.at(dataset_1);
    all_sets.at(dataset_2);
  }
  catch (...)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::bst_is_iterator it_1 = all_sets.at(dataset_1).begin();
  goltsov::bst_is_iterator it_2 = all_sets.at(dataset_2).begin();
  while (it_1 != all_sets.at(dataset_1).end() || it_2 != all_sets.at(dataset_2).end())
  {
    if (it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first == (*it_2).first)
    {
      new_set.insert(std::pair< long long, std::string >{(*it_1).first, (*it_1).second});
      ++it_1;
      ++it_2;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first < (*it_2).first)
      || (it_1 != all_sets.at(dataset_1).end() && it_2 == all_sets.at(dataset_2).end()))
    {
      new_set.insert(std::pair< long long, std::string >{(*it_1).first, (*it_1).second});
      ++it_1;
    }
    if ((it_1 != all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()
      && (*it_1).first > (*it_2).first)
      || (it_1 == all_sets.at(dataset_1).end() && it_2 != all_sets.at(dataset_2).end()))
    {
      new_set.insert(std::pair< long long, std::string >{(*it_2).first, (*it_2).second});
      ++it_2;
    }
  }
  all_sets[new_dataset] = new_set;
}
