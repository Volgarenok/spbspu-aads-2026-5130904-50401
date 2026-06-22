#include <iostream>
#include <limits>
#include "analytics.hpp"
#include "cafe.hpp"
#include "history.hpp"
#include "menu.hpp"
#include "order.hpp"
#include "queue.hpp"

namespace
{
  void cmdHelp(std::ostream& out, std::istream&, const zharov::CafeSystem&)
  {
    out << "=== CAFE SYSTEM COMMANDS ===\n";
    out << "\n-- MENU --\n";
    out << "menu-add <menu> <name> <price> <time> [desc]  add item to menu\n";
    out << "menu-remove <menu> <name>                     remove item from menu\n";
    out << "menu-price <menu> <name> <price>              change item price\n";
    out << "menu-time <menu> <name> <time>                change item prep time\n";
    out << "menu-desc <menu> <name> <desc>                update item description\n";
    out << "menu-available <menu> <name> yes|no           set item availability\n";
    out << "menu-import <file> <menu>                     load menu from file\n";
    out << "menu-copy <src> <dst>                         copy menu under new name\n";
    out << "menu-show <menu> [name]                       show menu or single item\n";
    out << "menu-export <file> <menu>                     save menu to file\n";
    out << "menu-list                                     list all menus\n";
    out << "\n-- ORDERS --\n";
    out << "order-create <queue> <id>                     create empty order in queue\n";
    out << "order-add <queue> <id> <menu> <name> [count]  add item to order\n";
    out << "order-remove <queue> <id> <name>              remove item from order\n";
    out << "order-cancel <queue> <id>                     cancel order (saved to history)\n";
    out << "order-show <queue> [id]                       show order(s) with totals\n";
    out << "\n-- QUEUES --\n";
    out << "queue-create <name>                           create new queue\n";
    out << "queue-remove <name>                           cancel all orders and remove queue\n";
    out << "queue-complete <name>                         mark all orders completed\n";
    out << "queue-reject <name>                           mark all orders rejected\n";
    out << "queue-show <name>                             list orders in queue\n";
    out << "queue-list                                    list all queues\n";
    out << "\n-- ANALYTICS --\n";
    out << "optimize <src> <accept> <reject> <time>       select max-profit orders by knapsack\n";
    out << "history [n]                                   show last n history entries\n";
    out << "analyze-item <menu> <name>                    show item stats from history\n";
    out << "help                                          show this message\n";
  }
}

int main(int argc, char** argv)
{
  zharov::CafeSystem cafe;
  cafe.history_file = (argc > 1) ? argv[1] : "history.txt";

  using Cmd = void (*)(std::ostream&, std::istream&, zharov::CafeSystem&);
  using ConstCmd = void (*)(std::ostream&, std::istream&, const zharov::CafeSystem&);
  zharov::RHHashTable< std::string, Cmd, std::hash< std::string >, std::equal_to< std::string > >
    cmds;
  zharov::RHHashTable< std::string, ConstCmd, std::hash< std::string >,
    std::equal_to< std::string > >
    constCmds;

  cmds.add("menu-add", zharov::menuAdd);
  cmds.add("menu-remove", zharov::menuRemove);
  cmds.add("menu-price", zharov::menuPrice);
  cmds.add("menu-time", zharov::menuTime);
  cmds.add("menu-desc", zharov::menuDesc);
  cmds.add("menu-available", zharov::menuAvailable);
  cmds.add("menu-import", zharov::menuImport);
  cmds.add("menu-copy", zharov::menuCopy);
  constCmds.add("menu-show", zharov::menuShow);
  constCmds.add("menu-export", zharov::menuExport);
  constCmds.add("menu-list", zharov::menuList);
  cmds.add("order-create", zharov::orderCreate);
  cmds.add("order-add", zharov::orderAdd);
  cmds.add("order-remove", zharov::orderRemove);
  cmds.add("order-cancel", zharov::orderCancel);
  constCmds.add("order-show", zharov::orderShow);
  cmds.add("queue-create", zharov::queueCreate);
  cmds.add("queue-remove", zharov::queueRemove);
  cmds.add("queue-complete", zharov::queueComplete);
  cmds.add("queue-reject", zharov::queueReject);
  constCmds.add("queue-show", zharov::queueShow);
  constCmds.add("queue-list", zharov::queueList);
  cmds.add("optimize", zharov::cmdOptimize);
  constCmds.add("history", zharov::cmdHistory);
  constCmds.add("analyze-item", zharov::analyzeItem);
  constCmds.add("help", cmdHelp);

  std::string command;
  while (std::cin >> command)
  {
    if (!cmds.has(command) && !constCmds.has(command))
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      continue;
    }
    try
    {
      if (cmds.has(command))
      {
        cmds.at(command)(std::cout, std::cin, cafe);
      }
      else
      {
        constCmds.at(command)(std::cout, std::cin, cafe);
      }
    }
    catch (const std::exception& e)
    {
      std::cin.clear();
      std::cout << "<INVALID COMMAND" << e.what() << ">\n";
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
}
