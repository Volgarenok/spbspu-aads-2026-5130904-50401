#include "commands.hpp"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "optimize.hpp"
#include "saveLoad.hpp"

void kuznetsov::detail::sortStrings(Vector< std::string >& v)
{
  for (size_t i = 1; i < v.getSize(); ++i) {
    size_t min = i;
    for (size_t j = i + 1; j < v.getSize(); ++j) {
      if (v[j] < v[min]) {
        min = j;
      }
    }
    if (min != i) {
      std::swap(v[min], v[i]);
    }
  }
}

std::string kuznetsov::detail::reqStr(std::istream& in)
{
  std::string s;
  if (!(in >> s)) {
    throw std::logic_error("Smth go bad");
  }
  return s;
}

size_t kuznetsov::detail::reqSize(std::istream& in)
{
  size_t s;
  if (!(in >> s)) {
    throw std::logic_error("Invalid argument");
  }
  return s;
}

double kuznetsov::detail::reqDouble(std::istream& in)
{
  double d;
  if (!(in >> d)) {
    throw std::logic_error("Invalid argument");
  }
  return d;
}

bool kuznetsov::detail::readOptional(std::istream& in, std::string& out)
{
  int c = in.peek();
  while (c == ' ' || c == '\t') {
    in.get();
    c = in.peek();
  }
  if (c == '\n' || c == -1) {
    return false;
  }
  return static_cast< bool >(in >> out);
}

void kuznetsov::addTransport(std::ostream&, std::istream& in, MapsController& m)
{
  m.activeMap().addTransport(detail::reqStr(in));
}

void kuznetsov::roadType(std::ostream&, std::istream& in, MapsController& m)
{
  std::string name = detail::reqStr(in);
  double k = detail::reqDouble(in);
  double b = detail::reqDouble(in);
  double n = detail::reqDouble(in);
  m.activeMap().setRoadType(name, k, b, n);
}

void kuznetsov::addCity(std::ostream&, std::istream& in, MapsController& m)
{
  m.activeMap().addCity(detail::reqStr(in));
}

void kuznetsov::addTerminal(std::ostream&, std::istream& in, MapsController& m)
{
  std::string city = detail::reqStr(in);
  std::string type = detail::reqStr(in);
  double cost = detail::reqDouble(in);
  m.activeMap().addTerminal(city, type, cost);
}

void kuznetsov::addRoad(std::ostream&, std::istream& in, MapsController& m)
{
  std::string type = detail::reqStr(in);
  std::string a = detail::reqStr(in);
  std::string b = detail::reqStr(in);
  size_t dist = detail::reqSize(in);
  m.activeMap().addRoad(type, a, b, dist);
}

void kuznetsov::addOrder(std::ostream&, std::istream& in, MapsController& m)
{
  Order o;
  o.id = detail::reqStr(in);
  o.from = detail::reqStr(in);
  o.to = detail::reqStr(in);
  o.importance = detail::reqSize(in);
  m.activeMap().addOrder(o);
}

void kuznetsov::removeCity(std::ostream&, std::istream& in, MapsController& m)
{
  std::string name = detail::reqStr(in);
  m.activeMap().removeCity(name);
}

void kuznetsov::removeRoad(std::ostream&, std::istream& in, MapsController& m)
{
  std::string type = detail::reqStr(in);
  std::string a = detail::reqStr(in);
  std::string b = detail::reqStr(in);
  m.activeMap().removeRoad(type, a, b);
}

void kuznetsov::removeOrder(std::ostream&, std::istream& in, MapsController& m)
{
  m.activeMap().removeOrder(detail::reqStr(in));
}

void kuznetsov::removeMapCmd(std::ostream&, std::istream& in, MapsController& m)
{
  m.removeMap(detail::reqStr(in));
}

void kuznetsov::nextStep(std::ostream& out, std::istream&, MapsController& mc)
{
  Map& m = mc.activeMap();
  if (!m.hasRoute() || m.route().getSize() == 0) {
    throw std::logic_error("No active route");
  }
  m.cursorNext();
  const RouteStep& s = m.route()[m.cursor()];
  out << '[' << m.cursor() << "] " << s.desc;
  if (s.hasValue) out << s.cost;
  out << '\n';
}

void kuznetsov::prevStep(std::ostream& out, std::istream&, MapsController& mc)
{
  Map& m = mc.activeMap();
  if (!m.hasRoute() || m.route().getSize() == 0) {
    throw std::logic_error("No active route");
  }
  m.cursorPrev();
  const RouteStep& s = m.route()[m.cursor()];
  out << '[' << m.cursor() << "] " << s.desc;
  if (s.hasValue) out << s.cost;
  out << '\n';
}

void kuznetsov::newMapCmd(std::ostream&, std::istream& in, MapsController& mc)
{
  mc.newMap(detail::reqStr(in));
}

void kuznetsov::switchMapCmd(std::ostream&, std::istream& in, MapsController& mc)
{
  mc.switchMap(detail::reqStr(in));
}

void kuznetsov::listCities(std::ostream& out, std::istream&, const MapsController& mc)
{
  const Map& m = mc.activeMap();
  if (m.cities().empty()) {
    out << "List of cities is empty\n";
    return;
  }
  Vector< std::string > names;
  for (auto it = m.cities().cbegin(); it != m.cities().cend(); ++it) {
    names.pushBack(it->first);
  }
  detail::sortStrings(names);
  for (size_t i = 0; i < names.getSize(); ++i) {
    out << (i + 1) << ". " << names[i] << '\n';
  }
}

void kuznetsov::listTerminals(std::ostream& out, std::istream& in, const MapsController& mc)
{
  const Map& m = mc.activeMap();
  std::string city;
  bool filter = detail::readOptional(in, city);
  if (filter && !m.hasCity(city)) {
    throw std::logic_error("No such city");
  }
  Vector< std::string > names;
  for (auto it = m.cities().cbegin(); it != m.cities().cend(); ++it) {
    names.pushBack(it->first);
  }
  detail::sortStrings(names);
  bool any = false;
  for (size_t i = 0; i < names.getSize(); ++i) {
    if (filter && names[i] != city) {
      continue;
    }
    const City& c = m.cities().at(names[i]);
    for (auto t = c.terminals.cbegin(); t != c.terminals.cend(); ++t) {
      out << names[i] << ": " << t->first << " (" << t->second << ")\n";
      any = true;
    }
  }
  if (!any) {
    out << "List of terminals is empty\n";
  }
}

void kuznetsov::listRoads(std::ostream& out, std::istream& in, const MapsController& mc)
{
  const Map& m = mc.activeMap();
  std::string type;
  bool filter = detail::readOptional(in, type);
  if (filter && !m.hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  Vector< std::string > names;
  for (auto it = m.cities().cbegin(); it != m.cities().cend(); ++it) {
    names.pushBack(it->first);
  }
  detail::sortStrings(names);
  bool any = false;
  for (size_t i = 0; i < names.getSize(); ++i) {
    const City& c = m.cities().at(names[i]);
    for (auto rt = c.roads.cbegin(); rt != c.roads.cend(); ++rt) {
      if (filter && rt->first != type) {
        continue;
      }
      const Vector< Edge >& es = rt->second;
      for (size_t e = 0; e < es.getSize(); ++e) {
        if (names[i] < es[e].to) {
          out << rt->first << ": " << names[i] << " -> " << es[e].to << " (" << es[e].dist << " km)\n";
          any = true;
        }
      }
    }
  }
  if (!any) {
    out << "List of roads is empty\n";
  }
}

void kuznetsov::listOrders(std::ostream& out, std::istream&, const MapsController& mc)
{
  const Map& m = mc.activeMap();
  if (m.orders().getSize() == 0) {
    out << "List of orders is empty\n";
    return;
  }
  for (size_t i = 0; i < m.orders().getSize(); ++i) {
    const Order& o = m.orders()[i];
    out << (i + 1) << ". ";
    out << o.id << ' ' << o.from << ' ' << o.to << ' ' << o.importance << '\n';
  }
}

void kuznetsov::listMaps(std::ostream& out, std::istream&, const MapsController& mc)
{
  for (auto it = mc.maps().cbegin(); it != mc.maps().cend(); ++it) {
    out << (it->first == mc.activeName() ? "* " : "  ") << it->first << '\n';
  }
}

void kuznetsov::showRoute(std::ostream& out, std::istream&, const MapsController& mc)
{
  const Map& m = mc.activeMap();
  if (!m.hasRoute()) {
    throw std::logic_error("No active route");
  }
  out << "total " << m.routeCost() << '\n';
  for (size_t i = 0; i < m.route().getSize(); ++i) {
    const RouteStep& s = m.route()[i];
    out << '[' << i << "] " << s.desc;
    if (s.hasValue) out << s.cost;
    out << '\n';
  }
}

void kuznetsov::saveCmd(std::ostream& out, std::istream& in, const MapsController& mc)
{
  std::string file = detail::reqStr(in);
  std::ofstream os(file);
  if (!os) {
    throw std::logic_error("Failed to create a file " + file);
  }
  saveMap(os, mc.activeMap(), mc.activeName());
  out << "Atlas saved to a file " << file << "\n";
}

void kuznetsov::optimizeCmd(std::ostream& out, std::istream& in, MapsController& mc)
{
  std::string base = detail::reqStr(in);
  Map& m = mc.activeMap();
  optimize(m, base);
  out << "total " << m.routeCost() << '\n';
}

void kuznetsov::loadCmd(std::ostream& out, std::istream& in, MapsController& mc)
{
  std::string file = detail::reqStr(in);
  std::ifstream input(file);
  if (!input) {
    throw std::logic_error("Cannot open file for reading");
  }
  loadMap(input, mc);
  out << "Atlas loaded from " << file << "\n";
}
