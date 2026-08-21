#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "../common/list.hpp"
#include "../common/HashTable.hpp"
#include "Graph.hpp"

using GraphTable = petrov::HashTable<std::string, petrov::Graph>;
using AdjListType = petrov::HashTable<int, petrov::List<petrov::Edge>>;

AdjListType& getAdj(petrov::Graph* g) {
  return *reinterpret_cast<AdjListType*>(g);
}

petrov::HashTable<std::string, int> strToInt;
petrov::HashTable<int, std::string> intToStr;
int nextVertexId = 1;

int getVertId(const std::string& v) {
  for (auto it = strToInt.begin(); it != strToInt.end(); ++it) {
    if (it->first == v) {
      return it->second;
    }
  }
  strToInt.add(v, nextVertexId);
  intToStr.add(nextVertexId, v);
  return nextVertexId++;
}

std::string getVertStr(int id) {
  for (auto it = intToStr.begin(); it != intToStr.end(); ++it) {
    if (it->first == id) {
      return it->second;
    }
  }
  return "";
}

petrov::Graph* findGraph(GraphTable& graphs, const std::string& name) {
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    if (it->first == name) {
      return &it->second;
    }
  }
  return nullptr;
}

void addEdgeUnconditionally(petrov::Graph* g, int u, int v, int w) {
  g->addVertex(u);
  g->addVertex(v);
  auto& adjList = getAdj(g);
  for (auto it = adjList.begin(); it != adjList.end(); ++it) {
    if (it->first == u) {
      it->second.push_back({v, w});
      break;
    }
  }
}

void processGraphs(std::istream&, std::ostream& out, GraphTable& graphs) {
  if (graphs.isEmpty()) {
    out << "\n";
    return;
  }
  std::vector<std::string> names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it) {
    names.push_back(it->first);
  }
  std::sort(names.begin(), names.end());
  for (const auto& name : names) {
    out << name << "\n";
  }
}

void processVertexes(std::istream& in, std::ostream& out, GraphTable& graphs) {
  std::string gname;
  if (!(in >> gname)) {
    throw std::runtime_error("");
  }
  petrov::Graph* g = findGraph(graphs, gname);
  if (!g) {
    throw std::runtime_error("");
  }
  petrov::List<int> verts = g->getAllVertices();
  if (verts.getSize() == 0) {
    out << "\n";
    return;
  }
  std::vector<std::string> names;
  for (auto it = verts.begin(); it != verts.end(); ++it) {
    names.push_back(getVertStr(*it));
  }
  std::sort(names.begin(), names.end());
  for (const auto& name : names) {
    out << name << "\n";
  }
}

struct EdgeInfo {
  std::string vertex;
  std::vector<int> weights;
};

bool compareEdgeInfo(const EdgeInfo& a, const EdgeInfo& b) {
  return a.vertex < b.vertex;
}

void processOutbound(std::istream& in, std::ostream& out, GraphTable& graphs) {
  std::string gname, vname;
  if (!(in >> gname >> vname)) {
    throw std::runtime_error("");
  }
  petrov::Graph* g = findGraph(graphs, gname);
  if (!g) {
    throw std::runtime_error("");
  }

  int vid = getVertId(vname);
  if (!g->hasVertex(vid)) {
    throw std::runtime_error("");
  }

  auto& adjList = getAdj(g);
  std::vector<EdgeInfo> res;

  for (auto it = adjList.begin(); it != adjList.end(); ++it) {
    if (it->first == vid) {
      for (const auto& edge : it->second) {
        std::string toStr = getVertStr(edge.to);
        bool found = false;
        for (auto& info : res) {
          if (info.vertex == toStr) {
            info.weights.push_back(edge.weight);
            found = true;
            break;
          }
        }
        if (!found) {
          EdgeInfo info;
          info.vertex = toStr;
          info.weights.push_back(edge.weight);
          res.push_back(info);
        }
      }
      break;
    }
  }

  if (res.empty()) {
    out << "\n";
    return;
  }

  for (auto& info : res) {
    std::sort(info.weights.begin(), info.weights.end());
  }
  std::sort(res.begin(), res.end(), compareEdgeInfo);

  for (const auto& info : res) {
    out << info.vertex;
    for (int w : info.weights) {
      out << " " << w;
    }
    out << "\n";
  }
}

void processInbound(std::istream& in, std::ostream& out, GraphTable& graphs) {
  std::string gname, vname;
  if (!(in >> gname >> vname)) {
    throw std::runtime_error("");
  }
  petrov::Graph* g = findGraph(graphs, gname);
  if (!g) {
    throw std::runtime_error("");
  }

  int vid = getVertId(vname);
  if (!g->hasVertex(vid)) {
    throw std::runtime_error("");
  }

  auto& adjList = getAdj(g);
  std::vector<EdgeInfo> res;

  for (auto it = adjList.begin(); it != adjList.end(); ++it) {
    int from = it->first;
    for (const auto& edge : it->second) {
      if (edge.to == vid) {
        std::string fromStr = getVertStr(from);
        bool found = false;
        for (auto& info : res) {
          if (info.vertex == fromStr) {
            info.weights.push_back(edge.weight);
            found = true;
            break;
          }
        }
        if (!found) {
          EdgeInfo info;
          info.vertex = fromStr;
          info.weights.push_back(edge.weight);
          res.push_back(info);
        }
      }
    }
  }

  if (res.empty()) {
    out << "\n";
    return;
  }

  for (auto& info : res) {
    std::sort(info.weights.begin(), info.weights.end());
  }
  std::sort(res.begin(), res.end(), compareEdgeInfo);

  for (const auto& info : res) {
    out << info.vertex;
    for (int w : info.weights) {
      out << " " << w;
    }
    out << "\n";
  }
}

void processBind(std::istream& in, std::ostream&, GraphTable& graphs) {
  std::string gname, u_str, v_str;
  int w;
  if (!(in >> gname >> u_str >> v_str >> w)) {
    throw std::runtime_error("");
  }
  petrov::Graph* g = findGraph(graphs, gname);
  if (!g) {
    throw std::runtime_error("");
  }

  int u = getVertId(u_str);
  int v = getVertId(v_str);
  addEdgeUnconditionally(g, u, v, w);
}

void processCut(std::istream& in, std::ostream&, GraphTable& graphs) {
  std::string gname, u_str, v_str;
  int w;
  if (!(in >> gname >> u_str >> v_str >> w)) {
    throw std::runtime_error("");
  }
  petrov::Graph* g = findGraph(graphs, gname);
  if (!g) {
    throw std::runtime_error("");
  }

  int u = getVertId(u_str);
  int v = getVertId(v_str);

  if (!g->hasVertex(u) || !g->hasVertex(v)) {
    throw std::runtime_error("");
  }

  bool found = false;
  auto& adjList = getAdj(g);
  for (auto it = adjList.begin(); it != adjList.end(); ++it) {
    if (it->first == u) {
      auto& neighbors = it->second;
      for (auto nit = neighbors.begin(); nit != neighbors.end(); ++nit) {
        if (nit->to == v && nit->weight == w) {
          neighbors.erase(nit);
          found = true;
          break;
        }
      }
      break;
    }
  }

  if (!found) {
    throw std::runtime_error("");
  }
}

void processCreate(std::istream& in, std::ostream&, GraphTable& graphs) {
  std::string gname;
  if (!(in >> gname)) {
    throw std::runtime_error("");
  }
  if (graphs.has(gname)) {
    throw std::runtime_error("");
  }

  int vcount;
  if (!(in >> vcount)) {
    throw std::runtime_error("");
  }

  petrov::Graph g;
  for (int i = 0; i < vcount; ++i) {
    std::string v;
    if (!(in >> v)) {
      throw std::runtime_error("");
    }
    g.addVertex(getVertId(v));
  }
  graphs.add(gname, g);
}

void processMerge(std::istream& in, std::ostream&, GraphTable& graphs) {
  std::string res, g1, g2;
  if (!(in >> res >> g1 >> g2)) {
    throw std::runtime_error("");
  }
  if (!graphs.has(g1) || !graphs.has(g2) || graphs.has(res)) {
    throw std::runtime_error("");
  }

  petrov::Graph* graph1 = findGraph(graphs, g1);
  petrov::Graph* graph2 = findGraph(graphs, g2);

  petrov::Graph result;

  petrov::List<int> v1 = graph1->getAllVertices();
  for (auto it = v1.begin(); it != v1.end(); ++it) {
    result.addVertex(*it);
  }

  petrov::List<int> v2 = graph2->getAllVertices();
  for (auto it = v2.begin(); it != v2.end(); ++it) {
    result.addVertex(*it);
  }

  auto& src1AdjList = getAdj(graph1);
  for (auto it = src1AdjList.begin(); it != src1AdjList.end(); ++it) {
    for (const auto& edge : it->second) {
      addEdgeUnconditionally(&result, it->first, edge.to, edge.weight);
    }
  }

  auto& src2AdjList = getAdj(graph2);
  for (auto it = src2AdjList.begin(); it != src2AdjList.end(); ++it) {
    for (const auto& edge : it->second) {
      addEdgeUnconditionally(&result, it->first, edge.to, edge.weight);
    }
  }

  graphs.add(res, result);
}

void processExtract(std::istream& in, std::ostream&, GraphTable& graphs) {
  std::string res, gname;
  if (!(in >> res >> gname)) {
    throw std::runtime_error("");
  }
  if (!graphs.has(gname) || graphs.has(res)) {
    throw std::runtime_error("");
  }

  int vcount;
  if (!(in >> vcount)) {
    throw std::runtime_error("");
  }

  std::vector<int> targetVerts;
  for (int i = 0; i < vcount; ++i) {
    std::string v;
    if (!(in >> v)) {
      throw std::runtime_error("");
    }
    targetVerts.push_back(getVertId(v));
  }

  petrov::Graph* src = findGraph(graphs, gname);
  for (int id : targetVerts) {
    if (!src->hasVertex(id)) {
      throw std::runtime_error("");
    }
  }

  petrov::Graph result;
  for (int id : targetVerts) {
    result.addVertex(id);
  }

  auto& srcAdjList = getAdj(src);
  for (auto it = srcAdjList.begin(); it != srcAdjList.end(); ++it) {
    int from = it->first;
    if (std::find(targetVerts.begin(), targetVerts.end(), from) != targetVerts.end()) {
      for (const auto& edge : it->second) {
        if (std::find(targetVerts.begin(), targetVerts.end(), edge.to) != targetVerts.end()) {
          addEdgeUnconditionally(&result, from, edge.to, edge.weight);
        }
      }
    }
  }

  graphs.add(res, result);
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "1 argument expected\n";
    return 1;
  }
  std::ifstream ifs(argv[1]);
  if (!ifs.is_open()) {
    return 1;
  }

  GraphTable graphs;
  std::string gname;
  int ecount;

  while (ifs >> gname >> ecount) {
    petrov::Graph g;
    for (int i = 0; i < ecount; ++i) {
      std::string u_str, v_str;
      int w;
      if (!(ifs >> u_str >> v_str >> w)) {
        return 1;
      }
      int u = getVertId(u_str);
      int v = getVertId(v_str);
      addEdgeUnconditionally(&g, u, v, w);
    }
    graphs.add(gname, g);
  }

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      if (cmd == "graphs") {
        processGraphs(std::cin, std::cout, graphs);
      } else if (cmd == "vertexes") {
        processVertexes(std::cin, std::cout, graphs);
      } else if (cmd == "outbound") {
        processOutbound(std::cin, std::cout, graphs);
      } else if (cmd == "inbound") {
        processInbound(std::cin, std::cout, graphs);
      } else if (cmd == "bind") {
        processBind(std::cin, std::cout, graphs);
      } else if (cmd == "cut") {
        processCut(std::cin, std::cout, graphs);
      } else if (cmd == "create") {
        processCreate(std::cin, std::cout, graphs);
      } else if (cmd == "merge") {
        processMerge(std::cin, std::cout, graphs);
      } else if (cmd == "extract") {
        processExtract(std::cin, std::cout, graphs);
      } else {
        throw std::runtime_error("");
      }
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}
