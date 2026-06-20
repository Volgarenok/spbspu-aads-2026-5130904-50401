#include "commands.hpp"
#include <limits>
#include <sstream>

void hvostov::handleError(std::ostream& out, std::istream& in)
{
  out << "<INVALID COMMAND>\n";
  auto toignore = std::numeric_limits< std::streamsize >::max();
  in.ignore(toignore, '\n');
}

void hvostov::loadDatasets(std::istream& in, DatasetMap& datasets)
{
  std::string line;
  while (std::getline(in, line)) {
    if (line.empty() || line[0] == '#')
      break;

    std::istringstream iss(line);
    std::string dataset_name;
    iss >> dataset_name;

    if (dataset_name.empty())
      continue;

    Dataset dataset;
    std::string key, value;
    while (iss >> key >> value) {
      dataset.push(key, value);
    }

    datasets[dataset_name] = std::move(dataset);
  }
}

void hvostov::printDataset(std::istream& in, std::ostream& out, DatasetMap& datasets)
{
  std::string dataset_name;
  in >> dataset_name;

  if (!in || dataset_name.empty()) {
    handleError(out, in);
    return;
  }

  auto it = datasets.find(dataset_name);
  if (it == datasets.end()) {
    handleError(out, in);
    return;
  }

  const Dataset& dataset = it->second;
  if (dataset.empty()) {
    out << "<EMPTY>\n";
  } else {
    out << dataset_name;
    for (auto node_it = dataset.cbegin(); node_it != dataset.cend(); ++node_it) {
      out << " " << (*node_it).data.first << " " << (*node_it).data.second;
    }
    out << "\n";
  }
}

void hvostov::complementDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    handleError(out, in);
    return;
  }

  auto it1 = datasets.find(ds1_name);
  auto it2 = datasets.find(ds2_name);

  if (it1 == datasets.end() || it2 == datasets.end()) {
    handleError(out, in);
    return;
  }

  const Dataset& ds1 = it1->second;
  const Dataset& ds2 = it2->second;
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    const std::string& key = (*it).data.first;
    if (!ds2.has(key)) {
      result.push(key, (*it).data.second);
    }
  }

  datasets[new_dataset] = std::move(result);
}

void hvostov::intersectDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    handleError(out, in);
    return;
  }

  auto it1 = datasets.find(ds1_name);
  auto it2 = datasets.find(ds2_name);

  if (it1 == datasets.end() || it2 == datasets.end()) {
    handleError(out, in);
    return;
  }

  const Dataset& ds1 = it1->second;
  const Dataset& ds2 = it2->second;
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    const std::string& key = (*it).data.first;
    if (ds2.has(key)) {
      result.push(key, (*it).data.second);
    }
  }

  datasets[new_dataset] = std::move(result);
}

void hvostov::unionDatasets(std::istream& in, std::ostream& out, DatasetMap& datasets)
{
  std::string new_dataset, ds1_name, ds2_name;
  in >> new_dataset >> ds1_name >> ds2_name;

  if (!in || new_dataset.empty() || ds1_name.empty() || ds2_name.empty()) {
    handleError(out, in);
    return;
  }

  auto it1 = datasets.find(ds1_name);
  auto it2 = datasets.find(ds2_name);

  if (it1 == datasets.end() || it2 == datasets.end()) {
    handleError(out, in);
    return;
  }

  const Dataset& ds1 = it1->second;
  const Dataset& ds2 = it2->second;
  Dataset result;

  for (auto it = ds1.cbegin(); it != ds1.cend(); ++it) {
    result.push((*it).data.first, (*it).data.second);
  }

  for (auto it = ds2.cbegin(); it != ds2.cend(); ++it) {
    const std::string& key = (*it).data.first;
    if (!result.has(key)) {
      result.push(key, (*it).data.second);
    }
  }

  datasets[new_dataset] = std::move(result);
}
