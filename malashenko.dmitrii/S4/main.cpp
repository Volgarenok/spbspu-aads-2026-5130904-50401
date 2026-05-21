#include <iostream>
#include <fstream>
#include <limits>
#include "BStree.hpp"
#include "collection.hpp"


bool isInteger(const std::string& s)
{
  if (s.empty())
  {
    return false;
  }
  size_t start = 0;
  if (s[0] == '-' || s[0] == '+')
  {
    start = 1;
  }
  if (start == s.size())
  {
    return false;
  }
  for (size_t i = start; i < s.size(); ++i)
  {
    if (!std::isdigit(s[i]))
    {
      return false;
    }
  }
  return true;
}

int main(int argc, char* argv[])
{
  using namespace malashenko;
  if (argc != 2)
  {
    std::cerr << "too much args\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Can't open the file\n";
    return 1;
  }

  Collection col;
  std::string token;
  std::string currDatasetName;

  while (file >> token)
  {
    if (isInteger(token))
    {
      if (currDatasetName.empty())
      {
        continue;
      }

      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        Dataset& ds = col.get(currDatasetName);
        ds.push(key, value);
      }
    }
    else
    {
      currDatasetName = token;
      try
      {
        col.get(currDatasetName);
      }
      catch (const std::out_of_range&)
      {
        col.push(currDatasetName, Dataset());

      }
    }
  }



  file.close();


  using cmd_t = void(*)(std::istream&, std::ostream&, malashenko::Collection&);
  malashenko::BSTree< std::string, cmd_t, std::less< std::string > > cmds;

  cmds.push("print", malashenko::print);
  cmds.push("complement", malashenko::complement);
  cmds.push("intersect", malashenko::intersect);
  cmds.push("union", malashenko::unionCollections);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cin, std::cout, col);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
