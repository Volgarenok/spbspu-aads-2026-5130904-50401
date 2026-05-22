#include "commands.hpp"

#include <iostream>
#include <vector.hpp>

void chernov::cmdPrint(std::istream & input, std::ostream & output, dicts_t & dicts)
{
  std::string dataset;
  input >> dataset;
  dict_t & dict = dicts.at(dataset);

  if (dict.empty()) {
    output << "<EMPTY>\n";
  } else {
    output << dataset;
    for (auto iter = dict.begin(); iter != dict.end(); ++iter) {
      output << " " << iter->first << " " << iter->second;
    }
    output << "\n";
  }
}

void chernov::cmdComplement(std::istream & input, std::ostream &, dicts_t & dicts)
{
  std::string newdataset, dataset1, dataset2;
  input >> newdataset >> dataset1 >> dataset2;

  if (dicts.contains(newdataset)) {
    throw std::runtime_error(newdataset + " already exists");
  }

  dict_t & dict1 = dicts.at(dataset1);
  dict_t & dict2 = dicts.at(dataset2);
  dict_t new_dict;

  for (auto iter = dict1.cbegin(); iter != dict1.cend(); ++iter) {
    if (!dict2.contains(iter->first)) {
      new_dict.push(iter->first, iter->second);
    }
  }

  dicts.push(newdataset, new_dict);
}
