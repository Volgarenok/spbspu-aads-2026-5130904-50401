#include "commands.hpp"

int main()
{
  vasyakin::SystemState state;
  using cmd_t = void(*)(std::istream&, std::ostream&, vasyakin::SystemState&);
  vasyakin::AVLTree< std::string, cmd_t > cmds;

  cmds.insert("create-warehouse", vasyakin::cmdCreateWarehouse);
  cmds.insert("add-item", vasyakin::cmdAddItem);
  cmds.insert("remove-item", vasyakin::cmdRemoveItem);
  cmds.insert("ship", vasyakin::cmdShip);
  cmds.insert("show-warehouse", vasyakin::cmdShowWarehouse);
  cmds.insert("show-transfers", vasyakin::cmdShowTransfers);
  cmds.insert("show-item", vasyakin::cmdShowItem);
  cmds.insert("show-log", vasyakin::cmdShowLog);
  cmds.insert("stats", vasyakin::cmdStats);
  cmds.insert("calculate-center-capacity", vasyakin::cmdCalculateCenterCapacity);
  cmds.insert("show-state-at", vasyakin::cmdShowStateAt);
  cmds.insert("help", vasyakin::cmdHelp);
  cmds.insert("save", vasyakin::cmdSave);
  cmds.insert("load", vasyakin::cmdLoad);

  std::cout << R"(
    ╔══════════════════════════════════════════════════════════════════════════════╗
    ║                                                                              ║
    ║     ██╗      ██████╗  ██████╗ ██╗███████╗████████╗██╗ ██████╗                ║
    ║     ██║     ██╔═══██╗██╔════╝ ██║██╔════╝╚══██╔══╝██║██╔════╝                ║
    ║     ██║     ██║   ██║██║  ███╗██║███████╗   ██║   ██║██║                     ║
    ║     ██║     ██║   ██║██║   ██║██║╚════██║   ██║   ██║██║   ██║               ║
    ║     ███████╗╚██████╔╝╚██████╔╝██║███████║   ██║   ██║╚██████╔╝               ║
    ║     ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝╚══════╝   ╚═╝   ╚═╝ ╚═════╝                ║
    ║                                                                              ║
    ║     ██████╗  ██████╗  ██████╗ ██╗███████╗ ██████╗████████╗                   ║
    ║     ██╔══██╗██╔══██╗██╔═══██╗██║██╔════╝██╔════╝╚══██╔══╝                    ║
    ║     ██████╔╝██████╔╝██║   ██║██║█████╗  ██║        ██║                       ║
    ║     ██╔═══╝ ██╔══██╗██║   ██║██║██╔══╝  ██║        ██║                       ║
    ║     ██║     ██║  ██║╚██████╔╝██║███████╗╚██████╗   ██║                       ║
    ║     ╚═╝     ╚═╝  ╚═╝ ╚═════╝ ╚═╝╚══════╝ ╚═════╝   ╚═╝                       ║
    ║                                                                              ║
    ╚══════════════════════════════════════════════════════════════════════════════╝
    )" << '\n';

  std::cout << "    ╔════════════════════════════════════════════════════════════════╗\n";
  std::cout << "    ║                                                                ║\n";
  std::cout << "    ║         WELCOME TO LOGISTIC PROJECT!                           ║\n";
  std::cout << "    ║                                                                ║\n";
  std::cout << "    ║         Type 'help' to see available commands!                 ║\n";
  std::cout << "    ║                                                                ║\n";
  std::cout << "    ╚════════════════════════════════════════════════════════════════╝\n";
  
  std::cout << "\n\n";

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      cmds.at(cmd)(std::cin, std::cout, state);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "Ты старался, но увы :(" << '\n';
      std::cout << "<INVALID COMMAND>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (const std::exception& e)
    {
      std::cerr << "Error: " << e.what() << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    catch (...)
    {
      std::cerr << "Internal error: unknown exception" << '\n';
      return 2;
    }
  }

  return 0;
}
