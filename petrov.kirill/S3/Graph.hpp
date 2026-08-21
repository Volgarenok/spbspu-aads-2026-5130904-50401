#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "HashTable.hpp"
#include "list.hpp"
#include <stdexcept>

namespace petrov
{
  struct Edge
  {
    int to;
    int weight;

    bool operator==(const Edge &other) const
    {
      return to == other.to && weight == other.weight;
    }
  };

  class Graph
  {
  private:
    HashTable< int, List< Edge > > adjList_;

    List< Edge > &getList(int v)
    {
      auto it = adjList_.find(v);
      if (it == adjList_.end())
      {
        throw std::runtime_error("");
      }
      return it->second;
    }

    const List< Edge > &getList(int v) const
    {
      auto it = adjList_.find(v);
      if (it == adjList_.end())
      {
        throw std::runtime_error("");
      }
      return it->second;
    }

    bool hasEdge(int u, int v, int w) const
    {
      if (!adjList_.contains(u))
      {
        return false;
      }
      for (const auto &edge : getList(u))
      {
        if (edge.to == v && edge.weight == w)
        {
          return true;
        }
      }
      return false;
    }

  public:
    Graph() = default;
    Graph(const Graph &other) : adjList_(other.adjList_) {}

    Graph &operator=(const Graph &other)
    {
      if (this != &other)
      {
        adjList_ = other.adjList_;
      }
      return *this;
    }

    void addVertex(int v)
    {
      if (!adjList_.contains(v))
      {
        adjList_.add(v, List< Edge >());
      }
    }

    void addWeightedEdge(int u, int v, int w)
    {
      addVertex(u);
      addVertex(v);
      if (!hasEdge(u, v, w))
      {
        getList(u).push_back({v, w});
        getList(v).push_back({u, w});
      }
    }

    void addEdge(int u, int v)
    {
      addWeightedEdge(u, v, 0);
    }

    void cut(int u, int v, int w)
    {
      if (adjList_.contains(u))
      {
        List< Edge > &neighborsU = getList(u);
        for (auto it = neighborsU.begin(); it != neighborsU.end(); ++it)
        {
          if (it->to == v && it->weight == w)
          {
            neighborsU.erase(it);
            break;
          }
        }
      }
      if (adjList_.contains(v))
      {
        List< Edge > &neighborsV = getList(v);
        for (auto it = neighborsV.begin(); it != neighborsV.end(); ++it)
        {
          if (it->to == u && it->weight == w)
          {
            neighborsV.erase(it);
            break;
          }
        }
      }
    }

    void cut(int u, int v)
    {
      cut(u, v, 0);
    }

    void swap(Graph &other) noexcept
    {
      adjList_.swap(other.adjList_);
    }

    bool hasVertex(int v) const
    {
      return adjList_.contains(v);
    }

    void removeVertex(int v)
    {
      if (!adjList_.contains(v))
      {
        return;
      }
      adjList_.erase(v);
      for (auto it = adjList_.begin(); it != adjList_.end(); ++it)
      {
        List< Edge > &list = it->second;
        for (auto nIt = list.begin(); nIt != list.end(); )
        {
          if (nIt->to == v)
          {
            nIt = list.erase(nIt);
          }
          else
          {
            ++nIt;
          }
        }
      }
    }

    size_t getVertexCount() const
    {
      return adjList_.size();
    }

    List< int > getAllVertices() const
    {
      List< int > vertices;
      for (auto it = adjList_.begin(); it != adjList_.end(); ++it)
      {
        vertices.push_back(it->first);
      }
      return vertices;
    }

    List< int > getOutbound(int v) const
    {
      List< int > res;
      if (adjList_.contains(v))
      {
        for (const auto &edge : getList(v))
        {
          res.push_back(edge.to);
        }
      }
      return res;
    }

    List< int > getInbound(int v) const
    {
      List< int > inbound;
      for (auto it = adjList_.begin(); it != adjList_.end(); ++it)
      {
        for (const auto &edge : it->second)
        {
          if (edge.to == v)
          {
            inbound.push_back(it->first);
          }
        }
      }
      return inbound;
    }

    Graph merge(const Graph &other) const
    {
      Graph result = *this;
      for (auto it = other.adjList_.begin(); it != other.adjList_.end(); ++it)
      {
        result.addVertex(it->first);
        for (const auto &edge : it->second)
        {
          if (it->first <= edge.to)
          {
            result.addWeightedEdge(it->first, edge.to, edge.weight);
          }
        }
      }
      return result;
    }
  };
}

#endif
