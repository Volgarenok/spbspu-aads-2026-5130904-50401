#include "robinTable.hpp"
#include "interface.hpp"
#include "commands.hpp"


int main()
{

  std::ofstream("cars.txt", std::ios::trunc).close();
  std::ofstream("history.txt", std::ios::trunc).close();

  using CarTable = donkeev::RobinTable<std::string, donkeev::Car, donkeev::StringHash, donkeev::StringEqual>;
  using AdTable = donkeev::RobinTable<size_t, donkeev::Ad, donkeev::SizeTHash, donkeev::SizeTEqual>;
  using CommandHandler = void(*)(CarTable&, AdTable&);

  CarTable cars(16);
  AdTable ads(16);

  donkeev::showBanner();
  donkeev::showNavigation();

  donkeev::RobinTable< std::string, CommandHandler,
    donkeev::StringHash, donkeev::StringEqual > cmds(16);

  cmds.insert("scroll", donkeev::handleScroll);
  cmds.insert("sort", donkeev::handleSort);
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

    if (command == "quit")
    {
      break;
    }

    CommandHandler* handler = cmds.find(command);
    if (handler)
    {
      (*handler)(cars, ads);
      donkeev::showNavigation();
    }
    else
    {
      std::cout << "\033[31m  Неизвестная команда.\033[0m\n\n";
    }
  }

  return 0;
}
