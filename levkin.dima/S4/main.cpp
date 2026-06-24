#include "commands.hpp"
#include "bstree.hpp"
#include "hashtable.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/functional/hash.hpp>

int main(int argc, char* argv[])
{
  if (argc < 2) {
    std::cerr << "Error: filename parameter is required.\n";
    return 1;
  }

  levkin::DatasetStore datasets;
  if (!levkin::loadDictionaries(argv[1], datasets)) {
    std::cerr << "Error: could not open file " << argv[1] << "\n";
    return 1;
  }


  using BoostStringHash = boost::hash< std::string >;
  using StandardStringEqual = std::equal_to< std::string >;

  levkin::HashTable< std::string, levkin::cmd_t, BoostStringHash, StandardStringEqual > cmds(4, 2);

  cmds.add("print", levkin::cmdPrint);
  cmds.add("complement", levkin::cmdComplement);
  cmds.add("intersect", levkin::cmdIntersect);
  cmds.add("union", levkin::cmdUnion);

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    std::stringstream ss(line);
    std::string cmd_name;
    ss >> cmd_name;

    if (cmds.has(cmd_name)) {
      try {
        auto func = cmds.get(cmd_name);
        func(ss, std::cout, datasets);
        if (ss.fail()) {
          std::cout << "<error>\n";
          std::cin.clear();
        }
      } catch (...) {
        std::cout << "<error>\n";
      }
    } else {
      std::cout << "<error>\n";
    }
  }

  return 0;
}
