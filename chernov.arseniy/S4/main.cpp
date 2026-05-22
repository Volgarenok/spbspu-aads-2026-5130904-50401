#include <fstream>
#include <iostream>

#include <bstree.hpp>

int main(int argc, char ** argv)
{
  if (argc < 2) {
    std::cerr << "One argument is required\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file) {
    std::cerr << "Could not open file\n";
    return 1;
  }

  std::istream & input = std::cin;
  std::ostream & output = std::cout;

  using cmd_t = void(*)(std::istream & input, std::ostream & output,
    chernov::BSTree< std::string, chernov::BSTree< int, std::string, std::less< int > >, std::less< std::string > > &);
  chernov::BSTree< std::string, cmd_t, std::less< std::string > > cmds;
}
