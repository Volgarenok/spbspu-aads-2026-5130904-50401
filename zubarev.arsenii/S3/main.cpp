#include "../common/top-it-vector.hpp"
#include "commands.hpp"
#include "graph.hpp"
#include "input.hpp"
#include "my_equal.hpp"
#include "my_hashtable.hpp"
#include "my_siphash.hpp"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>


int main(int argc, char* argv[])

{

  namespace zub = zubarev;

  if (argc != 2) {
    std::cerr << "Wrong arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  using Hash = zub::SipHash;
  using Equal = zub::Equaler< std::string >;
  using cmd_t = void (*)(std::istream&, std::ostream&, zub::GraphTable&);
  zub::HashTable< std::string, cmd_t, Hash, Equal > cmds;

  cmds["graphs"] = zub::cmd_graphs;
  cmds["vertexes"] = zub::cmd_vertexes;
  cmds["outbound"] = zub::cmd_outbound;
  cmds["inbound"] = zub::cmd_inbound;
  cmds["bind"] = zub::cmd_bind;
  cmds["cut"] = zub::cmd_cut;
  cmds["create"] = zub::cmd_create;
  cmds["merge"] = zub::cmd_merge;
  cmds["extract"] = zub::cmd_extract;

  zub::GraphTable graph;

  try {

    zub::inputGraphs(input, std::cout, graph);
  }

  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (!cmds.has(cmd)) {
        throw std::out_of_range("unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, graph);
    } catch (const std::out_of_range&) {
      std::cout << "<INVALID COMMAND>\n";
      auto toignore = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(toignore, '\n');
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << '\n';
    }
  }
  std::cin.clear();
  std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');

  if (!std::cin.eof() && std::cin.fail()) {
    std::cerr << "Bad input";
    return 1;
  }
}
