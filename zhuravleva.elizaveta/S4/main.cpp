#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"
namespace
{
  using Command = std::function< void(std::istream&,
      std::ostream&, zhuravleva::DictionaryStorage&) >;
  using CommandStorage = zhuravleva::BSTree< std::string, Command >;

  void skipLine(std::istream& in)
  {
    in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  }

  void readPrint( std::istream& in, std::ostream& out,
      zhuravleva::DictionaryStorage& dictionaries
  )
  {
    std::string name;
    in >> name;
    if (!in)
    {
      throw std::runtime_error("invalid command");
    }

    zhuravleva::print(out, name, dictionaries);
    out << '\n';
  }

  void readComplement( std::istream& in, std::ostream&,
      zhuravleva::DictionaryStorage& dictionaries
  )
  {
    std::string newName;
    std::string firstName;
    std::string secondName;
    in >> newName >> firstName >> secondName;
    if (!in)
    {
      throw std::runtime_error("invalid command");
    }

    zhuravleva::complement(dictionaries, newName, firstName, secondName);
  }

  void readIntersect( std::istream& in, std::ostream&,
      zhuravleva::DictionaryStorage& dictionaries
  )
  {
    std::string newName;
    std::string firstName;
    std::string secondName;
    in >> newName >> firstName >> secondName;
    if (!in)
    {
      throw std::runtime_error("invalid command");
    }

    zhuravleva::intersect(dictionaries, newName, firstName, secondName);
  }

  void readUnion( std::istream& in, std::ostream&,
      zhuravleva::DictionaryStorage& dictionaries
  )
  {
    std::string newName;
    std::string firstName;
    std::string secondName;
    in >> newName >> firstName >> secondName;
    if (!in)
    {
      throw std::runtime_error("invalid command");
    }

    zhuravleva::unite(dictionaries, newName, firstName, secondName);
  }

  void readDictionaries( std::istream& file,
      zhuravleva::DictionaryStorage& dictionaries
  )
  {
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
  }

  CommandStorage createCommands()
  {
    CommandStorage commands;
    commands.push("print", readPrint);
    commands.push("complement", readComplement);
    commands.push("intersect", readIntersect);
    commands.push("union", readUnion);
    return commands;
  }
}

int main(int argc, char** argv)
{
  if (argc != 2)
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
  readDictionaries(file, dictionaries);
  CommandStorage commands = createCommands();
  std::string command;
  while (std::cin >> command)
  {
    try
    {
      CommandStorage::iterator commandIt = commands.find(command);
      if (commandIt == commands.end())
      {
        throw std::runtime_error("invalid command");
      }

      commandIt->second(std::cin, std::cout, dictionaries);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      skipLine(std::cin);
    }
  }
  return 0;
}
