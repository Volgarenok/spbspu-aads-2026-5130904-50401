#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>

#include <list.hpp>

namespace burukov
{
  class RecommendationGraph
  {
  public:
    RecommendationGraph() = default;

    void addCoLend(const std::string& book1, const std::string& book2, double weight = 1.0);
    void calculateScores(double transitiveWeight = 0.5);
    List< std::pair< std::string, double > > getRecommendations(const std::string& book, size_t k) const;

  private:
    struct GraphNode
    {
      std::string title_;
      double freq_;
      List< std::pair< std::string, double > > edges_;
    };

    List< GraphNode > nodes_;

    GraphNode* findNode(const std::string& title);
    const GraphNode* findNode(const std::string& title) const;
    GraphNode& getOrCreateNode(const std::string& title);
    void addUndirectedEdge(const std::string& node1, const std::string& node2, double weight);
    void normalizeDirectEdges();
    void addTransitiveEdges(double weight);
  };
}

#endif
