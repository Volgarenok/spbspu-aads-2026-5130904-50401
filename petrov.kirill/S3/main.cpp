#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "list.hpp"
#include "HashTable.hpp"
#include "Graph.hpp"

using GraphTable = petrov::HashTable< std::string, petrov::Graph >;

class VertexMapper
{
public:
  int getVertId(const std::string &v)
  {
    auto it = strToInt_.find(v);
    if (it != strToInt_.end())
    {
      return it->second;
    }
    int id = nextVertexId_++;
    strToInt_.add(v, id);
    intToStr_.add(id, v);
    return id;
  }

  std::string getVertStr(int id) const
  {
    auto it = intToStr_.find(id);
    if (it != intToStr_.end())
    {
      return it->second;
    }
    return "";
  }

private:
  petrov::HashTable< std::string, int > strToInt_;
  petrov::HashTable< int, std::string > intToStr_;
  int nextVertexId_ = 1;
};

petrov::Graph *findGraph(GraphTable &graphs, const std::string &name)
{
  auto it = graphs.find(name);
  if (it != graphs.end())
  {
    return &it->second;
  }
  return nullptr;
}

void processGraphs(std::istream &, std::ostream &out, const GraphTable &graphs)
{
  if (graphs.begin() == graphs.end())
  {
    out << "\n";
    return;
  }
  std::vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.push_back(it->first);
  }
  std::sort(names.begin(), names.end());
  for (const auto &name : names)
  {
    out << name << "\n";
  }
}

void processVertexes(
  std::istream &in,
  std::ostream &out,
  GraphTable &graphs,
  const VertexMapper &mapper
)
{
  std::string gname = "";
  if (!(in >> gname))
  {
    throw std::runtime_error("");
  }
  petrov::Graph *g = findGraph(graphs, gname);
  if (!g)
  {
    throw std::runtime_error("");
  }
  petrov::List< int > verts = g->getAllVertices();
  if (verts.begin() == verts.end())
  {
    out << "\n";
    return;
  }
  std::vector< std::string > names;
  for (auto it = verts.begin(); it != verts.end(); ++it)
  {
    names.push_back(mapper.getVertStr(*it));
  }
  std::sort(names.begin(), names.end());
  for (const auto &name : names)
  {
    out << name << "\n";
  }
}

struct EdgeInfo
{
  std::string vertex;
  std::vector< int > weights;
};

bool compareEdgeInfo(const EdgeInfo &a, const EdgeInfo &b)
{
  return a.vertex < b.vertex;
}

void processOutbound(
  std::istream &in,
  std::ostream &out,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string gname = "";
  std::string vname = "";
  if (!(in >> gname >> vname))
  {
    throw std::runtime_error("");
  }
  petrov::Graph *g = findGraph(graphs, gname);
  if (!g)
  {
    throw std::runtime_error("");
  }

  int vid = mapper.getVertId(vname);
  if (!g->hasVertex(vid))
  {
    throw std::runtime_error("");
  }

  std::vector< EdgeInfo > res;
  const auto &edges = g->getEdges(vid);

  for (auto it = edges.begin(); it != edges.end(); ++it)
  {
    std::string toStr = mapper.getVertStr(it->to);
    bool found = false;
    for (auto &info : res)
    {
      if (info.vertex == toStr)
      {
        info.weights.push_back(it->weight);
        found = true;
        break;
      }
    }
    if (!found)
    {
      EdgeInfo info;
      info.vertex = toStr;
      info.weights.push_back(it->weight);
      res.push_back(info);
    }
  }

  if (res.empty())
  {
    out << "\n";
    return;
  }

  for (auto &info : res)
  {
    std::sort(info.weights.begin(), info.weights.end());
  }
  std::sort(res.begin(), res.end(), compareEdgeInfo);

  for (const auto &info : res)
  {
    out << info.vertex;
    for (int w : info.weights)
    {
      out << " " << w;
    }
    out << "\n";
  }
}

void processInbound(
  std::istream &in,
  std::ostream &out,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string gname = "";
  std::string vname = "";
  if (!(in >> gname >> vname))
  {
    throw std::runtime_error("");
  }
  petrov::Graph *g = findGraph(graphs, gname);
  if (!g)
  {
    throw std::runtime_error("");
  }

  int vid = mapper.getVertId(vname);
  if (!g->hasVertex(vid))
  {
    throw std::runtime_error("");
  }

  std::vector< EdgeInfo > res;
  petrov::List< int > vertices = g->getAllVertices();

  for (auto vit = vertices.begin(); vit != vertices.end(); ++vit)
  {
    int from = *vit;
    const auto &edges = g->getEdges(from);
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      if (eit->to == vid)
      {
        std::string fromStr = mapper.getVertStr(from);
        bool found = false;
        for (auto &info : res)
        {
          if (info.vertex == fromStr)
          {
            info.weights.push_back(eit->weight);
            found = true;
            break;
          }
        }
        if (!found)
        {
          EdgeInfo info;
          info.vertex = fromStr;
          info.weights.push_back(eit->weight);
          res.push_back(info);
        }
      }
    }
  }

  if (res.empty())
  {
    out << "\n";
    return;
  }

  for (auto &info : res)
  {
    std::sort(info.weights.begin(), info.weights.end());
  }
  std::sort(res.begin(), res.end(), compareEdgeInfo);

  for (const auto &info : res)
  {
    out << info.vertex;
    for (int w : info.weights)
    {
      out << " " << w;
    }
    out << "\n";
  }
}

void processBind(
  std::istream &in,
  std::ostream &,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string gname = "";
  std::string u_str = "";
  std::string v_str = "";
  int w = 0;
  if (!(in >> gname >> u_str >> v_str >> w))
  {
    throw std::runtime_error("");
  }
  petrov::Graph *g = findGraph(graphs, gname);
  if (!g)
  {
    throw std::runtime_error("");
  }

  int u = mapper.getVertId(u_str);
  int v = mapper.getVertId(v_str);
  g->addVertex(u);
  g->addVertex(v);
  g->addWeightedEdge(u, v, w);
}

void processCut(
  std::istream &in,
  std::ostream &,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string gname = "";
  std::string u_str = "";
  std::string v_str = "";
  int w = 0;
  if (!(in >> gname >> u_str >> v_str >> w))
  {
    throw std::runtime_error("");
  }
  petrov::Graph *g = findGraph(graphs, gname);
  if (!g)
  {
    throw std::runtime_error("");
  }

  int u = mapper.getVertId(u_str);
  int v = mapper.getVertId(v_str);

  if (!g->hasVertex(u) || !g->hasVertex(v))
  {
    throw std::runtime_error("");
  }

  g->cut(u, v, w);
}

void processCreate(
  std::istream &in,
  std::ostream &,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string gname = "";
  if (!(in >> gname))
  {
    throw std::runtime_error("");
  }
  if (graphs.find(gname) != graphs.end())
  {
    throw std::runtime_error("");
  }

  int vcount = 0;
  if (!(in >> vcount))
  {
    throw std::runtime_error("");
  }

  petrov::Graph g;
  for (int i = 0; i < vcount; ++i)
  {
    std::string v = "";
    if (!(in >> v))
    {
      throw std::runtime_error("");
    }
    g.addVertex(mapper.getVertId(v));
  }
  graphs.add(gname, g);
}

void processMerge(std::istream &in, std::ostream &, GraphTable &graphs)
{
  std::string res = "";
  std::string g1 = "";
  std::string g2 = "";
  if (!(in >> res >> g1 >> g2))
  {
    throw std::runtime_error("");
  }
  if (graphs.find(g1) == graphs.end() || graphs.find(g2) == graphs.end())
  {
    throw std::runtime_error("");
  }
  if (graphs.find(res) != graphs.end())
  {
    throw std::runtime_error("");
  }

  petrov::Graph *graph1 = findGraph(graphs, g1);
  petrov::Graph *graph2 = findGraph(graphs, g2);

  petrov::Graph result;

  petrov::List< int > v1 = graph1->getAllVertices();
  for (auto it = v1.begin(); it != v1.end(); ++it)
  {
    result.addVertex(*it);
    const auto &edges = graph1->getEdges(*it);
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      result.addVertex(eit->to);
      result.addWeightedEdge(*it, eit->to, eit->weight);
    }
  }

  petrov::List< int > v2 = graph2->getAllVertices();
  for (auto it = v2.begin(); it != v2.end(); ++it)
  {
    result.addVertex(*it);
    const auto &edges = graph2->getEdges(*it);
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      result.addVertex(eit->to);
      result.addWeightedEdge(*it, eit->to, eit->weight);
    }
  }

  graphs.add(res, result);
}

void processExtract(
  std::istream &in,
  std::ostream &,
  GraphTable &graphs,
  VertexMapper &mapper
)
{
  std::string res = "";
  std::string gname = "";
  if (!(in >> res >> gname))
  {
    throw std::runtime_error("");
  }
  if (graphs.find(gname) == graphs.end() || graphs.find(res) != graphs.end())
  {
    throw std::runtime_error("");
  }

  int vcount = 0;
  if (!(in >> vcount))
  {
    throw std::runtime_error("");
  }

  std::vector< int > targetVerts;
  for (int i = 0; i < vcount; ++i)
  {
    std::string v = "";
    if (!(in >> v))
    {
      throw std::runtime_error("");
    }
    targetVerts.push_back(mapper.getVertId(v));
  }

  petrov::Graph *src = findGraph(graphs, gname);
  for (int id : targetVerts)
  {
    if (!src->hasVertex(id))
    {
      throw std::runtime_error("");
    }
  }

  petrov::Graph result;
  for (int id : targetVerts)
  {
    result.addVertex(id);
  }

  for (int from : targetVerts)
  {
    const auto &edges = src->getEdges(from);
    for (auto eit = edges.begin(); eit != edges.end(); ++eit)
    {
      if (std::find(targetVerts.begin(), targetVerts.end(), eit->to) != targetVerts.end())
      {
        result.addVertex(eit->to);
        result.addWeightedEdge(from, eit->to, eit->weight);
      }
    }
  }

  graphs.add(res, result);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    std::cerr << "1 argument expected\n";
    return 1;
  }
  std::ifstream ifs(argv[1]);
  if (!ifs.is_open())
  {
    return 1;
  }

  GraphTable graphs;
  VertexMapper mapper;
  std::string gname = "";
  int ecount = 0;

  while (ifs >> gname >> ecount)
  {
    petrov::Graph g;
    for (int i = 0; i < ecount; ++i)
    {
      std::string u_str = "";
      std::string v_str = "";
      int w = 0;
      if (!(ifs >> u_str >> v_str >> w))
      {
        return 1;
      }
      int u = mapper.getVertId(u_str);
      int v = mapper.getVertId(v_str);
      g.addVertex(u);
      g.addVertex(v);
      g.addWeightedEdge(u, v, w);
    }
    graphs.add(gname, g);
  }

  std::string cmd = "";
  while (std::cin >> cmd)
  {
    try
    {
      if (cmd == "graphs")
      {
        processGraphs(std::cin, std::cout, graphs);
      }
      else if (cmd == "vertexes")
      {
        processVertexes(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "outbound")
      {
        processOutbound(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "inbound")
      {
        processInbound(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "bind")
      {
        processBind(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "cut")
      {
        processCut(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "create")
      {
        processCreate(std::cin, std::cout, graphs, mapper);
      }
      else if (cmd == "merge")
      {
        processMerge(std::cin, std::cout, graphs);
      }
      else if (cmd == "extract")
      {
        processExtract(std::cin, std::cout, graphs, mapper);
      }
      else
      {
        throw std::runtime_error("");
      }
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}
