#ifndef YALOVSKY_F0_GRAPH_STORAGE_HPP
#define YALOVSKY_F0_GRAPH_STORAGE_HPP

#include <string>
#include "avl-map.hpp"
#include "road-graph.hpp"

namespace yalovsky
{
  class GraphStorage
  {
  public:
    using Graphs = AvlMap< std::string, RoadGraph >;

    void makeGraph(const std::string& name);
    void dropGraph(const std::string& name);

    RoadGraph& getGraph(const std::string& name);
    const RoadGraph& getGraph(const std::string& name) const;
    const Graphs& getGraphs() const noexcept;

    void swap(GraphStorage& other) noexcept;

  private:
    Graphs graphs_;
  };
}

#endif
