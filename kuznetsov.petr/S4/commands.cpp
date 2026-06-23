#include "commands.hpp"
#include <iostream>

void kuznetsov::print(std::ostream& out, std::istream& in, dicts& ds)
{
  namespace kuz = kuznetsov;
  std::string title;
  in >> title;
  if (in.fail()) {
    throw std::logic_error("Smth went wrong");
  }
  const record& dict = ds.at(title);
  if (dict.empty()) {
    out << "<EMPTY>";
    return;
  }
  out << title << ' ';
  auto it = dict.cbegin();
  out << it->first << ' ' << it->second;
  ++it;
  for (; it != dict.cend(); ++it) {
    out << ' ' << it->first << ' ' << it->second;
  }
}

void kuznetsov::complement(std::ostream&, std::istream& in, dicts& ds)
{
  namespace kuz = kuznetsov;
  std::string dict3, dict2, dict1;
  in >> dict3 >> dict2 >> dict1;
  if (in.fail()) {
    throw std::logic_error("Smth went wrong");
  }
  if (!ds.contain(dict1) || !ds.contain(dict2)) {
    throw std::logic_error("Not found dict");
  }

  record nd;
  const record& dataset1 = ds.at(dict2);
  const record& dataset2 = ds.at(dict1);
  for (auto it = dataset1.cbegin(); it != dataset1.cend(); ++it) {
    if (!dataset2.contain(it->first)) {
      nd.push(it->first, it->second);
    }
  }
  ds.push(dict3, nd);
}

void kuznetsov::intersect(std::ostream&, std::istream& in, dicts& ds)
{
  namespace kuz = kuznetsov;
  std::string dict3, dict2, dict1;
  in >> dict3 >> dict2 >> dict1;
  if (in.fail()) {
    throw std::logic_error("Smth went wrong");
  }
  if (!ds.contain(dict1) || !ds.contain(dict2)) {
    throw std::logic_error("Not found dict");
  }
  record nd;
  const record& dataset1 = ds.at(dict2);
  const record& dataset2 = ds.at(dict1);
  for (auto it = dataset1.cbegin(); it != dataset1.cend(); ++it) {
    if (dataset2.contain(it->first)) {
      nd.push(it->first, it->second);
    }
  }
  ds.push(dict3, nd);
}

void kuznetsov::unionDicts(std::ostream&, std::istream& in, dicts& ds)
{
  namespace kuz = kuznetsov;
  std::string dict3, dict2, dict1;
  in >> dict3 >> dict2 >> dict1;
  if (in.fail()) {
    throw std::logic_error("Smth went wrong");
  }
  if (!ds.contain(dict1) || !ds.contain(dict2)) {
    throw std::logic_error("Not found dict");
  }
  record nd;
  const record& dataset1 = ds.at(dict2);
  const record& dataset2 = ds.at(dict1);
  for (auto it = dataset1.cbegin(); it != dataset1.cend(); ++it) {
    if (!nd.contain(it->first)) {
      nd.push(it->first, it->second);
    }
  }

  for (auto it = dataset2.cbegin(); it != dataset2.cend(); ++it) {
    if (!nd.contain(it->first)) {
      nd.push(it->first, it->second);
    }
  }
  ds.push(dict3, nd);
}
