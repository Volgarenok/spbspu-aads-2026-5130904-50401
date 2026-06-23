#include <fstream>
#include <iostream>
#include <boost/describe/class.hpp>
#include <boost/hash2/hash_append.hpp>
#include <boost/hash2/siphash.hpp>
#include "./HashTable.hpp"
#include "./Hasher.hpp"
#include "./commands.hpp"
#include "./graph.hpp"

int main(int argc, char** argv)
{
  namespace kuz = kuznetsov;
  using command = void (*)(std::ostream&, std::istream&, kuz::table&);
  using constCommand = void (*)(std::ostream&, std::istream&, const kuz::table&);
  using hashString = kuz::SipHasher< std::string >;

  if (argc < 2) {
    std::cerr << "Not enough arguments\n";
    return 1;
  }
  std::ifstream input(argv[1]);
  if (!input) {
    std::cerr << "Couldn't open file\n";
    return 1;
  }

  std::string name;
  size_t count = 0;
  kuz::table grphs;
  kuz::HashTable< std::string, command, hashString, kuz::KeyComparator > cmds;
  kuz::HashTable< std::string, constCommand, hashString, kuz::KeyComparator > constCmds;

  constCmds.add("graphs", kuz::graphs);
  constCmds.add("vertexes", kuz::vertexes);
  constCmds.add("outbound", kuz::outbound);
  constCmds.add("inbound", kuz::inbound);
  cmds.add("bind", kuz::bind);
  cmds.add("cut", kuz::cut);
  cmds.add("create", kuz::create);
  cmds.add("merge", kuz::merge);
  cmds.add("extract", kuz::extract);
  while (input >> name >> count) {
    kuz::Graph t(count);
    for (size_t i = 0; i < count; ++i) {
      kuz::key e;
      size_t weight;
      input >> e.first >> e.second >> weight;
      t.addEdge(e.first, e.second, weight);
    }
    grphs.add(name, t);
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmds.contains(cmd)) {
        cmds.at(cmd)(std::cout, std::cin, grphs);
      } else {
        constCmds.at(cmd)(std::cout, std::cin, grphs);
        std::cout << '\n';
      }
    } catch (const std::logic_error& e) {
      std::cout << "<INVALID COMMAND>\n";
      auto skip = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(skip, '\n');
    }
  }
}
