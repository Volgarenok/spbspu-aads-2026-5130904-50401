#include "bstree.hpp"
#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

namespace madieva {

  bool read(std::istream & file, Dictionarycollection & storage)
  {
    std::string line;
    while (std::getline(file, line)) {
      if (line.empty()) {
        continue;
      }
      std::istringstream iss(line);
      std::string dictionary_name;
      if (!(iss >> dictionary_name)) {
        return false;
      }
      if (storage.collection.has(dictionary_name)) {
        return false;
      }
      Dictionary new_dictionary;
      size_t key;
      std::string value;
      while (iss >> key >> value) {
        new_dictionary.dictionary.push(key, value);
      }
      storage.collection.push(dictionary_name, new_dictionary);
    }
    return true;
  }
}

int main(int argc, char * argv[])
{
  if (argc != 2) {
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!(file.is_open())) {
    return 1;
  }

  madieva::Commands commands;
  commands.comm.push("print", madieva::cmd_print);
  commands.comm.push("complement", madieva::cmd_complement);
  commands.comm.push("intersect", madieva::cmd_intersect);
  commands.comm.push("union", madieva::cmd_union);

  madieva::Dictionarycollection storage;
  if (!madieva::read(file, storage)) {
    return 1;
  }

  std::string line;
  while (std::getline(std::cin, line)) {
    std::istringstream iss(line);
    std::string comm;
    if (!(iss >> comm)) {
      std::cout << "<INVALID COMMAND>\n";
      continue;
    }
    if (commands.comm.has(comm)) {
      commands.comm.get(comm)(iss, std::cout, storage);
    }
    else {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
