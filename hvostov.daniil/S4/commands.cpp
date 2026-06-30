#include "commands.hpp"
#include <stdexcept>

void hvostov::loadDatasets(std::istream& in, DatasetMap& datasets)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#')
      break;

    size_t pos = 0;
    while (pos < line.size() && line[pos] == ' ')
      pos++;

    size_t start = pos;
    while (pos < line.size() && line[pos] != ' ')
      pos++;
    std::string dataset_name = line.substr(start, pos - start);

    if (dataset_name.empty())
      continue;

    Dataset dataset;
    while (pos < line.size()) {
      while (pos < line.size() && line[pos] == ' ')
        pos++;
      if (pos >= line.size())
        break;

      start = pos;
      while (pos < line.size() && line[pos] != ' ')
        pos++;
      std::string key = line.substr(start, pos - start);

      while (pos < line.size() && line[pos] == ' ')
        pos++;
      if (pos >= line.size())
        break;

      start = pos;
      while (pos < line.size() && line[pos] != ' ')
        pos++;
      std::string value = line.substr(start, pos - start);

      dataset.push(key, value);
    }

    datasets.push(dataset_name, std::move(dataset));
  }
}

void hvostov::printDataset(std::istream& in, std::ostream& out, const DatasetMap& datasets)
{
  std::string dataset_name;
  in >> dataset_name;

  if (!in || dataset_name.empty()) {
    throw std::runtime_error("Invalid command arguments\n");
  }

  if (!datasets.has(dataset_name)) {
    throw std::runtime_error("Dataset not found\n");
  }

  const Dataset& dataset = datasets.at(dataset_name);
  if (dataset.empty()) {
    out << "<EMPTY>";
  } else {
    out << dataset_name;
    for (auto node_it = dataset.cbegin(); node_it != dataset.cend(); ++node_it) {
      out << " " << (*node_it).first << " " << (*node_it).second;
    }
  }
}

void hvostov::complementDatasets(std::istream& in, std::ostream&, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    throw std::runtime_error("Invalid command arguments\n");
  }

  if (!datasets.has(ds1_name) || !datasets.has(ds2_name)) {
    throw std::runtime_error("One or both datasets not found\n");
  }

  const Dataset& ds1 = datasets.at(ds1_name);
  const Dataset& ds2 = datasets.at(ds2_name);
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    const std::string& key = (*it).first;
    if (!ds2.has(key)) {
      result.push(key, (*it).second);
    }
  }

  datasets.push(new_dataset, std::move(result));
}

void hvostov::intersectDatasets(std::istream& in, std::ostream&, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    throw std::runtime_error("Invalid command arguments\n");
  }

  if (!datasets.has(ds1_name) || !datasets.has(ds2_name)) {
    throw std::runtime_error("One or both datasets not found\n");
  }

  const Dataset& ds1 = datasets.at(ds1_name);
  const Dataset& ds2 = datasets.at(ds2_name);
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    const std::string& key = (*it).first;
    if (ds2.has(key)) {
      result.push(key, (*it).second);
    }
  }

  datasets.push(new_dataset, std::move(result));
}

void hvostov::unionDatasets(std::istream& in, std::ostream&, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    throw std::runtime_error("Invalid command arguments\n");
  }

  if (!datasets.has(ds1_name) || !datasets.has(ds2_name)) {
    throw std::runtime_error("One or both datasets not found\n");
  }

  const Dataset& ds1 = datasets.at(ds1_name);
  const Dataset& ds2 = datasets.at(ds2_name);
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    result.push((*it).first, (*it).second);
  }

  for (auto it = ds2.cbegin(); it != ds2.cend(); ++it) {
    const std::string& key = (*it).first;
    if (!result.has(key)) {
      result.push(key, (*it).second);
    }
  }

  datasets.push(new_dataset, std::move(result));
}
