#include "commands.hpp"

void zharov::print(std::ostream& out, std::istream& in, const dictionaries_t& dicts)
{
  std::string dict_name;
  in >> dict_name;
  if (!dicts.has(dict_name))
  {
    throw std::logic_error("Dict not found");
  }

  if (dicts.at(dict_name).size() == 0)
  {
    out << "<EMPTY>";
    return;
  }
  out << dict_name;
  for (auto i = dicts.at(dict_name).cbegin(); i != dicts.at(dict_name).cend(); ++i)
  {
    out << ' ' << i->first << ' ' << i->second;
  }
}

void zharov::complement(std::ostream&, std::istream& in, dictionaries_t& dicts)
{
  std::string dict_name_new, dict_name_old_1, dict_name_old_2;
  in >> dict_name_new >> dict_name_old_1 >> dict_name_old_2;
  if (!dicts.has(dict_name_old_1) || !dicts.has(dict_name_old_2))
  {
    throw std::logic_error("Dict not found");
  }

  dictionary_t new_dict;
  for (auto i = dicts.at(dict_name_old_1).cbegin(); i != dicts.at(dict_name_old_1).cend(); ++i)
  {
    if (!dicts.at(dict_name_old_2).has(i->first))
    {
      new_dict.push(i->first, i->second);
    }
  }
  for (auto i = dicts.at(dict_name_old_2).cbegin(); i != dicts.at(dict_name_old_2).cend(); ++i)
  {
    if (!dicts.at(dict_name_old_1).has(i->first))
    {
      new_dict.push(i->first, i->second);
    }
  }
  dicts.push(dict_name_new, new_dict);
}

void zharov::intersect(std::ostream&, std::istream& in, dictionaries_t& dicts)
{
  std::string dict_name_new, dict_name_old_1, dict_name_old_2;
  in >> dict_name_new >> dict_name_old_1 >> dict_name_old_2;
  if (!dicts.has(dict_name_old_1) || !dicts.has(dict_name_old_2))
  {
    throw std::logic_error("Dict not found");
  }

  dictionary_t new_dict;
  for (auto i = dicts.at(dict_name_old_1).cbegin(); i != dicts.at(dict_name_old_1).cend(); ++i)
  {
    if (dicts.at(dict_name_old_2).has(i->first))
    {
      new_dict.push(i->first, i->second);
    }
  }
  dicts.push(dict_name_new, new_dict);
}

void zharov::makeUnion(std::ostream&, std::istream& in, dictionaries_t& dicts)
{
  std::string dict_name_new, dict_name_old_1, dict_name_old_2;
  in >> dict_name_new >> dict_name_old_1 >> dict_name_old_2;
  if (!dicts.has(dict_name_old_1) || !dicts.has(dict_name_old_2))
  {
    throw std::logic_error("Dict not found");
  }

  dictionary_t new_dict;
  for (auto i = dicts.at(dict_name_old_2).cbegin(); i != dicts.at(dict_name_old_2).cend(); ++i)
  {
    new_dict.push(i->first, i->second);
  }
  for (auto i = dicts.at(dict_name_old_1).cbegin(); i != dicts.at(dict_name_old_1).cend(); ++i)
  {
    new_dict.push(i->first, i->second);
  }
  dicts.push(dict_name_new, new_dict);
}
