#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"

namespace sedov
{
  bool isInteger(const std::string & str)
  {
    if (str.empty())
    {
      return false;
    }
    size_t start = 0;
    if (str[0] == '-' || str[0] == '+')
    {
      start = 1;
    }
    if (start == str.size())
    {
      return false;
    }
    for (size_t i = start; i < str.size(); ++i)
    {
      if (str[i] < '0' || str[i] > '9')
      {
        return false;
      }
    }
    return true;
  }

  template< class Key, class Value >
  void initFake()
  {
    if (TreeNode< Key, Value >::fakeLeaf == nullptr)
    {
      TreeNode< Key, Value >::fakeLeaf = new TreeNode< Key, Value >(Key(), Value(), nullptr);
      TreeNode< Key, Value >::fakeLeaf->left_ = TreeNode< Key, Value >::fakeLeaf;
      TreeNode< Key, Value >::fakeLeaf->right_ = TreeNode< Key, Value >::fakeLeaf;
      TreeNode< Key, Value >::fakeLeaf->parent_ = TreeNode< Key, Value >::fakeLeaf;
    }
  }

  void initFakes()
  {
    initFake< int, std::string >();
    initFake< std::string, pairs >();
  }
}

int main(int argc, char ** argv)
{
  if (argc != 2)
  {
    std::cerr << "bad num of args" << '\n';
    return 1;
  }
  sedov::initFakes();
  sedov::trees trees;
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }
  std::string token;
  std::string curp;
  while (file >> token)
  {
    if (sedov::isInteger(token))
    {
      int key = std::stoi(token);
      std::string value;
      if (file >> value)
      {
        try
        {
          trees.at(curp).push(key, value);
        }
        catch (const std::out_of_range &)
        {}
      }
    }
    else
    {
      curp = token;
      try
      {
        trees.at(curp);
      }
      catch (const std::out_of_range &)
      {
        trees.push(curp, sedov::pairs{});
      }
    }
  }
  file.close();
  using cmdT = void (*)(std::istream &, std::ostream &, sedov::trees &);
  sedov::BSTree< std::string, cmdT > commands;
  commands.push("print", sedov::print);
  commands.push("complement", sedov::complement);
  commands.push("intersect", sedov::intersect);
  commands.push("union", sedov::cUnion);
  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      commands.at(cmd)(std::cin, std::cout, trees);
    }
    catch (const std::exception &)
    {
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
