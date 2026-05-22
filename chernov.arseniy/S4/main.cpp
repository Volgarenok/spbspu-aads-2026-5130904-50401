#include <fstream>
#include <iostream>
#include <limits>

#include <bstree.hpp>
#include "commands.hpp"

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

  chernov::dicts_t dicts;

  std::string dataset;
  int key;
  std::string value;
  while (file >> dataset) {
    dicts.push(dataset, chernov::dict_t());
    chernov::dict_t & dict = dicts.at(dataset);
    while (file.peek() != '\n' && file.peek() != EOF) {
      file >> key >> value;
      dict.push(key, value);
    }
  }

  if (!file.eof()) {
    std::cerr << "bad file input\n";
    return 1;
  }

  chernov::BSTree< std::string, chernov::cmd_t, std::less< std::string > > cmds;
  cmds.push("print", chernov::cmdPrint);
  cmds.push("complement", chernov::cmdComplement);

  std::string cmd;
  while (input >> cmd) {
    try {
      cmds.at(cmd)(input, output, dicts);
    } catch (...) {
      output << "<INVALID COMMAND>\n";
      input.clear();
      std::streamsize max_streamsize = std::numeric_limits< std::streamsize >::max();
      input.ignore(max_streamsize, '\n');
    }
  }

  if (!input.eof()) {
    std::cerr << "bad input\n";
    return 1;
  }
}
