#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "robinTable.hpp"
#include "Ad.hpp"
#include "robinTableStructs.hpp"

namespace donkeev
{
  using CarTable = donkeev::RobinTable<std::string, Car, StringHash, StringEqual>;
  using AdTable = donkeev::RobinTable<size_t, Ad, SizeTHash, SizeTEqual>;
  void handleScroll(CarTable& cars, AdTable& ads, const std::string& parametrs);
  void handleMake(CarTable& cars, AdTable& ads, const std::string& parametrs);
  void handleBuy(CarTable& cars, AdTable& ads, const std::string& parametrs);
  void handleDelete(CarTable& cars, AdTable& ads, const std::string& parametrs);
  void handleShowHistory(CarTable& cars, AdTable& ads, const std::string& parametrs);
  void handleQuit(CarTable& cars, AdTable& ads, const std::string& parametrs);
}
#endif
