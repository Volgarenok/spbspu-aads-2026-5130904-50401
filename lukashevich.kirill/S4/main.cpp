#include <iostream>
#include <exception>
#include <fstream>

#include "commands.hpp"
#include "dataset.hpp"
#include "input.hpp"

int main(int argc, char * argv[])
{
  if (argc != 2)
  {
    std::cerr << "invalid command rguments\n";
    return 1;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile)
  {
    std::cerr << "cant open file\n";
    return 1;
  }

  lukashevich::DatasetTable dataSets;
  try
  {
    lukashevich::inputDataset(inputFile, dataSets);
  }
  catch (const std::exception & error)
  {
    std::cerr << error.what() << '\n';
    return 1;
  }

  try
  {
    lukashevich::executeCommands(std::cin, std::cout, dataSets);
  }
  catch (const std::exception & error)
  {
    std::cerr << error.what() << '\n';
    return 2;
  }
  return 0;
}
