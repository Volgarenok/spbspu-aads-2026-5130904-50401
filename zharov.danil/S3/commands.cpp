#include "commands.hpp"
#include <functional>

namespace
{
  template< class T, class Cmp >
  void sort(zharov::Vector< T >& v, Cmp cmp)
  {
    for (size_t i = 0; i < v.getSize(); ++i)
    {
      size_t min = i;
      for (size_t j = i + 1; j < v.getSize(); ++j)
      {
        if (cmp(v[j], v[min]))
        {
          min = j;
        }
      }
      if (min != i)
      {
        std::swap(v[i], v[min]);
      }
    }
  }
}

void zharov::graphs(std::ostream& out, std::istream&, const graphs_table& graphs)
{
  zharov::Vector< std::string > names;
  for (auto i = graphs.begin(); i != graphs.end(); ++i)
  {
    names.pushBack(i->first);
  }
  sort(names, std::less< std::string >{});
  std::string sep;
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << sep << *i;
    sep = "\n";
  }
}

void zharov::vertexes(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name;
  in >> gr_name;
  const zharov::Graph& gr = graphs.at(gr_name);
  zharov::Vector< std::string > names;
  for (auto i = gr.vertexes.cbegin(); i != gr.vertexes.cend(); ++i)
  {
    names.pushBack(*i);
  }

  sort(names, std::less< std::string >{});
  std::string sep;
  for (auto i = names.begin(); i != names.end(); ++i)
  {
    out << sep << *i;
    sep = "\n";
  }
}

void zharov::outbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  const zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = gr.edges.cbegin(); i != gr.edges.cend(); ++i)
  {
    if (i->first.first == vert_name)
    {
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
        names.pushBack(std::make_pair(i->first.second, *j));
      }
    }
  }
  sort(names, std::less< std::pair< std::string, size_t > >{});
  std::string sep;
  for (auto i = names.begin(); i != names.end();)
  {
    std::string temp = i->first;
    out << sep << i->first << " " << i->second;
    ++i;
    while (i != names.end() && i->first == temp)
    {
      out << " " << i->second;
      ++i;
    }
    sep = "\n";
  }
}

void zharov::inbound(std::ostream& out, std::istream& in, const graphs_table& graphs)
{
  std::string gr_name, vert_name;
  in >> gr_name >> vert_name;
  const zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes.has(vert_name))
  {
    throw std::logic_error("Vertex not found");
  }

  zharov::Vector< std::pair< std::string, size_t > > names;
  for (auto i = gr.edges.cbegin(); i != gr.edges.cend(); ++i)
  {
    if (i->first.second == vert_name)
    {
      for (auto j = i->second.begin(); j != i->second.end(); ++j)
      {
        names.pushBack(std::make_pair(i->first.first, *j));
      }
    }
  }
  sort(names, std::less< std::pair< std::string, size_t > >{});
  std::string sep;
  for (auto i = names.begin(); i != names.end();)
  {
    std::string temp = i->first;
    out << sep << i->first << " " << i->second;
    ++i;
    while (i != names.end() && i->first == temp)
    {
      out << " " << i->second;
      ++i;
    }
    sep = "\n";
  }
}

void zharov::bind(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vert_name_1, vert_name_2;
  size_t weight;
  in >> gr_name >> vert_name_1 >> vert_name_2 >> weight;
  graphs.at(gr_name).addEdge(vert_name_1, vert_name_2, weight);
}

void zharov::cut(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vert_name_1, vert_name_2;
  size_t weight;
  in >> gr_name >> vert_name_1 >> vert_name_2 >> weight;
  zharov::Graph& gr = graphs.at(gr_name);
  if (!gr.vertexes.has(vert_name_1) || !gr.vertexes.has(vert_name_2))
  {
    throw std::logic_error("Vertex not found");
  }

  if (!gr.edges.contains(std::make_pair(vert_name_1, vert_name_2)) ||
    !gr.edges.at(std::make_pair(vert_name_1, vert_name_2)).has(weight))
  {
    throw std::logic_error("Edge not found");
  }
  gr.rmEdge(vert_name_1, vert_name_2, weight);
}

void zharov::create(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_name, vertex;
  size_t count;
  in >> gr_name;
  if (graphs.contains(gr_name))
  {
    throw std::logic_error("Graph already exist");
  }
  in >> count;
  Graph gr(count);
  for (size_t i = 0; i < count; ++i)
  {
    in >> vertex;
    gr.addVertex(vertex);
  }
  graphs.add(gr_name, gr);
  if (in.fail())
  {
    in.clear();
  }
}

void zharov::merge(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_new, gr_old_1, gr_old_2;
  in >> gr_new >> gr_old_1 >> gr_old_2;
  if (graphs.contains(gr_new))
  {
    throw std::logic_error("Graph already exist");
  }

  Graph gr;
  gr.vertexes = graphs.at(gr_old_1).vertexes;
  gr.edges = graphs.at(gr_old_1).edges;
  for (auto i = graphs.at(gr_old_2).vertexes.cbegin(); i != graphs.at(gr_old_2).vertexes.cend();
    ++i)
  {
    gr.addVertex(*i);
  }
  for (auto i = graphs.at(gr_old_2).edges.cbegin(); i != graphs.at(gr_old_2).edges.cend(); ++i)
  {
    for (auto j = i->second.cbegin(); j != i->second.cend(); ++j)
    {
      gr.addEdge(i->first.first, i->first.second, *j);
    }
  }

  graphs.add(gr_new, gr);
}

void zharov::extract(std::ostream&, std::istream& in, graphs_table& graphs)
{
  std::string gr_new, gr_old, vertex;
  size_t count;
  in >> gr_new >> gr_old >> count;
  Vector< std::string > vertexes;
  if (graphs.contains(gr_new))
  {
    throw std::logic_error("Graph already exist");
  }

  for (size_t i = 0; i < count; ++i)
  {
    in >> vertex;
    vertexes.pushBack(vertex);
  }
  for (auto i = vertexes.cbegin(); i != vertexes.cend(); ++i)
  {
    if (!graphs.at(gr_old).vertexes.has(*i))
    {
      throw std::logic_error("Vertex not found");
    }
  }

  Graph gr;
  gr.vertexes = vertexes;
  for (auto i = graphs.at(gr_old).edges.cbegin(); i != graphs.at(gr_old).edges.cend(); ++i)
  {
    if (vertexes.has(i->first.first) && vertexes.has(i->first.second))
    {
      for (auto j = i->second.cbegin(); j != i->second.cend(); ++j)
      {
        gr.addEdge(i->first.first, i->first.second, *j);
      }
    }
  }
  graphs.add(gr_new, gr);
}
