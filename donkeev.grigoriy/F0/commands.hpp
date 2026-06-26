#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "robinTable.hpp"
#include "Ad.hpp"
#include "helpedFunc.hpp"
#include "robinTableStructs.hpp"

namespace donkeev
{
  using CarTable = donkeev::RobinTable<std::string, Car, StringHash, StringEqual>;
  using AdTable = donkeev::RobinTable<size_t, Ad, SizeTHash, SizeTEqual>;
  using AdIterator = RobinIter<size_t, Ad, SizeTHash, SizeTEqual>;
  using AdConstIterator = RobinCIter<size_t, Ad, SizeTHash, SizeTEqual>;

  void printCar(const Car& car);
  void saveCarToDataBase(const Car&, const std::string&);
  size_t getHistorySize();
  size_t generateNextAdId(AdTable&, size_t);
  void saveAdToHistory(const Ad&);

  void handleScroll(CarTable& cars, AdTable& ads);
  void handleMake(CarTable& cars, AdTable& ads);
  void handleBuy(CarTable& cars, AdTable& ads);
  void handleDelete(CarTable& cars, AdTable& ads);
  void handleShowHistory(CarTable& cars, AdTable& ads);
}
#endif
