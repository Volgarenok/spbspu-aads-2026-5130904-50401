#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>
#include "commands.hpp"

int main()
{
  using namespace petrov;
  FuncManage table;

  using cmd_t = void (FuncManage::*)(std::ostream&, std::istream&, const std::string&);
  KukuHashTable< cmd_t, std::string, KKHash< std::string >, Equal< std::string > > commands;

  commands.add("mk-user", &FuncManage::mk_user);
  commands.add("rm-user", &FuncManage::rm_user);
  commands.add("update-user", &FuncManage::update_user);
  commands.add("show-users", &FuncManage::show_users);
  commands.add("save", &FuncManage::save);
  commands.add("load", &FuncManage::load);
  commands.add("train", &FuncManage::train);
  commands.add("mk-ex", &FuncManage::mk_ex);
  commands.add("rm-ex", &FuncManage::rm_ex);
  commands.add("show-ex", &FuncManage::show_ex);
  commands.add("add-ex-pool", &FuncManage::add_ex_pool);
  commands.add("rm-pool", &FuncManage::rm_pool);
  commands.add("show-pools", &FuncManage::show_pools);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (!commands.has(cmd))
      {
        throw std::runtime_error("Invalid command");
      }

      std::string arg = "";
      if (cmd != "show-users" && cmd != "show-ex" && cmd != "show-pools")
      {
        if (!(std::cin >> arg))
        {
          throw std::runtime_error("Argument missing");
        }
      }

      auto func = commands.get(cmd);
      (table.*func)(std::cout, std::cin, arg);
    }
    catch (const std::exception& e)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
