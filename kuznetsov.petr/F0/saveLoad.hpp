#ifndef SAVELOAD_H
#define SAVELOAD_H
#include <iosfwd>
#include "model.hpp"
namespace kuznetsov {
  void saveMap(std::ostream& out, const Map& m, const std::string& name);
  void loadMap(std::istream& in, MapsController& m);
}

#endif

