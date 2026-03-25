#include <fstream>
#include <iostream>

#include <stack.hpp>
#include <queue.hpp>

int main(int args, char ** argv)
{
  using namespace chernov;

  std::istream * input = &std::cin;
  if (args > 1) {
    static std::ifstream file_input(argv[1]);
    input = &file_input;
  }

  std::string line;
  while (*input) {
    *input >> line;
  }
}
