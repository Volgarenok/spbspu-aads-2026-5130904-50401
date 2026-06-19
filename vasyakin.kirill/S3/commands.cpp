#include "commands.hpp"
#include <utility>

namespace
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

  struct LessString
  {
    bool operator()(const std::string& a, const std::string& b) const
    {
      return a < b;
    }
  };

  struct LessOutputLine
  {
    bool operator()(const OutputLine& a, const OutputLine& b) const
    {
      return a.name < b.name;
    }
  };

  struct LessSizeT
  {
    bool operator()(size_t a, size_t b) const
    {
      return a < b;
    }
  };

  void output_int(std::ostream& out, const vasyakin::List< size_t >& list)
  {
    for (auto it = list.begin(); it != list.end(); ++it)
    {
      out << " " << *it;
    }
  }

  void copy_adjacency(vasyakin::Graph& dest, const vasyakin::Graph& src)
  {
    auto vertices = src.getAllVertices();
    for (const auto& v : vertices)
    {
      auto edges = src.getOutgoingEdges(v);

      for (const auto& e : edges)
      {
        dest.addEdge(v, e.first, e.second);
      }
    }
  }
}

namespace vasyakin
{
  void graphs(std::istream&, std::ostream& out, GraphsMap& graphs)
  {
    vasyakin::List< std::string > names;
    for (auto it = graphs.begin(); it != graphs.end(); ++it)
    {
      names.pushBack(it->first);
    }

    if (names.begin() == names.end())
    {
      out << '\n';
      return;
    }

    sort_list(names, LessString{});

    for (auto it = names.begin(); it != names.end(); ++it)
    {
      out << *it << '\n';
    }
  }

  void vertexes(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    in >> graph_name;

    if (!in || !graphs.contains(graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    const Graph& graph = graphs.at(graph_name);
    vasyakin::List< std::string > vertices;

    auto allVertices = graph.getAllVertices();
    for (const auto& vertex : allVertices)
    {
      vertices.pushBack(vertex);
      auto edges = graph.getOutgoingEdges(vertex);

      for (const auto& edge : edges)
      {
        vertices.pushBack(edge.first);
      }
    }

    if (vertices.begin() == vertices.end())
    {
      out << '\n';
      return;
    }

    sort_list(vertices, LessString{});

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

    if (!in || !graphs.contains(graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    const Graph& graph = graphs.at(graph_name);

    if (!graph.hasVertex(vertex_name) && graph.getIncomingEdges(vertex_name).isEmpty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    auto edges = graph.getOutgoingEdges(vertex_name);
    if (edges.isEmpty())
    {
      out << '\n';
      return;
    }

    vasyakin::List< OutputLine > lines;
    for (const auto& edge : edges)
    {
      bool found = false;

      for (auto lit = lines.begin(); lit != lines.end(); ++lit)
      {
        if (lit->name == edge.first)
        {
          lit->weights.pushBack(edge.second);
          found = true;
          break;
        }
      }
      if (!found)
      {
        OutputLine nl;
        nl.name = edge.first;

        nl.weights.pushBack(edge.second);
        lines.pushBack(nl);
      }
    }

    sort_list(lines, LessOutputLine{});
    for (auto lit = lines.begin(); lit != lines.end(); ++lit)
    {
      sort_list(lit->weights, LessSizeT{});
      out << lit->name;

      output_int(out, lit->weights);
      out << '\n';
    }
  }

  void inbound(std::istream& in, std::ostream& out, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;

    in >> graph_name >> vertex_name;

    if (!in || !graphs.contains(graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    const Graph& graph = graphs.at(graph_name);

    bool vertex_exists = graph.hasVertex(vertex_name);
    if (!vertex_exists)
    {
      if (!graph.getIncomingEdges(vertex_name).isEmpty())
      {
        vertex_exists = true;
      }
    }

    if (!vertex_exists)
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    vasyakin::List< OutputLine > lines;
    auto incoming = graph.getIncomingEdges(vertex_name);

    for (const auto& edge : incoming)
    {
      bool found = false;
      for (auto lit = lines.begin(); lit != lines.end(); ++lit)
      {
        if (lit->name == edge.first)
        {
          lit->weights.pushBack(edge.second);
          found = true;
          break;
        }
      }
      if (!found)
      {
        OutputLine nl;
        nl.name = edge.first;

        nl.weights.pushBack(edge.second);
        lines.pushBack(nl);
      }
    }

    sort_list(lines, LessOutputLine{});
    for (auto it = lines.begin(); it != lines.end(); ++it)
    {
      sort_list(it->weights, LessSizeT{});
      out << it->name;

      output_int(out, it->weights);
      out << '\n';
    }
  }

  void bind(std::istream& in, std::ostream&, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;
    std::string vertex_name_to;

    size_t w = 0;

    in >> graph_name >> vertex_name >> vertex_name_to >> w;

    if (!in || !graphs.contains(graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    Graph& graph = graphs.at(graph_name);

    Graph temp_graph = graph;

    temp_graph.addEdge(vertex_name, vertex_name_to, w);

    graph = std::move(temp_graph);
  }

  void cut(std::istream& in, std::ostream&, GraphsMap& graphs)
  {
    std::string graph_name;
    std::string vertex_name;
    std::string vertex_name_to;

    size_t w = 0;

    in >> graph_name >> vertex_name >> vertex_name_to >> w;

    if (!in || !graphs.contains(graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    Graph& graph = graphs.at(graph_name);

    bool vertex_from_exists = graph.hasVertex(vertex_name);
    if (!vertex_from_exists && graph.getIncomingEdges(vertex_name).isEmpty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    bool vertex_to_exists = graph.hasVertex(vertex_name_to);
    if (!vertex_to_exists && graph.getIncomingEdges(vertex_name_to).isEmpty())
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    if (!graph.hasEdge(vertex_name, vertex_name_to, w))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    graph.removeEdge(vertex_name, vertex_name_to, w);
  }

  void create(std::istream& in, std::ostream&, GraphsMap& graphs)
  {
    std::string graph_name;
    if (!(in >> graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    if (graphs.contains(graph_name))
    {
      throw std::invalid_argument("<INVALID COMMAND>");
    }

    Graph graph;
    size_t k = 0;

    if (in >> k)
    {
      for (size_t i = 0; i < k; ++i)
      {
        std::string vertex;

        if (!(in >> vertex))
        {
          throw std::runtime_error("<INVALID COMMAND>");
        }
        graph.addVertex(vertex);
      }
    }

    graphs.insert(std::make_pair(graph_name, std::move(graph)));
  }

  void merge(std::istream& in, std::ostream&, GraphsMap& graphs)
  {
    std::string new_graph_name;
    std::string graph_name1, graph_name2;

    in >> new_graph_name >> graph_name1 >> graph_name2;

    if (!in || graphs.contains(new_graph_name) ||
      !graphs.contains(graph_name1) || !graphs.contains(graph_name2))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    const Graph& graph1 = graphs.at(graph_name1);
    const Graph& graph2 = graphs.at(graph_name2);
    Graph graph;

    copy_adjacency(graph, graph1);
    copy_adjacency(graph, graph2);

    graphs.insert(std::make_pair(new_graph_name, graph));
  }

  void extract(std::istream& in, std::ostream&, GraphsMap& graphs)
  {
    std::string new_graph_name;
    std::string old_graph_name;

    size_t k = 0;

    in >> new_graph_name >> old_graph_name >> k;

    if (!in || graphs.contains(new_graph_name) || !graphs.contains(old_graph_name))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }

    const Graph& old_graph = graphs.at(old_graph_name);
    Graph new_graph;

    vasyakin::List< std::string > all_vertices;
    auto old_vertices = old_graph.getAllVertices();

    for (const auto& v : old_vertices)
    {
      bool exists = false;

      for (auto it = all_vertices.begin(); it != all_vertices.end(); ++it)
      {
        if (*it == v)
        {
          exists = true;
          break;
        }
      }
      if (!exists)
      {
        all_vertices.pushBack(v);
      }

      auto edges = old_graph.getOutgoingEdges(v);

      for (const auto& e : edges)
      {
        bool exists = false;

        for (auto it = all_vertices.begin(); it != all_vertices.end(); ++it)
        {
          if (*it == e.first)
          {
            exists = true;
            break;
          }
        }
        if (!exists)
        {
          all_vertices.pushBack(e.first);
        }
      }
    }

    vasyakin::List< std::string > selected_vertices;
    for (size_t i = 0; i < k; ++i)
    {
      std::string vertex_name;

      in >> vertex_name;

      if (!in)
      {
        throw std::runtime_error("<INVALID COMMAND>");
      }

      bool found = false;
      for (auto it = all_vertices.begin(); it != all_vertices.end(); ++it)
      {
        if (*it == vertex_name)
        {
          found = true;
          break;
        }
      }
      if (!found)
      {
        throw std::invalid_argument("<INVALID COMMAND>");
      }

      new_graph.addVertex(vertex_name);
      selected_vertices.pushBack(vertex_name);
    }

    for (const auto& src : selected_vertices)
    {
      auto edges = old_graph.getOutgoingEdges(src);
      for (const auto& e : edges)
      {
        bool target_selected = false;

        for (auto it = selected_vertices.begin(); it != selected_vertices.end(); ++it)
        {
          if (*it == e.first) { target_selected = true; break; }
        }
        if (target_selected)
        {
          new_graph.addEdge(src, e.first, e.second);
        }
      }
    }

    graphs.insert(std::make_pair(new_graph_name, std::move(new_graph)));
  }
}
