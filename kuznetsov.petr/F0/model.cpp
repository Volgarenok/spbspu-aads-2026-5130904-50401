#include "model.hpp"
#include <cmath>

void kuznetsov::Map::swap(Map& other) noexcept
{
  std::swap(cities_, other.cities_);
  std::swap(transports_, other.transports_);
  std::swap(orders_, other.orders_);
  std::swap(route_, other.route_);
  std::swap(routeCost_, other.routeCost_);
  std::swap(hasRoute_, other.hasRoute_);
  std::swap(cursor_, other.cursor_);
}

bool kuznetsov::detail::StrEqual::operator()(const std::string& a, const std::string& b) const
{
  return a == b;
}

double kuznetsov::RoadType::cost(double x) const
{
  return k * std::pow(x, n) + b;
}

kuznetsov::Map::Map():
  cities_(),
  transports_(),
  orders_(),
  route_(),
  routeCost_(0.0),
  hasRoute_(false),
  cursor_(0)
{}

bool kuznetsov::Map::hasCity(const std::string& name) const
{
  return cities_.contains(name);
}

bool kuznetsov::Map::hasTransport(const std::string& type) const
{
  return transports_.contains(type);
}

bool kuznetsov::Map::hasTerminal(const std::string& city, const std::string& type) const
{
  return cities_.contains(city) && cities_.at(city).terminals.contains(type);
}

bool kuznetsov::Map::hasOrder(const std::string& id) const
{
  for (auto it = orders_.cbegin(); it != orders_.cend(); ++it) {
    if (it->id == id) {
      return true;
    }
  }
  return false;
}

bool kuznetsov::Map::hasRoad(const std::string& type, const std::string& a, const std::string& b) const
{
  if (!cities().contains(a)) {
    return false;
  }
  const City& ca = cities().at(a);
  if (!ca.roads.contains(type)) {
    return false;
  }
  for (size_t i = 0; i < ca.roads.at(type).getSize(); ++i) {
    if (ca.roads.at(type)[i].to == b) {
      return true;
    }
  }
  return false;
}

double kuznetsov::Map::terminalCost(const std::string& city, const std::string& type) const
{
  return cities_.at(city).terminals.at(type);
}

void kuznetsov::Map::addCity(const std::string& name)
{
  if (hasCity(name)) {
    throw std::logic_error("This city already exist");
  }
  cities_.insert(name, City());
  dropRoute();
}

bool kuznetsov::detail::edgeListRemove(Vector< Edge >& edges, const std::string& to)
{
  for (auto it = edges.cbegin(); it != edges.cend(); ++it) {
    if (it->to == to) {
      edges.erase(it);
      return true;
    }
  }
  return false;
}

void kuznetsov::Map::removeCity(const std::string& name)
{
  if (!hasCity(name)) {
    throw std::logic_error("This city doesnt exist");
  }
  Map tmp(*this);
  tmp.cities_.erase(name);
  for (auto it = tmp.cities_.begin(); it != tmp.cities_.end(); ++it) {
    City& c = it->second;
    for (auto rt = c.roads.begin(); rt != c.roads.end(); ++rt) {
      detail::edgeListRemove(rt->second, name);
    }
  }
  Vector< Order > kept;
  for (auto it = tmp.orders_.begin(); it != tmp.orders_.end(); ++it) {
    if (it->from != name && it->to != name) {
      kept.pushBack(*it);
    }
  }
  tmp.orders_ = kept;
  tmp.dropRoute();
  swap(tmp);
}

void kuznetsov::Map::addTransport(const std::string& type)
{
  if (hasTransport(type)) {
    throw std::logic_error("Such transport alredy exist");
  }
  transports_.insert(type, RoadType{ 1, 0, 1 });
  dropRoute();
}

void kuznetsov::Map::setRoadType(const std::string& type, double k, double b, double n)
{
  RoadType r{ k, b, n };
  transports_.insert(type, r);
  dropRoute();
}

void kuznetsov::Map::addTerminal(const std::string& city, const std::string& type, double cost)
{
  if (!hasCity(city)) {
    throw std::logic_error("This city doesnt exist");
  }
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  cities_.at(city).terminals.insert(type, cost);
  dropRoute();
}

void kuznetsov::Map::addRoad(const std::string& type, const std::string& a, const std::string& b, size_t dist)
{
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  if (!hasCity(a) || !hasCity(b)) {
    throw std::logic_error("No such city");
  }
  if (a == b) {
    throw std::logic_error("Road must connect two different cities");
  }
  if (hasRoad(type, a, b)) {
    throw std::logic_error("Road already exists");
  }
  Map tmp(*this);
  City& ca = tmp.cities_.at(a);
  if (!ca.roads.contains(type)) {
    ca.roads.insert(type, Vector< Edge >());
  }
  Edge ea{ b, dist };
  ca.roads.at(type).pushBack(ea);

  City& cb = tmp.cities_.at(b);
  if (!cb.roads.contains(type)) {
    cb.roads.insert(type, Vector< Edge >());
  }
  Edge eb{ a, dist };
  cb.roads.at(type).pushBack(eb);
  tmp.dropRoute();
  swap(tmp);
}

void kuznetsov::Map::removeRoad(const std::string& type, const std::string& a, const std::string& b)
{
  if (!hasTransport(type)) {
    throw std::logic_error("No such transport type");
  }
  if (!hasCity(a) || !hasCity(b)) {
    throw std::logic_error("No such city");
  }
  if (a == b) {
    throw std::logic_error("Road must connect two different cities");
  }
  if (!hasRoad(type, a, b)) {
    throw std::logic_error("Road doesnt exists");
  }
  Map tmp(*this);
  detail::edgeListRemove(tmp.cities_.at(a).roads.at(type), b);
  detail::edgeListRemove(tmp.cities_.at(b).roads.at(type), a);
  tmp.dropRoute();
  swap(tmp);
}

void kuznetsov::Map::addOrder(const Order& o)
{
  if (hasOrder(o.id)) {
    throw std::logic_error("Order already exist");
  }
  if (!hasCity(o.from) || !hasCity(o.to)) {
    throw std::logic_error("No such city");
  }
  if (o.importance < 1 || o.importance > 10) {
    throw std::logic_error("Importance must be in [1, 10]");
  }
  orders_.pushBack(o);
  dropRoute();
}

void kuznetsov::Map::removeOrder(const std::string& id)
{
  for (auto it = orders_.cbegin(); it != orders_.cend(); ++it) {
    if (it->id == id) {
      orders_.erase(it);
      dropRoute();
      return;
    }
  }
  throw std::logic_error("No such order");
}

void kuznetsov::Map::clear()
{
  cities_ = StrHashMap< City >();
  transports_ = map< std::string, RoadType >();
  orders_ = Vector< Order >();
  dropRoute();
}

void kuznetsov::Map::dropRoute()
{
  route_ = Vector< RouteStep >();
  routeCost_ = 0.0;
  hasRoute_ = false;
  cursor_ = 0;
}

const kuznetsov::StrHashMap< kuznetsov::City >& kuznetsov::Map::cities() const
{
  return cities_;
}

const kuznetsov::map< std::string, kuznetsov::RoadType >& kuznetsov::Map::transports() const
{
  return transports_;
}

const kuznetsov::Vector< kuznetsov::Order >& kuznetsov::Map::orders() const
{
  return orders_;
}

bool kuznetsov::Map::hasRoute() const
{
  return hasRoute_;
}

double kuznetsov::Map::routeCost() const
{
  return routeCost_;
}

const kuznetsov::Vector< kuznetsov::RouteStep >& kuznetsov::Map::route() const
{
  return route_;
}

size_t kuznetsov::Map::cursor() const
{
  return cursor_;
}

void kuznetsov::Map::setRoute(Vector< RouteStep > steps, double cost)
{
  route_ = std::move(steps);
  routeCost_ = cost;
  hasRoute_ = true;
  cursor_ = 0;
}

void kuznetsov::Map::cursorNext()
{
  if (cursor_ + 1 < route_.getSize()) {
    ++cursor_;
  }
}

void kuznetsov::Map::cursorPrev()
{
  if (cursor_ > 0) {
    --cursor_;
  }
}

kuznetsov::MapsController::MapsController():
  maps_(),
  active_("default")
{
  maps_.insert("default", Map());
}

kuznetsov::Map& kuznetsov::MapsController::activeMap()
{
  return maps_.at(active_);
}

const kuznetsov::Map& kuznetsov::MapsController::activeMap() const
{
  return maps_.at(active_);
}

void kuznetsov::MapsController::newMap(const std::string& name)
{
  if (maps_.contains(name)) {
    throw std::logic_error("Such map already exist");
  }
  maps_.insert(name, Map());
  active_ = name;
}

void kuznetsov::MapsController::switchMap(const std::string& name)
{
  if (!maps_.contains(name)) {
    throw std::logic_error("Such map doesnt exist");
  }
  active_ = name;
}

void kuznetsov::MapsController::removeMap(const std::string& name)
{
  if (!maps_.contains(name)) {
    throw std::logic_error("Such map doesnt exist");
  }
  maps_.erase(name);
  if (maps_.size() == 0) {
    maps_.insert("default", Map());
    active_ = "default";
  } else if (active_ == name) {
    active_ = maps_.cbegin()->first;
  }
}

const std::string& kuznetsov::MapsController::activeName() const
{
  return active_;
}
const kuznetsov::map< std::string, kuznetsov::Map >& kuznetsov::MapsController::maps() const
{
  return maps_;
}





