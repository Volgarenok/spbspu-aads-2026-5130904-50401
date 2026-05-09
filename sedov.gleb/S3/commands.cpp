#include "commands.hpp"
#include "../common/vector.hpp"
#include <limits>

namespace sedov
{
  namespace det
  {
    void sort_strings(sedov::Vector< std::string > & vec)
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

    void sort_weights(sedov::Vector< size_t > & vec)
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
}

void sedov::graphs(std::istream &, std::ostream & out, graphSet & graphs)
{
  sedov::Vector< std::string > names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    names.pushBack((*it).first);
  }
  det::sort_strings(names);
  if (names.getSize() == 0)
  {
    out << "\n";
  }
  else
  {
    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << "\n";
    }
  }
}

void sedov::vertexes(std::istream & in, std::ostream & out, graphSet & graphs)
{
  std::string g_name;
  in >> g_name;
  if (!graphs.contains(g_name))
  {
    throw std::runtime_error("No graph");
  }
  Graph & g = graphs.at(g_name);
  sedov::Vector< std::string > verts;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    verts.pushBack(*it);
  }
  det::sort_strings(verts);
  if (verts.getSize() == 0)
  {
    out << "\n";
  }
  else
  {
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      out << verts[i] << "\n";
    }
  }
}

void sedov::bind(std::istream & in, std::ostream &, graphSet & graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  Graph & g = graphs.at(g_name);
  g.addEdge(v1, v2, w);
}

void sedov::cut(std::istream & in, std::ostream &, graphSet & graphs)
{
  std::string g_name, v1, v2;
  size_t w = 0;
  in >> g_name >> v1 >> v2 >> w;
  Graph & g = graphs.at(g_name);
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
}

void sedov::create(std::istream & in, std::ostream &, graphSet & graphs)
{
  std::string g_name;
  in >> g_name;
  if (!in)
  {
    throw std::runtime_error("Invalid input");
  }
  if (graphs.contains(g_name))
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
  graphs.add(g_name, std::move(g));
}

void sedov::outbound(std::istream & in, std::ostream & out, graphSet & graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  Graph & g = graphs.at(g_name);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    if (*it == v_name)
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
  sedov::Vector< EdgeOut > edges_out;

  for (auto eit = g.edges.begin(); eit != g.edges.end(); ++eit)
  {
    if ((*eit).first.first == v_name)
    {
      bool exists = false;
      for (size_t i = 0; i < edges_out.getSize(); ++i)
      {
        if (edges_out[i].first == (*eit).first.second)
        {
          for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
          {
            edges_out[i].second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        sedov::Vector< size_t > ws;
        for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
        {
          ws.pushBack(*wit);
        }
        edges_out.pushBack({(*eit).first.second, std::move(ws)});
      }
    }
  }

  sedov::Vector< std::string > dests;
  for (size_t i = 0; i < edges_out.getSize(); ++i)
  {
    dests.pushBack(edges_out[i].first);
  }
  det::sort_strings(dests);

  if (dests.getSize() > 0)
  {
    for (size_t i = 0; i < dests.getSize(); ++i)
    {
      out << dests[i];
      for (size_t j = 0; j < edges_out.getSize(); ++j)
      {
        if (edges_out[j].first == dests[i])
        {
          det::sort_weights(edges_out[j].second);
          for (size_t k = 0; k < edges_out[j].second.getSize(); ++k)
          {
            out << " " << edges_out[j].second[k];
          }
          break;
        }
      }
      if (i < dests.getSize() - 1)
      {
        out << "\n";
      }
    }
    out << "\n";
  }
  else
  {
    out << "\n";
  }
}

void sedov::inbound(std::istream & in, std::ostream & out, graphSet & graphs)
{
  std::string g_name, v_name;
  in >> g_name >> v_name;
  Graph & g = graphs.at(g_name);
  bool found = false;
  for (LIter< std::string > it = g.vertices.begin(); it != g.vertices.end(); ++it)
  {
    if (*it == v_name)
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
  sedov::Vector< EdgeIn > edges_in;

  for (auto eit = g.edges.begin(); eit != g.edges.end(); ++eit)
  {
    if ((*eit).first.second == v_name)
    {
      bool exists = false;
      for (size_t i = 0; i < edges_in.getSize(); ++i)
      {
        if (edges_in[i].first == (*eit).first.first)
        {
          for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
          {
            edges_in[i].second.pushBack(*wit);
          }
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        sedov::Vector< size_t > ws;
        for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
        {
          ws.pushBack(*wit);
        }
        edges_in.pushBack({(*eit).first.first, std::move(ws)});
      }
    }
  }

  sedov::Vector< std::string > srcs;
  for (size_t i = 0; i < edges_in.getSize(); ++i)
  {
    srcs.pushBack(edges_in[i].first);
  }
  det::sort_strings(srcs);

  for (size_t i = 0; i < srcs.getSize(); ++i)
  {
    out << srcs[i];
    for (size_t j = 0; j < edges_in.getSize(); ++j)
    {
      if (edges_in[j].first == srcs[i])
      {
        det::sort_weights(edges_in[j].second);
        for (size_t k = 0; k < edges_in[j].second.getSize(); ++k)
        {
          out << " " << edges_in[j].second[k];
        }
        break;
      }
    }
    out << "\n";
  }
}

void sedov::merge(std::istream & in, std::ostream &, graphSet & graphs)
{
  std::string new_g, g1, g2;
  in >> new_g >> g1 >> g2;
  if (graphs.contains(new_g) || !graphs.contains(g1) || !graphs.contains(g2))
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
    for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
    {
      merged.addEdge((*eit).first.first, (*eit).first.second, *wit);
    }
  }
  for (auto eit = src2.edges.begin(); eit != src2.edges.end(); ++eit)
  {
    for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
    {
      merged.addEdge((*eit).first.first, (*eit).first.second, *wit);
    }
  }

  graphs.add(new_g, std::move(merged));
}

void sedov::extract(std::istream & in, std::ostream &, graphSet & graphs)
{
  std::string new_g, old_g;
  size_t k = 0;
  in >> new_g >> old_g >> k;
  if (graphs.contains(new_g) || !graphs.contains(old_g))
  {
    throw std::runtime_error("Invalid");
  }
  Graph & src = graphs.at(old_g);
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
    bool v1_ok = false;
    bool v2_ok = false;
    for (LIter< std::string > rit = required.begin(); rit != required.end(); ++rit)
    {
      if (*rit == (*eit).first.first)
      {
        v1_ok = true;
      }
      if (*rit == (*eit).first.second)
      {
        v2_ok = true;
      }
    }
    if (v1_ok && v2_ok)
    {
      for (LIter< size_t > wit = (*eit).second.begin(); wit != (*eit).second.end(); ++wit)
      {
        ext.addEdge((*eit).first.first, (*eit).first.second, *wit);
      }
    }
  }

  graphs.add(new_g, std::move(ext));
}
