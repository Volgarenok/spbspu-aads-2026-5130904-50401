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
  std::string dictName;
  if (!(in >> dictName)) {
    throw std::invalid_argument("Invalid arguments");
  }

  if (!datasets.has(dictName)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const SubTree& dict = datasets.get(dictName);
  if (dict.cbegin() == dict.cend()) {
    out << "<EMPTY>\n";
    return;
  }

  out << dictName;
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
    throw std::invalid_argument("Invalid arguments");
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


void levkin::cmdIntersect(std::istream& in, std::ostream& out, DatasetStore& datasets)
{
  std::string res_name, left_name, right_name;
  if (!(in >> res_name >> left_name >> right_name)) {
    throw std::invalid_argument("Invalid arguments");
  }

  if (!datasets.has(left_name) || !datasets.has(right_name)) {
    out << "<error>\n";
    return;
  }

  const SubTree& left = datasets.get(left_name);
  const SubTree& right = datasets.get(right_name);
  SubTree result;

  for (auto it = left.cbegin(); it != left.cend(); ++it) {
    if (right.has(it->key)) {
      result.push(it->key, it->value);
    }
  }

  if (datasets.has(res_name)) {
    datasets.drop(res_name);
  }
  datasets.push(res_name, std::move(result));
}