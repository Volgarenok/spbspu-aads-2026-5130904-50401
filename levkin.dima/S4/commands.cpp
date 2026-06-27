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
