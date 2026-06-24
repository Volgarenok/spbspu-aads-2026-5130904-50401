#include "robinTable.hpp"
#include "interface.hpp"
#include "commands.hpp"


int main()
{
  using CarTable = donkeev::RobinTable<std::string, donkeev::Car, donkeev::StringHash, donkeev::StringEqual>;
  using AdTable = donkeev::RobinTable<size_t, donkeev::Ad, donkeev::SizeTHash, donkeev::SizeTEqual>;

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
  cmds.insert("showhistory", donkeev::handleShowHistory);
  cmds.insert("quit", donkeev::handleQuit);
  

  while (true)
  {

  }
}