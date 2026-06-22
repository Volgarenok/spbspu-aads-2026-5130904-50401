#include <iostream>
#include <stdexcept>

#include "trees-commands.hpp"

void donkeev::printDicts(std::istream& input, std::ostream& output, donkeev::Datasets& dicts)
{
    std::string datasetName;
    if (!(input >> datasetName))
    {
      throw std::runtime_error("Bad input");
    }

    Datasets::iterator it = dicts.find(datasetName);
    if (it == dicts.end())
    {
      throw std::runtime_error("No such dictionary");
    }

    const Dataset& tree = it->second;

    if (tree.empty())
    {
      throw std::runtime_error("Empty dictionary");
    }

    output << datasetName;
    for (Dataset::constIterator begin = tree.begin(); begin != tree.end(); ++begin)
    {
      output << " " << begin->first << " " << begin->second;
    }
    
    output << "\n";
}
