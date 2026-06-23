#ifndef COMMANDS_HPP
#define COMMANDS_HPP
#include "model.hpp"
#include <cstddef>
#include <iosfwd>
#include <string>
#include <vector.hpp>

namespace kuznetsov {
  using command = void (*)(std::ostream&, std::istream&, MapsController&);
  using commandConst = void (*)(std::ostream&, std::istream&, const MapsController&);

  namespace detail {
    std::string reqStr(std::istream& in);
    size_t reqSize(std::istream& in);
    double reqDouble(std::istream& in);
    bool readOptional(std::istream& in, std::string& out);
    void sortStrings(Vector< std::string >&);
  }

  void addTransport(std::ostream&, std::istream&, MapsController&);
  void roadType(std::ostream&, std::istream&, MapsController&);
  void addCity(std::ostream&, std::istream&, MapsController&);
  void addTerminal(std::ostream&, std::istream&, MapsController&);
  void addRoad(std::ostream&, std::istream&, MapsController&);
  void removeCity(std::ostream&, std::istream&, MapsController&);
  void removeRoad(std::ostream&, std::istream&, MapsController&);
  void addOrder(std::ostream&, std::istream&, MapsController&);
  void removeOrder(std::ostream&, std::istream&, MapsController&);
  void optimizeCmd(std::ostream&, std::istream&, MapsController&);
  void nextStep(std::ostream&, std::istream&, MapsController&);
  void prevStep(std::ostream&, std::istream&, MapsController&);
  void loadCmd(std::ostream&, std::istream&, MapsController&);
  void newMapCmd(std::ostream&, std::istream&, MapsController&);
  void switchMapCmd(std::ostream&, std::istream&, MapsController&);
  void removeMapCmd(std::ostream&, std::istream&, MapsController&);

  void listCities(std::ostream&, std::istream&, const MapsController&);
  void listTerminals(std::ostream&, std::istream&, const MapsController&);
  void listRoads(std::ostream&, std::istream&, const MapsController&);
  void listOrders(std::ostream&, std::istream&, const MapsController&);
  void listMaps(std::ostream&, std::istream&, const MapsController&);
  void showRoute(std::ostream&, std::istream&, const MapsController&);
  void saveCmd(std::ostream&, std::istream&, const MapsController&);
}

#endif
