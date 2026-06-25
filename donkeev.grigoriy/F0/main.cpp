#include "robinTable.hpp"
#include "interface.hpp"
#include "commands.hpp"


int main()
{
  using CarTable = donkeev::RobinTable<std::string, donkeev::Car, donkeev::StringHash, donkeev::StringEqual>;
  using AdTable = donkeev::RobinTable<size_t, donkeev::Ad, donkeev::SizeTHash, donkeev::SizeTEqual>;
  using CommandHandler = void(*)(CarTable&, AdTable&, const std::string&);

  CarTable cars(16);
  AdTable ads(16);

  donkeev::showBanner();
  donkeev::showNavigation();

  donkeev::RobinTable< std::string, void(*)(CarTable&, AdTable&, const std::string&),
    donkeev::StringHash, donkeev::StringEqual > cmds(16);

  cmds.insert("scroll", donkeev::handleScroll);
  cmds.insert("make", donkeev::handleMake);
  cmds.insert("buy", donkeev::handleBuy);
  cmds.insert("delete", donkeev::handleDelete);
  cmds.insert("show-history", donkeev::handleShowHistory);
  
  std::string input;
  while (true)
  {
    std::getline(std::cin, input);
    if (input.empty())
    {
      continue;
    }

    size_t pos = 0;
    std::string command = donkeev::nextWord(input, pos);
    std::string parametrs = input.substr(pos);

    CommandHandler* handler = cmds.find(command);
    if (handler)
    { 
      (*handler)(cars, ads, parametrs);
    }
    else
    {
      std::cout << "\033[31m  Неизвестная команда.\033[0m\n\n";
    }

    if (command == "quit")
    {
      break;
    }
  }

  return 0;
}