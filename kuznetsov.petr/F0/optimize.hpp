#ifndef OPTIMIZE_HPP
#define OPTIMIZE_HPP
#include <string>
#include <vector.hpp>
#include <limits>
#include "model.hpp"

namespace kuznetsov {
  const size_t BRUTE_LIMIT = 9;

  void optimize(Map& map, const std::string& base);

  namespace detail {
    constexpr double INF = std::numeric_limits< double >::infinity();
    enum class EdgeKind {
      Board,
      Road,
      Transship
    };
    struct NodeState {
      double dist;
      bool done;
      size_t prev;
      EdgeKind kind;
      size_t len;
      double cost;
      size_t type;
    };

    struct NodeIndex {
      Vector< std::string > cityNames;
      StrHashMap< size_t > cityIdx;
      Vector< std::string > typeNames;
    };

    NodeIndex buildNodeIndex(const Map& m);

    struct Segment {
      std::string from;
      std::string to;
      bool reachable;
      double cost;
      Vector< RouteStep > steps;
    };

    Segment routeSegment(const Map& m, const NodeIndex& idx, const std::string& from, const std::string& to);

    struct SegmentCache {
      Vector< Segment > segments;
      void build(const Map& m, const NodeIndex& idx, const Vector< std::string >& keys);
      const Segment& get(const std::string& a, const std::string& b) const;
    };

    Vector< std::string > keyCities(const Map& m, const std::string& base);
    double fineSum(const Map& m, const Vector< size_t >& perm);
    double sequenceCost(const Map& m, const SegmentCache& cache, const std::string& base, const Vector< size_t >& perm);

    struct PermResult {
      Vector< size_t > order;
      double cost;
    };

    PermResult permute(const Map& map, const SegmentCache& cache, const std::string& base,
      Vector< size_t >& perm, size_t k);
  }
}

#endif
