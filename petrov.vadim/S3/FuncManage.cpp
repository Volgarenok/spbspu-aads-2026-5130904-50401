#include "FuncManage.hpp"
#include "Grath.hpp"

namespace petrov
{
  void sort(topit::Vector< std::string >& vec)
  {
    for (size_t i = 1; i < vec.getSize(); ++i)
    {
      std::string key = vec[i];
      size_t j = i;
      while (j > 0 && vec[j-1] > key)
      {
        vec[j] = vec[j-1];
        --j;
      }
      vec[j] = key;
    }
  }

  void FuncManage::graphs(std::ostream& out, std::istream&, std::string)
  {
    topit::Vector< std::string > names;

    HIter< std::string, Grath > it = graphs_.begin();
    while (it != graphs_.end())
    {
      names.pushBack(it->key_);
      ++it;
    }

    if (names.isEmpty())
    {
      out << "\n";
      return;
    }

    sort(names);

    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << "\n";
    }
  }

  void FuncManage::vertexes(std::ostream& out, std::istream&, std::string str)
  {
    if (!graphs_.has(str))
    {
      throw std::runtime_error("Graph not found\n");
    }

    topit::Vector< std::string > names = graphs_.get(str).getVertices();

    if (names.isEmpty())
    {
      out << "\n";
      return;
    }

    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << "\n";
    }
  }

  void FuncManage::outbound(std::ostream& out, std::istream& in, std::string str)
  {
    std::string vert;
    if (!(in >> vert) || !graphs_.has(str) || !graphs_.get(str).hasVertex(vert))
    {
      throw std::runtime_error("command invalid");
    }

    topit::Vector< EdgeVec > edges = graphs_.get(str).getOutputEdges(vert);

    if (edges.isEmpty())
    {
      out << "\n";
      return;
    }

    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      out << edges[i].first;
      for (size_t j = 0; j < edges[i].second.getSize(); ++j)
      {
        out  << " " << edges[i].second[j];
      }
      out << "\n";
    }
  }

  void FuncManage::inbound(std::ostream& out, std::istream& in, std::string str)
  {
    std::string vert;
    if (!(in >> vert) || !graphs_.has(str) || !graphs_.get(str).hasVertex(vert))
    {
      throw std::runtime_error("command invalid");
    }

    topit::Vector< EdgeVec > edges = graphs_.get(str).getInputEdges(vert);

    if (edges.isEmpty())
    {
      out << "\n";
      return;
    }

    for (size_t i = 0; i < edges.getSize(); ++i)
    {
      out << edges[i].first;
      for (size_t j = 0; j < edges[i].second.getSize(); ++j)
      {
        out  << " " << edges[i].second[j];
      }
      out << "\n";
    }
  }

  void FuncManage::bind(std::ostream&, std::istream& in, std::string str)
  {
    std::string from, to;
    size_t w;
    if (!(in >> from >> to >> w) || !graphs_.has(str))
    {
      throw std::runtime_error("command invalid");
    }

    graphs_.get(str).addEdge(from, to, w);
  }

  bool findEdgeW(const Grath& g, const std::string& from, const std::string& to, size_t w)
  {
    if (!g.hasEdge(from, to))
    {
      return false;
    }

    topit::Vector< EdgeVec > v = g.getOutputEdges(from);
    for (size_t i = 0; i < v.getSize(); ++i)
    {
      if (v[i].first == to)
      {
        const Weight& ww = v[i].second;
        for (size_t j = 0; j < ww.getSize(); ++j)
        {
          if (ww[j] == w)
          {
            return true;
          }
        }
      }
    }
    return false;
  }

  void FuncManage::cut(std::ostream&, std::istream& in, std::string str)
  {
    std::string from, to;
    size_t w;
    if (!(in >> from >> to >> w) ||
        !graphs_.has(str) ||
        !graphs_.get(str).hasVertex(from) ||
        !graphs_.get(str).hasVertex(to) ||
        !findEdgeW(graphs_.get(str), from, to, w))
    {
      throw std::runtime_error("command invalid");
    }

    graphs_.get(str).removeEdge(from, to, w);
  }

  void FuncManage::create(std::ostream&, std::istream& in, std::string str)
  {
    if (graphs_.has(str))
    {
      throw std::runtime_error("Graph already exists");
    }

    Grath g;
    size_t count = 0;

    if (in >> count)
    {
      for (size_t i = 0; i < count; ++i)
      {
        std::string v;
        if (!(in >> v))
        {
          throw std::runtime_error("Missing vertex name");
        }
        g.addVertex(v);
      }
    }
    else
    {
      in.clear();
    }

    graphs_.add(str, std::move(g));
  }

  void FuncManage::merge(std::ostream&, std::istream& in, std::string str)
  {
    std::string g1, g2;
    if (!(in >> g1 >> g2) || graphs_.has(str) || !graphs_.has(g1) || !graphs_.has(g2))
    {
      throw std::runtime_error("command invalid");
    }

    Grath result = graphs_.get(g1);
    const Grath& from = graphs_.get(g2);

    auto verts = from.getVertices();
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      result.addVertex(verts[i]);
    }

    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      topit::Vector< petrov::EdgeVec > edges = from.getOutputEdges(verts[i]);
      for (size_t j = 0; j < edges.getSize(); ++j)
      {
        for (size_t k = 0; k < edges[j].second.getSize(); ++k)
        {
          result.addEdge(verts[i], edges[j].first, edges[j].second[k]);
        }
      }
    }

    graphs_.add(str, std::move(result));
  }

  void FuncManage::extract(std::ostream&, std::istream& in, std::string str)
  {
    std::string from;
    size_t count;
    if (!(in >> from >> count) || !graphs_.has(from))
    {
      throw std::runtime_error("command invalid");
    }

    topit::Vector<std::string> verts;
    for (size_t i = 0; i < count; ++i)
    {
      std::string v;
      if (!(in >> v) || !graphs_.get(from).hasVertex(v))
      {
        throw std::runtime_error("Invalid args");
      }
      verts.pushBack(v);
    }

    Grath result;
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      result.addVertex(verts[i]);
    }

    const Grath& src = graphs_.get(from);
    for (size_t i = 0; i < verts.getSize(); ++i)
    {
      topit::Vector< petrov::EdgeVec > edges = src.getOutputEdges(verts[i]);
      for (size_t j = 0; j < edges.getSize(); ++j)
      {
        bool target = false;
        for (size_t k = 0; k < verts.getSize(); ++k)
        {
          if (edges[j].first == verts[k])
          {
            target = true;
            break;
          }
        }

        if (target)
        {
          for (size_t k = 0; k < edges[j].second.getSize(); ++k)
          {
            result.addEdge(verts[i], edges[j].first, edges[j].second[k]);
          }
        }
      }
    }

    graphs_.add(str, std::move(result));
  }

  void FuncManage::readFile(std::istream & in)
  {
    std::string graphName;
    size_t edgesCount = 0;

    while (in >> graphName >> edgesCount)
    {
      Grath graph;

      for (size_t i = 0; i < edgesCount; ++i)
      {
        std::string from;
        std::string to;
        size_t weight = 0;

        if (!(in >> from >> to >> weight))
        {
          throw std::logic_error("invalid file");
        }

        graph.addEdge(from, to, weight);
      }

      graphs_.add(graphName, graph);
    }
  }
}
