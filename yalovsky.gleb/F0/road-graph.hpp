#ifndef YALOVSKY_F0_ROAD_GRAPH_HPP
#define YALOVSKY_F0_ROAD_GRAPH_HPP

#include <cstddef>
#include <string>
#include "avl-map.hpp"

namespace yalovsky
{
  using Distance = unsigned long long;

  bool isValidName(const std::string& name);

  class RoadGraph
  {
  public:
    using Roads = AvlMap< std::string, Distance >;
    using Cities = AvlMap< std::string, Roads >;

    bool hasCity(const std::string& name) const;
    bool hasRoad(const std::string& from, const std::string& to) const;

    void addCity(const std::string& name);
    void removeCity(const std::string& name);
    void addRoad(const std::string& from, const std::string& to, Distance distance);
    void removeRoad(const std::string& from, const std::string& to);

    Distance getRoadWeight(const std::string& from, const std::string& to) const;
    const Roads& getRoads(const std::string& city) const;
    const Cities& getCities() const noexcept;
    std::size_t getRoadCount() const;

  private:
    Cities cities_;
  };
}

#endif
