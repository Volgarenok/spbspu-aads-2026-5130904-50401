#include "algorithms.hpp"

#include <algorithm>
#include <limits>
#include <stdexcept>
#include "avl-map.hpp"

namespace yalovsky
{
  namespace detail
  {
    struct PathState
    {
      Distance distance;
      std::string previous;
      bool reachable;
      bool visited;
      bool overflow;
    };

    using DistanceTable = AvlMap< std::string, PathState >;

    bool isCloser(
        Distance candidateDistance,
        bool candidateOverflow,
        Distance currentDistance,
        bool currentOverflow)
    {
      if (candidateOverflow)
      {
        return false;
      }

      if (currentOverflow)
      {
        return true;
      }

      return candidateDistance < currentDistance;
    }

    bool improvesPath(
        const PathState& currentPath,
        Distance newDistance,
        bool newOverflow)
    {
      if (!currentPath.reachable)
      {
        return true;
      }

      return isCloser(
          newDistance,
          newOverflow,
          currentPath.distance,
          currentPath.overflow);
    }

    DistanceTable findDistances(
        const RoadGraph& graph,
        const std::string& start)
    {
      if (!graph.hasCity(start))
      {
        throw std::out_of_range("City not found");
      }

      DistanceTable states;

      graph.getCities().forEach(
          [&states](
              const std::string& city,
              const RoadGraph::Roads&)
          {
            states.insert(
                city,
                PathState{0, "", false, false, false});
          });

      states.at(start).reachable = true;

      const Distance maximum =
          std::numeric_limits< Distance >::max();

      while (true)
      {
        std::string currentCity;
        Distance bestDistance = 0;
        bool bestOverflow = false;
        bool cityFound = false;

        states.forEach(
            [&currentCity, &bestDistance,
                &bestOverflow, &cityFound](
                const std::string& city,
                const PathState& state)
            {
              if (!state.reachable || state.visited)
              {
                return;
              }

              if (!cityFound ||
                  isCloser(
                      state.distance,
                      state.overflow,
                      bestDistance,
                      bestOverflow))
              {
                currentCity = city;
                bestDistance = state.distance;
                bestOverflow = state.overflow;
                cityFound = true;
              }
            });

        if (!cityFound)
        {
          break;
        }

        PathState& currentState = states.at(currentCity);
        currentState.visited = true;

        graph.getRoads(currentCity).forEach(
            [&states, &currentCity,
                &currentState](
                const std::string& nextCity,
                const Distance& roadLength)
            {
              PathState& nextState = states.at(nextCity);

              if (nextState.visited)
              {
                return;
              }

              bool newOverflow = currentState.overflow;
              Distance newDistance = 0;

              if (!newOverflow)
              {
                if (roadLength >
                    maximum - currentState.distance)
                {
                  newOverflow = true;
                }
                else
                {
                  newDistance =
                      currentState.distance + roadLength;
                }
              }

              if (improvesPath(
                      nextState,
                      newDistance,
                      newOverflow))
              {
                nextState.distance = newDistance;
                nextState.previous = currentCity;
                nextState.reachable = true;
                nextState.overflow = newOverflow;
              }
            });
      }

      return states;
    }
  }

  PathResult findShortestPath(
      const RoadGraph& graph,
      const std::string& from,
      const std::string& to)
  {
    if (!graph.hasCity(to))
    {
      throw std::out_of_range("City not found");
    }

    const detail::DistanceTable states =
        detail::findDistances(graph, from);

    const detail::PathState& destination = states.at(to);

    if (!destination.reachable)
    {
      throw std::logic_error("Path not found");
    }

    if (destination.overflow)
    {
      throw std::overflow_error(
          "Path distance is too large");
    }

    PathResult result{{}, 0, destination.distance};
    result.cities.reserve(graph.getCities().size());

    std::string currentCity = to;

    while (true)
    {
      result.cities.push_back(currentCity);

      if (currentCity == from)
      {
        break;
      }

      currentCity = states.at(currentCity).previous;
    }

    std::reverse(
        result.cities.begin(),
        result.cities.end());

    result.size = result.cities.size();
    return result;
  }

  RoadSuggestion suggestRoad(
      const RoadGraph& graph,
      Distance weight)
  {
    if (weight == 0)
    {
      throw std::invalid_argument(
          "Invalid road weight");
    }

    RoadSuggestion best{"", "", weight, 0};

    graph.getCities().forEach(
        [&graph, &best, weight](
            const std::string& from,
            const RoadGraph::Roads&)
        {
          const detail::DistanceTable states =
              detail::findDistances(graph, from);

          states.forEach(
              [&graph, &best, &from, weight](
                  const std::string& to,
                  const detail::PathState& state)
              {
                if (!(from < to))
                {
                  return;
                }

                if (graph.hasRoad(from, to) ||
                    !state.reachable)
                {
                  return;
                }

                if (state.overflow)
                {
                  throw std::overflow_error(
                      "Cannot calculate improvement");
                }

                if (state.distance <= weight)
                {
                  return;
                }

                const Distance improvement =
                    state.distance - weight;

                if (improvement > best.improvement)
                {
                  best.from = from;
                  best.to = to;
                  best.improvement = improvement;
                }
              });
        });

    if (best.improvement == 0)
    {
      throw std::logic_error("No improving road");
    }

    return best;
  }
}
