#ifndef YALOVSKY_F0_ALGORITHMS_HPP
#define YALOVSKY_F0_ALGORITHMS_HPP

#include <cstddef>
#include <string>
#include <vector>
#include "road-graph.hpp"

namespace yalovsky
{
  struct PathResult
  {
    std::vector< std::string > cities;
    std::size_t size;
    Distance distance;
  };

  struct RoadSuggestion
  {
    std::string from;
    std::string to;
    Distance weight;
    Distance improvement;
  };

  PathResult findShortestPath(
      const RoadGraph& graph,
      const std::string& from,
      const std::string& to);

  RoadSuggestion suggestRoad(
      const RoadGraph& graph,
      Distance weight);
}

#endif