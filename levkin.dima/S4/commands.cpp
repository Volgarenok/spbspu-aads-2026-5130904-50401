#include "commands.hpp"
#include <fstream>
#include <sstream>
bool levkin::loadDictionaries(const std::string& filename,
                              DatasetStore& datasets)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    return false;
  }
  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string dictName;
    ss >> dictName;
    SubTree newDict;
    int key;
    std::string value;
    while (ss >> key >> value) {
      newDict.push(key, value);
    }
    datasets.push(dictName, std::move(newDict));
  }
  return true;
}
void levkin::cmdPrint(std::istream& in,
                      std::ostream& out,
                      DatasetStore& datasets)
{
  std::string dict_name;
  if (!(in >> dict_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!datasets.has(dict_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const SubTree& dict = datasets.get(dict_name);
  if (dict.cbegin() == dict.cend()) {
    out << "<EMPTY>\n";
    return;
  }
  out << dict_name;
  for (auto it = dict.cbegin(); it != dict.cend(); ++it) {
    out << " " << it->key << " " << it->value;
  }
  out << "\n";
}
void levkin::cmdComplement(std::istream& in,
                           std::ostream& out,
                           DatasetStore& datasets)
{
  std::string resName, leftName, rightName;
  if (!(in >> resName >> leftName >> rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!datasets.has(leftName) || !datasets.has(rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const SubTree& left = datasets.get(leftName);
  const SubTree& right = datasets.get(rightName);
  SubTree result;
  for (auto it = left.cbegin(); it != left.cend(); ++it) {
    if (!right.has(it->key)) {
      result.push(it->key, it->value);
    }
  }
  if (datasets.has(resName)) {
    datasets.drop(resName);
  }
  datasets.push(resName, std::move(result));
}
void levkin::cmdIntersect(std::istream& in,
                          std::ostream& out,
                          DatasetStore& datasets)
{
  std::string resName, leftName, rightName;
  if (!(in >> resName >> leftName >> rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!datasets.has(leftName) || !datasets.has(rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const SubTree& left = datasets.get(leftName);
  const SubTree& right = datasets.get(rightName);
  SubTree result;
  for (auto it = left.cbegin(); it != left.cend(); ++it) {
    if (right.has(it->key)) {
      result.push(it->key, it->value);
    }
  }
  if (datasets.has(resName)) {
    datasets.drop(resName);
  }
  datasets.push(resName, std::move(result));
}
void levkin::cmdUnion(std::istream& in,
                      std::ostream& out,
                      DatasetStore& datasets)
{
  std::string resName, leftName, rightName;
  if (!(in >> resName >> leftName >> rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  if (!datasets.has(leftName) || !datasets.has(rightName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }
  const SubTree& left = datasets.get(leftName);
  const SubTree& right = datasets.get(rightName);
  SubTree result;
  for (auto it = left.cbegin(); it != left.cend(); ++it) {
    result.push(it->key, it->value);
  }
  for (auto it = right.cbegin(); it != right.cend(); ++it) {
    if (!result.has(it->key)) {
      result.push(it->key, it->value);
    }
  }
  if (datasets.has(resName)) {
    datasets.drop(resName);
  }
  datasets.push(resName, std::move(result));
}
