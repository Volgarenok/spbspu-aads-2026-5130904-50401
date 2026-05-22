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
