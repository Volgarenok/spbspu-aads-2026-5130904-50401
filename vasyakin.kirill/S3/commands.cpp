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

  static auto p = [](const std::string& a, const std::string& b)
  {
    return a < b;
  };

  static auto f = [](const OutputLine& a, const OutputLine& b)
  {
    return a.name < b.name;
  };

  static auto g = [](size_t a, size_t b)
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

  void graphs(std::istream&, std::ostream& out, GraphsMap& graphs)
  {
    vasyakin::List< std::string > names;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      names.push_back(it->first);
    }

    if (names.begin() == names.end())
    {
      out << '\n';
      return;
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

    if (vertices.begin() == vertices.end())
    {
      out << '\n';
      return;
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

  void inbound(std::istream& in, std::ostream& out, GraphsMap& graphs)
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
    bool vertex_exists = graph.adj.has(vertex_name);
    if (!vertex_exists)
    {
      for (auto it = graph.adj.begin(); it != graph.adj.end() && !vertex_exists; ++it)
      {
        for (auto eit = it->second.begin(); eit != it->second.end(); ++eit)
        {
          if (eit->to == vertex_name)
          {
            vertex_exists = true;
            break;
          }
        }
      }
    }
    if (!vertex_exists)
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    vasyakin::List< OutputLine > lines;

    for (auto it = graph.adj.begin(); it != graph.adj.end(); ++it)
    {
      const std::string& source = it->first;
      const auto& edges = it->second;

      for (auto eit = edges.begin(); eit != edges.end(); ++eit)
      {
        if (eit->to == vertex_name)
        {
          bool found = false;
          for (auto lit = lines.begin(); lit != lines.end(); ++lit)
          {
            if (source == lit->name)
            {
              lit->weights.push_back(eit->weight);
              found = true;
              break;
            }
          }
          if (!found)
          {
            OutputLine nl;
            nl.name = source;
            nl.weights.push_back(eit->weight);
            lines.push_back(nl);
          }
        }
      }
    }

    sort_list(lines, f);

    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
      sort_list(it->weights, g);
      out << it->name;
      output_int(out, it->weights);
    }
  }

  void bind(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;
    std::string vertex_name_to;
    size_t w = 0;
    in >> graph_name >> vertex_name >> vertex_name_to >> w;

    if (!graphs.has(graph_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    Graph& graph = graphs.get(graph_name);

    if (!graph.adj.has(vertex_name_to))
    {
      graph.adj.add(vertex_name_to, vasyakin::List< Edge >{});
    }

    if (graph.adj.has(vertex_name))
    {
      graph.adj.get(vertex_name).push_back({vertex_name_to, w});
    }
    else
    {
      vasyakin::List< Edge > edges;
      edges.push_back({vertex_name_to, w});
      graph.adj.add(vertex_name, edges);
    }
  }

  void cut(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;
    std::string vertex_name_to;
    size_t w = 0;
    in >> graph_name >> vertex_name >> vertex_name_to >> w;

    if (!graphs.has(graph_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    Graph& graph = graphs.get(graph_name);

    bool vertex_from_exists = graph.adj.has(vertex_name);
    if (!vertex_from_exists)
    {
      for (auto it = graph.adj.begin(); it != graph.adj.end(); ++it)
      {
        for (auto eit = it->second.begin(); eit != it->second.end(); ++eit)
        {
          if (eit->to == vertex_name)
          {
            vertex_from_exists = true;
            break;
          }
        }
        if (vertex_from_exists) break;
      }
    }

    bool vertex_to_exists = graph.adj.has(vertex_name_to);
    if (!vertex_to_exists)
    {
      for (auto it = graph.adj.begin(); it != graph.adj.end(); ++it)
      {
        for (auto eit = it->second.begin(); eit != it->second.end(); ++eit)
        {
          if (eit->to == vertex_name_to)
          {
            vertex_to_exists = true;
            break;
          }
        }
        if (vertex_to_exists)
        {
          break;
        }
      }
    }

    if (!vertex_from_exists || !vertex_to_exists)
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    if (!graph.adj.has(vertex_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    auto& edges = graph.adj.get(vertex_name);
    auto p = [&vertex_name_to, &w](const Edge& e)
    {
      return e.to == vertex_name_to && e.weight == w;
    };

    if (edges.erase_if(p))
    {
      return;
    }
    out << "<INVALID COMMAND>" << '\n';
  }

  void create(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    if (!(in >> graph_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (graphs.has(graph_name))
    {
      out << "<INVALID COMMAND>\n";
      return;
    }

    size_t k = 0;
    char next_char = in.peek();
    if (next_char != '\n' && next_char != EOF)
    {
      if (!(in >> k))
      {
        out << "<INVALID COMMAND>\n";
        return;
      }

      Graph graph;
      for (size_t i = 0; i < k; ++i)
      {
        std::string vertex;
        if (!(in >> vertex))
        {
          out << "<INVALID COMMAND>\n";
          return;
        }
        graph.addVertex(vertex);
      }
      graphs.add(graph_name, std::move(graph));
    }
    else
    {
      graphs.add(graph_name, Graph{});
    }
  }

  void merge(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string new_graph_name;
    std::string graph_name1, graph_name2;
    in >> new_graph_name >> graph_name1 >> graph_name2;

    if (graphs.has(new_graph_name) || !graphs.has(graph_name1) || !graphs.has(graph_name2))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    const Graph& graph1 = graphs.get(graph_name1);
    const Graph& graph2 = graphs.get(graph_name2);
    Graph graph;

    auto copy_data = [&](const Graph& src)
    {
      for (auto it = src.adj.begin(); it != src.adj.end(); ++it)
      {
        const std::string& vertex = it->first;
        const auto& src_edges = it->second;

        if (!graph.adj.has(vertex))
        {
          graph.adj.add(vertex, vasyakin::List< Edge >{});
        }

        auto& edges = graph.adj.get(vertex);
        for (auto it = src_edges.begin(); it != src_edges.end(); ++it)
        {
          edges.push_back(*it);
        }
      }
    };

    copy_data(graph1);
    copy_data(graph2);

    graphs.add(new_graph_name, graph);
  }

  void extract(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string new_graph_name;
    std::string old_graph_name;
    size_t k = 0;
    in >> new_graph_name >> old_graph_name >> k;

    if (graphs.has(new_graph_name) || !graphs.has(old_graph_name))
    {
      out << "<INVALID COMMAND>" << '\n';
      return;
    }

    const Graph& old_graph = graphs.get(old_graph_name);
    Graph new_graph;

    vasyakin::List< std::string > all_vertices;
    for (auto it = old_graph.adj.begin(); it != old_graph.adj.end(); ++it)
    {
      all_vertices.push_back(it->first);
      for (auto eit = it->second.begin(); eit != it->second.end(); ++eit)
      {
        all_vertices.push_back(eit->to);
      }
    }

    for (size_t i = 0; i < k; ++i)
    {
      std::string vertex_name;
      in >> vertex_name;

      if (!in || !old_graph.adj.has(vertex_name))
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }

      bool found = false;
      for (auto vit = all_vertices.begin(); vit != all_vertices.end(); ++vit)
      {
        if (*vit == vertex_name)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        out << "<INVALID COMMAND>" << '\n';
        return;
      }

      new_graph.addVertex(vertex_name);
    }

    for (auto it = old_graph.adj.begin(); it != old_graph.adj.end(); ++it)
    {
      const std::string& vertex_name = it->first;

      if (!new_graph.adj.has(vertex_name))
      {
        continue;
      }

      const vasyakin::List< Edge >& edges = it->second;

      for (auto it = edges.begin(); it != edges.end(); ++it)
      {
        if (new_graph.adj.has(it->to))
        {
          new_graph.adj.get(vertex_name).push_back(*it);
        }
      }
    }
    graphs.add(new_graph_name, std::move(new_graph));
  }
}
