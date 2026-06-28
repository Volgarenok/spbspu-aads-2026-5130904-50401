#include <fstream>
#include <iostream>
#include <limits>
#include <string>

#include "commands.hpp"
#include "input.hpp"
#include "../common/my_tree/my_compare.hpp"
#include "../common/my_tree/my_tree.hpp"


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

  using Compare = zub::Comparator< std::string >;
  using cmd_t = void (*)(std::istream&, std::ostream&, zub::DatasetTable&);
  zub::BSTree< std::string, cmd_t, Compare > cmds;

  cmds["print"] = zub::cmd_print;
  cmds["complement"] = zub::cmd_complement;
  cmds["intersect"] = zub::cmd_intersect;
  cmds["union"] = zub::cmd_union;

  zub::DatasetTable dataset;

  try {

    zub::inputDataset(input, dataset);
  }

  catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (!cmds.contains(cmd)) {
        throw std::out_of_range("unknown command");
      }
      cmds.at(cmd)(std::cin, std::cout, dataset);
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
