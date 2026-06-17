#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include <iostream>
#include "structures.hpp"

namespace vasyakin
{
  void cmdCreateWarehouse(std::istream& in, std::ostream& out, SystemState& state);
  void cmdAddItem(std::istream& in, std::ostream& out, SystemState& state);
  void cmdRemoveItem(std::istream& in, std::ostream& out, SystemState& state);
  void cmdShip(std::istream& in, std::ostream& out, SystemState& state);

  void cmdShowWarehouse(std::istream& in, std::ostream& out, SystemState& state);
  void cmdShowTransfers(std::istream& in, std::ostream& out, SystemState& state);
  void cmdShowItem(std::istream& in, std::ostream& out, SystemState& state);
  void cmdShowLog(std::istream& in, std::ostream& out, SystemState& state);
  void cmdShowStateAt(std::istream& in, std::ostream& out, SystemState& state);

  void cmdStats(std::istream& in, std::ostream& out, SystemState& state);
  void cmdCalculateCenterCapacity(
    std::istream& in, std::ostream& out, SystemState& state);
  void cmdHelp(std::istream& in, std::ostream& out, SystemState& state);
  void cmdSave(std::istream& in, std::ostream& out, SystemState& state);
  void cmdLoad(std::istream& in, std::ostream& out, SystemState& state);
}

#endif
