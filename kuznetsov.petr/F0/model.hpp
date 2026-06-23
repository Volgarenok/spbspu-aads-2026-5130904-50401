#ifndef MODEL_HPP
#define MODEL_HPP
#include <string>
#include <vector.hpp>
#include <vectorIterators.hpp>
#include "Hasher.hpp"
#include "map.hpp"
#include "unorderedMap.hpp"

namespace kuznetsov {
  struct RoadType {
    double k;
    double b;
    double n;
    double cost(double x) const;
  };

  struct Edge {
    std::string to;
    size_t dist;
  };

  namespace detail {
    struct StrEqual {
      bool operator()(const std::string& a, const std::string& b) const;
    };
    bool edgeListRemove(Vector< Edge >& edges, const std::string& to);
  }

  template< class V >
  using StrHashMap = unorderedMap< std::string, V, SipHasher< std::string >, detail::StrEqual >;

  struct City {
    StrHashMap< Vector< Edge > > roads;
    StrHashMap< double > terminals;
  };

  struct Order {
    std::string id;
    std::string from;
    std::string to;
    size_t importance;
  };

  struct RouteStep {
    std::string desc;
    double cost;
    bool hasValue;
  };

  struct Map {
    Map();

    bool hasCity(const std::string& name) const;
    bool hasTransport(const std::string& type) const;
    bool hasTerminal(const std::string& city, const std::string& type) const;
    bool hasOrder(const std::string& id) const;
    bool hasRoad(const std::string& type, const std::string& a, const std::string& b) const;

    double terminalCost(const std::string& city, const std::string& type) const;

    void addCity(const std::string& name);
    void removeCity(const std::string& name);

    void addTransport(const std::string& type);
    void setRoadType(const std::string& type, double k, double b, double n);

    void addTerminal(const std::string& city, const std::string& type, double cost);

    void addRoad(const std::string& type, const std::string& a, const std::string& b, size_t dist);
    void removeRoad(const std::string& type, const std::string& a, const std::string& b);

    void addOrder(const Order& o);
    void removeOrder(const std::string& id);

    void clear();

    const StrHashMap< City >& cities() const;
    const map< std::string, RoadType >& transports() const;
    const Vector< Order >& orders() const;

    bool hasRoute() const;
    double routeCost() const;
    const Vector< RouteStep >& route() const;
    size_t cursor() const;
    void setRoute(Vector< RouteStep > steps, double cost);
    void dropRoute();
    void cursorNext();
    void cursorPrev();

    void swap(Map& other) noexcept;
  private:
    StrHashMap< City > cities_;
    map< std::string, RoadType > transports_;
    Vector< Order > orders_;

    Vector< RouteStep > route_;
    double routeCost_;
    bool hasRoute_;
    size_t cursor_;
  };

  struct MapsController {
    MapsController();

    Map& activeMap();
    const Map& activeMap() const;
    const std::string& activeName() const;
    const map< std::string, Map >& maps() const;

    void newMap(const std::string& name);
    void switchMap(const std::string& name);
    void removeMap(const std::string& name);
  private:
    map< std::string, Map > maps_;
    std::string active_;
  };

}

#endif
