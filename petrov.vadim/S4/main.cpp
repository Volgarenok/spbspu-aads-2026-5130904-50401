#include "commands.hpp"
#include <fstream>
#include <limits>
#include <cstddef>

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

int main(int argc, char const *argv[])
{
  using namespace petrov;
  if (argc != 2)
  {
    std::cerr << "argument invalid\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "invalid file\n";
    return 1;
  }

  collection coll;
  std::string token;
  std::string data_name;

  while (file >> token)
  {
    if (isInteger(token))
    {
      if (data_name.empty())
      {
        continue;
      }
      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        dataset& dd = coll.get(data_name);
        dd.push(key, value);
      }
    }
    else
    {
      data_name = token;
      try
      {
        coll.get(data_name);
      }
      catch(const std::exception& e)
      {
        coll.push(data_name, dataset());
      }
    }
  }

  file.close();

  using cmd_t = void(*)(std::istream&, std::ostream&, collection&);
  petrov::BSTree< std::string, cmd_t, std::less< std::string > > cmds;
  cmds.push("print", petrov::print);
  cmds.push("complement", petrov::complement);
  cmds.push("intersect", petrov::intersect);
  cmds.push("union", petrov::unionn);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.get(cmd)(std::cin, std::cout, coll);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
}

