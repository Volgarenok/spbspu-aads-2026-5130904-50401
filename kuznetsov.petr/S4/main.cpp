#include <fstream>
#include <iostream>
#include <limits>

#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Not enought arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Couldnt open file\n";
    return 1;
  }
  namespace kuz = kuznetsov;

  kuz::dicts dcts;

  std::string token;
  size_t key = 0;
  std::string value;
  while (file >> token) {
    kuz::record d;
    char c;
    while (true) {
      while (file.get(c) && c == ' ');
      if (!file || c == '\n') {
        break;
      }
      file.unget();
      file >> key >> value;
      d.push(key, value);
    }
    dcts.push(token, d);
  }
  file.close();
  using cmd_t = void(*)(std::ostream&, std::istream&, kuz::dicts&);
  kuz::BSTree< std::string, cmd_t, std::less< std::string > > commands;
  commands.push("print", kuz::print);
  commands.push("complement", kuz::complement);
  commands.push("intersect", kuz::intersect);
  commands.push("union", kuz::unionDicts);

  while (std::cin >> token) {
    try {
      commands.at(token)(std::cout, std::cin, dcts);
      if (token == "print") {
        std::cout << '\n';
      }
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::streamsize n = std::numeric_limits< std::streamsize >::max();
      std::cin.ignore(n);
    }
  }
}
