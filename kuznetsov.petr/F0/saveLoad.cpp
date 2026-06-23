#include "saveLoad.hpp"
#include "commands.hpp"
#include <iostream>

void kuznetsov::saveMap(std::ostream& out, const Map& m, const std::string& name)
{
  out << "map " << name << '\n';
  for (auto it = m.transports().cbegin(); it != m.transports().cend(); ++it) {
    const RoadType& r = it->second;
    out << "transport " << it->first << ' ' << r.k << ' ' << r.b << ' ' << r.n << '\n';
  }

  Vector< std::string > names;
  for (auto it = m.cities().cbegin(); it != m.cities().cend(); ++it) {
    names.pushBack(it->first);
  }
  detail::sortStrings(names);

  for (size_t i = 0; i < names.getSize(); ++i) {
    out << "city " << names[i] << '\n';
  }
  for (size_t i = 0; i < names.getSize(); ++i) {
    const City& c = m.cities().at(names[i]);
    for (auto t = c.terminals.cbegin(); t != c.terminals.cend(); ++t) {
      out << "terminal " << names[i] << ' ' << t->first << ' ' << t->second << '\n';
    }
  }
  for (size_t i = 0; i < names.getSize(); ++i) {
    const City& c = m.cities().at(names[i]);
    for (auto rt = c.roads.cbegin(); rt != c.roads.cend(); ++rt) {
      const Vector< Edge >& es = rt->second;
      for (size_t e = 0; e < es.getSize(); ++e) {
        if (names[i] < es[e].to) {
          out << "road " << rt->first << ' ' << names[i] << ' ' << es[e].to << ' ' << es[e].dist << '\n';
        }
      }
    }
  }
  for (size_t i = 0; i < m.orders().getSize(); ++i) {
    const Order& o = m.orders()[i];
    out << "order " << o.id << ' ' << o.from << ' ' << o.to << ' ' << o.importance << '\n';
  }
}

void kuznetsov::loadMap(std::istream& in, MapsController& mc)
{
  Map m;
  std::string nameMap;
  std::string tag;
  while (in >> tag) {
    if (tag == "map") {
      in >> nameMap;
    } else if (tag == "transport") {
      std::string type;
      double k = 0.0, b = 0.0, n = 0.0;
      in >> type >> k >> b >> n;
      if (in.fail()) {
        throw std::logic_error("Corrupted file: transport");
      }
      m.setRoadType(type, k, b, n);
    } else if (tag == "city") {
      std::string name;
      in >> name;
      if (in.fail()) {
        throw std::logic_error("Corrupted file: city");
      }
      m.addCity(name);
    } else if (tag == "terminal") {
      std::string city, type;
      double cost = 0.0;
      in >> city >> type >> cost;
      if (in.fail()) {
        throw std::logic_error("Corrupted file: terminal");
      }
      m.addTerminal(city, type, cost);
    } else if (tag == "road") {
      std::string type, a, b;
      size_t dist = 0;
      in >> type >> a >> b >> dist;
      if (in.fail()) {
        throw std::logic_error("Corrupted file: road");
      }
      m.addRoad(type, a, b, dist);
    } else if (tag == "order") {
      Order o{std::string(), std::string(), std::string(), 0};
      in >> o.id >> o.from >> o.to >> o.importance;
      if (in.fail()) {
        throw std::logic_error("Corrupted file: order");
      }
      m.addOrder(o);
    } else {
      throw std::logic_error("Corrupted file: unknown tag");
    }
  }
  mc.newMap(nameMap);
  mc.activeMap() = std::move(m);
}

