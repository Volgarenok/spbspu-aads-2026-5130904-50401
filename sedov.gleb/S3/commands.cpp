#include "commands.hpp"
#include <vector.hpp>
#include <limits>

namespace
{
  void sortStrings(sedov::Vector< std::string > & vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i)
    {
      std::string key = vec[i];
      size_t j = i;
      while ((j > 0) && (vec[j - 1] > key))
      {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }

  void sortWeights(sedov::Vector< size_t > & vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i)
    {
      size_t key = vec[i];
      size_t j = i;
      while ((j > 0) && (vec[j - 1] > key))
      {
        vec[j] = vec[j - 1];
        --j;
      }
      vec[j] = key;
    }
  }
}

void sedov::graphs(std::istream &, std::ostream & out, bool & printed, graphSet & graphs)
{
  sedov::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack(it->first);
  }
  sortStrings(names);
  if (names.getSize())
  {
    out << names[0];
    for (size_t i = 1; i < names.getSize(); ++i)
    {
      out << "\n" << names[i];
    }
    printed = true;
  }
  else
  {
    out << "\n";
    printed = false;
  }
}

void sedov::vertexes(std::istream & in, std::ostream & out, bool & printed, graphSet & graphs)
{
  std::string gName;
  in >> gName;
  Graph & g = graphs.at(gName);
  sedov::Vector< std::string > verts;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    verts.pushBack(*it);
  }
  sortStrings(verts);
  if (verts.getSize())
  {
    out << verts[0];
    for (size_t i = 1; i < verts.getSize(); ++i)
    {
      out << "\n" << verts[i];
    }
    printed = true;
  }
  else
  {
    out << "\n";
    printed = false;
  }
}

void sedov::bind(std::istream & in, std::ostream &, bool & printed, graphSet & graphs)
{
  std::string gName, v1, v2;
  size_t w = 0;
  in >> gName >> v1 >> v2 >> w;
  Graph & g = graphs.at(gName);
  g.addEdge(v1, v2, w);
  printed = false;
}

void sedov::cut(std::istream & in, std::ostream &, bool & printed, graphSet & graphs)
{
  std::string gName, v1, v2;
  size_t w = 0;
  in >> gName >> v1 >> v2 >> w;
  Graph & g = graphs.at(gName);
  bool v1_exists = false;
  bool v2_exists = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    if (*it == v1)
    {
      v1_exists = true;
    }
    if (*it == v2)
    {
      v2_exists = true;
    }
  }
  if (!v1_exists || !v2_exists)
  {
    throw std::runtime_error("Vertex missing");
  }
  g.removeEdge(v1, v2, w);
  printed = false;
}

void sedov::create(std::istream & in, std::ostream &, bool & printed, graphSet & graphs)
{
  std::string gName;
  in >> gName;
  if (!in)
  {
    throw std::runtime_error("Invalid input");
  }
  if (graphs.contains(gName))
  {
    throw std::runtime_error("Graph already exists");
  }
  size_t k = 0;
  in >> k;
  if (!in)
  {
    throw std::runtime_error("Invalid vertex count");
  }
  Graph g;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    if (!in)
    {
      throw std::runtime_error("Invalid vertex name");
    }
    g.addVertex(v);
  }
  graphs.add(gName, std::move(g));
  printed = false;
}

void sedov::outbound(std::istream & in, std::ostream & out, bool & printed, graphSet & graphs)
{
  std::string gName, vName;
  in >> gName >> vName;
  Graph & g = graphs.at(gName);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    if (*it == vName)
    {
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Vertex missing");
  }

  using EdgeOut = std::pair< std::string, sedov::Vector< size_t > >;
  sedov::Vector< EdgeOut > edgesOut;

  for (auto eit = g.edges.begin(); eit != g.edges.end(); ++eit)
  {
    if (eit->first.first == vName)
    {
      bool exists = false;
      for (size_t i = 0; i < edgesOut.getSize(); ++i)
      {
        if (edgesOut[i].first == eit->first.second)
        {
          for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
          {
            edgesOut[i].second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        sedov::Vector< size_t > ws;
        for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
        {
          ws.pushBack(*wit);
        }
        edgesOut.pushBack({eit->first.second, std::move(ws)});
      }
    }
  }

  sedov::Vector< std::string > dests;
  for (size_t i = 0; i < edgesOut.getSize(); ++i)
  {
    dests.pushBack(edgesOut[i].first);
  }
  sortStrings(dests);

  if (dests.getSize() > 0)
  {
    for (size_t i = 0; i < dests.getSize(); ++i)
    {
      out << dests[i];
      for (size_t j = 0; j < edgesOut.getSize(); ++j)
      {
        if (edgesOut[j].first == dests[i])
        {
          sortWeights(edgesOut[j].second);
          for (size_t k = 0; k < edgesOut[j].second.getSize(); ++k)
          {
            out << " " << edgesOut[j].second[k];
          }
          break;
        }
      }
      if (i < dests.getSize() - 1)
      {
        out << "\n";
      }
    }
    printed = true;
  }
  else
  {
    printed = false;
  }
}

void sedov::inbound(std::istream & in, std::ostream & out, bool & printed, graphSet & graphs)
{
  std::string gName, vName;
  in >> gName >> vName;
  Graph & g = graphs.at(gName);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    if (*it == vName)
    {
      found = true;
      break;
    }
  }
  if (!found)
  {
    throw std::out_of_range("Vertex missing");
  }

  using EdgeIn = std::pair< std::string, sedov::Vector< size_t > >;
  sedov::Vector< EdgeIn > edgesIn;

  for (auto eit = g.edges.begin(); eit != g.edges.end(); ++eit)
  {
    if (eit->first.second == vName)
    {
      bool exists = false;
      for (size_t i = 0; i < edgesIn.getSize(); ++i)
      {
        if (edgesIn[i].first == eit->first.first)
        {
          for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
          {
            edgesIn[i].second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        sedov::Vector< size_t > ws;
        for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
        {
          ws.pushBack(*wit);
        }
        edgesIn.pushBack({eit->first.first, std::move(ws)});
      }
    }
  }

  sedov::Vector< std::string > srcs;
  for (size_t i = 0; i < edgesIn.getSize(); ++i)
  {
    srcs.pushBack(edgesIn[i].first);
  }
  sortStrings(srcs);

  if (srcs.getSize())
  {
    for (size_t i = 0; i < srcs.getSize(); ++i)
    {
      out << srcs[i];
      for (size_t j = 0; j < edgesIn.getSize(); ++j)
      {
        if (edgesIn[j].first == srcs[i])
        {
          sortWeights(edgesIn[j].second);
          for (size_t k = 0; k < edgesIn[j].second.getSize(); ++k)
          {
            out << " " << edgesIn[j].second[k];
          }
          break;
        }
      }
      if (i < srcs.getSize() - 1)
      {
        out << "\n";
      }
    }
    printed = true;
  }
  else
  {
    printed = false;
  }
}

void sedov::merge(std::istream & in, std::ostream &, bool & printed, graphSet & graphs)
{
  std::string newG, g1, g2;
  in >> newG >> g1 >> g2;
  if (graphs.contains(newG))
  {
    throw std::runtime_error("Invalid");
  }
  Graph merged(16);
  Graph & src1 = graphs.at(g1);
  Graph & src2 = graphs.at(g2);

  for (LIter< std::string > it = src1.vertices.begin(); it != src1.vertices.end(); ++it)
  {
    merged.addVertex(*it);
  }
  for (LIter< std::string > it = src2.vertices.begin(); it != src2.vertices.end(); ++it)
  {
    merged.addVertex(*it);
  }

  for (auto eit = src1.edges.begin(); eit != src1.edges.end(); ++eit)
  {
    for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
    {
      merged.addEdge(eit->first.first, eit->first.second, *wit);
    }
  }
  for (auto eit = src2.edges.begin(); eit != src2.edges.end(); ++eit)
  {
    for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
    {
      merged.addEdge(eit->first.first, eit->first.second, *wit);
    }
  }

  graphs.add(newG, std::move(merged));
  printed = false;
}

void sedov::extract(std::istream & in, std::ostream &, bool & printed, graphSet & graphs)
{
  std::string newG, oldG;
  size_t k = 0;
  in >> newG >> oldG >> k;
  if (graphs.contains(newG))
  {
    throw std::runtime_error("Invalid");
  }
  Graph & src = graphs.at(oldG);
  sedov::List< std::string > required;
  for (size_t i = 0; i < k; ++i)
  {
    std::string v;
    in >> v;
    required.pushBack(v);
  }

  for (LIter< std::string > it = required.begin(); it != required.end(); ++it)
  {
    bool found = false;
    for (LIter< std::string > vit = src.vertices.begin(); vit != src.vertices.end(); ++vit)
    {
      if (*vit == *it)
      {
        found = true;
        break;
      }
    }
    if (!found)
    {
      throw std::out_of_range("Vertex missing");
    }
  }

  Graph ext(16);
  for (LIter< std::string > it = required.begin(); it != required.end(); ++it)
  {
    ext.addVertex(*it);
  }

  for (auto eit = src.edges.begin(); eit != src.edges.end(); ++eit)
  {
    bool v1Ok = false;
    bool v2Ok = false;
    for (LIter< std::string > rit = required.begin(); rit != required.end(); ++rit)
    {
      if (*rit == eit->first.first)
      {
        v1Ok = true;
      }
      if (*rit == eit->first.second)
      {
        v2Ok = true;
      }
    }
    if (v1Ok && v2Ok)
    {
      for (LIter< size_t > wit = eit->second.begin(); wit != eit->second.end(); ++wit)
      {
        ext.addEdge(eit->first.first, eit->first.second, *wit);
      }
    }
  }

  graphs.add(newG, std::move(ext));
  printed = false;
}
