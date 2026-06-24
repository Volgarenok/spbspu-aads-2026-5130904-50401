#include "graph.hpp"
#include <cmath>

burukov::RecommendationGraph::GraphNode* burukov::RecommendationGraph::findNode(const std::string& title)
{
  for (auto it = nodes_.begin(); it != nodes_.end(); ++it)
  {
    if (it->title_ == title)
    {
      return &(*it);
    }
  }
  return nullptr;
}

const burukov::RecommendationGraph::GraphNode* burukov::RecommendationGraph::findNode(const std::string& title) const
{
  for (auto it = nodes_.cbegin(); it != nodes_.cend(); ++it)
  {
    if (it->title_ == title)
    {
      return &(*it);
    }
  }
  return nullptr;
}

burukov::RecommendationGraph::GraphNode& burukov::RecommendationGraph::getOrCreateNode(const std::string& title)
{
  GraphNode* node = findNode(title);
  if (node != nullptr)
  {
    return *node;
  }
  GraphNode newNode;
  newNode.title_ = title;
  newNode.freq_ = 0.0;
  nodes_.pushBack(std::move(newNode));
  GraphNode* last = nullptr;
  for (auto it = nodes_.begin(); it != nodes_.end(); ++it)
  {
    last = &(*it);
  }
  return *last;
}

void burukov::RecommendationGraph::addUndirectedEdge(const std::string& node1, const std::string& node2, double weight)
{
  GraphNode& nodeA = getOrCreateNode(node1);
  GraphNode& nodeB = getOrCreateNode(node2);
  nodeA.freq_ += weight;
  nodeB.freq_ += weight;
  auto updateEdge = [weight](GraphNode& src, const std::string& dst)
  {
    bool found = false;
    for (auto it = src.edges_.begin(); it != src.edges_.end(); ++it)
    {
      if (it->first == dst)
      {
        it->second += weight;
        found = true;
        break;
      }
    }
    if (!found)
    {
      src.edges_.pushBack(std::make_pair(dst, weight));
    }
  };
  updateEdge(nodeA, node2);
  updateEdge(nodeB, node1);
}

void burukov::RecommendationGraph::addCoLend(const std::string& book1, const std::string& book2, double weight)
{
  if (book1 == book2)
  {
    return;
  }
  addUndirectedEdge(book1, book2, weight);
}

void burukov::RecommendationGraph::normalizeDirectEdges()
{
  for (auto itA = nodes_.begin(); itA != nodes_.end(); ++itA)
  {
    for (auto itB = itA->edges_.begin(); itB != itA->edges_.end(); ++itB)
    {
      const std::string& target = itB->first;
      const double co = itB->second;
      const GraphNode* nodeB = findNode(target);
      const double freqB = (nodeB != nullptr) ? nodeB->freq_ : 1.0;
      const double freqA = (itA->freq_ > 0.0) ? itA->freq_ : 1.0;
      itB->second = co / std::sqrt(freqA * freqB);
    }
  }
}

void burukov::RecommendationGraph::addTransitiveEdges(double weight)
{
  for (auto itA = nodes_.begin(); itA != nodes_.end(); ++itA)
  {
    List< std::pair< std::string, double > > edgesCopy;
    for (auto it = itA->edges_.cbegin(); it != itA->edges_.cend(); ++it)
    {
      edgesCopy.pushBack(*it);
    }
    for (auto itB = edgesCopy.cbegin(); itB != edgesCopy.cend(); ++itB)
    {
      const std::string& mid = itB->first;
      const double scoreAB = itB->second;
      const GraphNode* nodeB = findNode(mid);
      if (nodeB == nullptr)
      {
        continue;
      }
      for (auto itC = nodeB->edges_.cbegin(); itC != nodeB->edges_.cend(); ++itC)
      {
        const std::string& targetC = itC->first;
        if (itA->title_ == targetC)
        {
          continue;
        }
        const double transitive = scoreAB * itC->second * weight;
        bool found = false;
        for (auto itUpdate = itA->edges_.begin(); itUpdate != itA->edges_.end(); ++itUpdate)
        {
          if (itUpdate->first == targetC)
          {
            itUpdate->second += transitive;
            found = true;
            break;
          }
        }
        if (!found)
        {
          itA->edges_.pushBack(std::make_pair(targetC, transitive));
        }
      }
    }
  }
}

void burukov::RecommendationGraph::calculateScores(double transitiveWeight)
{
  normalizeDirectEdges();
  addTransitiveEdges(transitiveWeight);
}

burukov::List< std::pair< std::string, double > >
burukov::RecommendationGraph::getRecommendations(const std::string& book, size_t k) const
{
  List< std::pair< std::string, double > > result;
  const GraphNode* node = findNode(book);
  if (node == nullptr)
  {
    return result;
  }
  for (auto it = node->edges_.cbegin(); it != node->edges_.cend(); ++it)
  {
    result.pushBack(*it);
  }
  result.sort(
    [](const std::pair< std::string, double >& a, const std::pair< std::string, double >& b)
    {
      return a.second > b.second;
    }
  );
  List< std::pair< std::string, double > > topK;
  size_t cnt = 0;
  for (auto it = result.cbegin(); it != result.cend() && cnt < k; ++it, ++cnt)
  {
    topK.pushBack(*it);
  }
  return topK;
}
