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
  void printAd(const Ad&, size_t&);

  void handleScroll(CarTable&, AdTable&);
  void handleSort(CarTable&, AdTable&);
  void handleMake(CarTable&, AdTable&);
  void handleBuy(CarTable&, AdTable&);
  void handleDelete(CarTable&, AdTable&);
  void handleShowHistory(CarTable&, AdTable&);
}
#endif
