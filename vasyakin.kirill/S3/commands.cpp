#include "commands.hpp"
#include <utility>

namespace vasyakin
{
  template< class T, class Cmp >
  void sort_list(vasyakin::List< T >& list, Cmp cmp)
  {
    for (auto i = list.begin(); i != list.end(); ++i)
    {
      for (auto j = list.begin(); j != list.end(); ++j)
      {
        if (cmp(*i, *j))
        {
          std::swap(*i, *j);
        }
      }
    }
  }

  struct OutputLine
  {
    std::string name;
    vasyakin::List< size_t > weights;
  };

  static inline auto p = [](const std::string& a, const std::string& b)
  {
    return a < b;
  };

  static inline auto f = [](const OutputLine& a, const OutputLine& b)
  {
    return a.name < b.name;
  };

  static inline auto g = [](size_t a, size_t b)
  {
    return a < b;
  };

  void output_int(std::ostream& out, const vasyakin::List< size_t >& list)
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      out << " " << *it;
    }
    out << '\n';
  }

  void graphs(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    vasyakin::List< std::string > names;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      names.push_back(it->first);
    }

    sort_list(names, p);

    for (auto it = names.begin(); it != names.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void vertexes(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    in >> graph_name;
    
    if (!graphs.has(graph_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    const Graph& graph = graphs.get(graph_name);
    vasyakin::List< std::string > vertices;

    for (auto it = graph.adj.begin(); it != graph.adj.end(); ++it)
    {
      vertices.push_back(it->first);
      const auto& edges = it->second;
      for (auto eit = edges.begin(); eit != edges.end(); ++eit)
      {
        vertices.push_back(eit->to);
      }
    }

    sort_list(vertices, p);

    std::string last;
    bool first = true;
    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
      if (first || *it != last)
      {
        out << *it << '\n';
        last = *it;
        first = false;
      }
    }
  }

  void outbound(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;
    in >> graph_name >> vertex_name;

    if (!graphs.has(graph_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    const Graph& graph = graphs.get(graph_name);
    if (!graph.adj.has(vertex_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    const auto& edges = graph.adj.get(vertex_name);
    vasyakin::List< OutputLine > lines;

    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      bool found = false;
      for (auto lit = lines.begin(); lit != lines.end(); ++lit)
      {
        if (lit->name == it->to)
        {
          lit->weights.push_back(it->weight);
          found = true;
          break;
        }
      }
      if (!found)
      {
        OutputLine nl;
        nl.name = it->to;
        nl.weights.push_back(it->weight);
        lines.push_back(nl);
      }
    }

    sort_list(lines, f);

    for (auto lit = lines.begin(); lit != lines.end(); ++lit)
    {
      sort_list(lit->weights, g);
      out << lit->name;
      output_int(out, lit->weights);
    }
  }

}
