#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include "commands.hpp"
#include "model.hpp"
#include "unorderedMap.hpp"

int main()
{
  namespace kuz = kuznetsov;
  kuz::MapsController mc;
  using hasher = kuz::SipHasher< std::string >;
  using CmdMap = kuz::unorderedMap< std::string, kuz::command, hasher, kuz::detail::StrEqual >;
  using CstMap = kuz::unorderedMap< std::string, kuz::commandConst, hasher, kuz::detail::StrEqual >;

  CmdMap mut;
  mut.insert("add_transport", kuz::addTransport);
  mut.insert("road_type", kuz::roadType);
  mut.insert("add_city", kuz::addCity);
  mut.insert("add_terminal", kuz::addTerminal);
  mut.insert("add_road", kuz::addRoad);
  mut.insert("remove_city", kuz::removeCity);
  mut.insert("remove_road", kuz::removeRoad);
  mut.insert("add_order", kuz::addOrder);
  mut.insert("remove_order", kuz::removeOrder);
  mut.insert("optimize", kuz::optimizeCmd);
  mut.insert("next_step", kuz::nextStep);
  mut.insert("prev_step", kuz::prevStep);
  mut.insert("load", kuz::loadCmd);
  mut.insert("new_map", kuz::newMapCmd);
  mut.insert("switch_map", kuz::switchMapCmd);
  mut.insert("remove_map", kuz::removeMapCmd);

  CstMap cst;
  cst.insert("list_cities", kuz::listCities);
  cst.insert("list_terminals", kuz::listTerminals);
  cst.insert("list_roads", kuz::listRoads);
  cst.insert("list_orders", kuz::listOrders);
  cst.insert("list_maps", kuz::listMaps);
  cst.insert("show_route", kuz::showRoute);
  cst.insert("save", kuz::saveCmd);

  std::string cmd;
  while (std::cout << "> " && std::cin >> cmd) {
    try {
      if (mut.contains(cmd)) {
        mut.at(cmd)(std::cout, std::cin, mc);
      } else if (cst.contains(cmd)) {
        cst.at(cmd)(std::cout, std::cin, mc);
      } else {
        throw std::logic_error("Unknown command");
      }
    } catch (const std::exception& e) {
      std::cout << "<INVALID COMMAND: " << e.what() << "/>" << '\n';
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
