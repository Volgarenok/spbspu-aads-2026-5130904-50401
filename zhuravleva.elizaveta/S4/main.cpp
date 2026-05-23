#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file)
  {
    std::cerr << "<INVALID COMMAND>\n";
    return 1;
  }
  zhuravleva::DictionaryStorage dictionaries;
  std::string name;
  int key = 0;
  std::string value;
  while (file >> name)
  {
    zhuravleva::Dictionary dictionary;
    char symbol = '\0';
    while (true)
    {
      while (file.get(symbol) && symbol == ' ')
      {}
      if (!file || symbol == '\n')
      {
        break;
      }
      file.unget();
      file >> key >> value;
      dictionary.push(key, value);
    }
    dictionaries.push(name, dictionary);
  }
  while (std::cin)
  {
    zhuravleva::processCommand(
      std::cin,
      std::cout,
      dictionaries
    );
  }
  return 0;
}
