#include "graph_functions.hpp"
#include <iostream>
#include <cstring>
#include <fstream>
#include "my_hash_table.hpp"

namespace
{
  void skipSpaces(std::istream& in)
  {
    while (in.peek() == ' ')
    {
      in.get();
    }
  }
  bool checkEndLineWithSkipSpaces(std::istream& in)
  {
    skipSpaces(in);
    if (in.peek() == '\n' || in.peek() == EOF)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

void goltsov::detail::sortStringVector(goltsov::Vector< std::string >& vec)
{
  if (vec.getSize() == 0)
  {
    return;
  }
  for (size_t i = 0; i < vec.getSize() - 1; ++i)
  {
    for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
    {
      if (vec[j] > vec[j + 1])
      {
        std::string temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }
}
void goltsov::detail::sortSizeTVector(goltsov::Vector< size_t >& vec)
{
  if (vec.getSize() == 0)
  {
    return;
  }
  for (size_t i = 0; i < vec.getSize() - 1; ++i)
  {
    for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
    {
      if (vec[j] > vec[j + 1])
      {
        size_t temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }
}
void goltsov::detail::sortPairStringSizeTVector(goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > >& vec)
{
  if (vec.getSize() == 0)
  {
    return;
  }
  for (size_t i = 0; i < vec.getSize() - 1; ++i)
  {
    for (size_t j = 0; j < vec.getSize() - i - 1; ++j)
    {
      if (vec[j].first > vec[j + 1].first)
      {
        std::pair< std::string, goltsov::Vector< size_t > > temp = vec[j];
        vec[j] = vec[j + 1];
        vec[j + 1] = temp;
      }
    }
  }
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    sortSizeTVector(vec[i].second);
  }
}

void goltsov::graphsParsing(graphs_t& g, std::istream& in)
{
  if (!checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  graphs(g);
  std::cout << '\n';
}
void goltsov::vertexesParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  if (!(in >> graph_name) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  vertexes(g, graph_name);
  std::cout << '\n';
}
void goltsov::outboundParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  std::string vertexe_name;
  if (!(in >> graph_name) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_name) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  outbound(g, graph_name, vertexe_name);
  std::cout << '\n';
}
void goltsov::inboundParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  std::string vertexe_name;
  if (!(in >> graph_name) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_name) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  inbound(g, graph_name, vertexe_name);
  std::cout << '\n';
}
void goltsov::bindParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  std::string vertexe_a, vertexe_b;
  size_t weight;
  if (!(in >> graph_name) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_a) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_b) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> weight) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  bind(g, graph_name, vertexe_a, vertexe_b, weight);
}
void goltsov::cutParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  std::string vertexe_a, vertexe_b;
  size_t weight;
  if (!(in >> graph_name) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_a) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> vertexe_b) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> weight) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  cut(g, graph_name, vertexe_a, vertexe_b, weight);
}
void goltsov::createParsing(graphs_t& g, std::istream& in)
{
  std::string graph_name;
  size_t count;
  if (!(in >> graph_name) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> count) || checkEndLineWithSkipSpaces(in))
  {
    if (count == 0)
    {
      create(g, graph_name, count, goltsov::Vector< std::string > {});
      return;
    }
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::Vector< std::string > vertexes_names;
  for (size_t i = 0; i < count - 1; ++i)
  {
    std::string name;
    if (!(in >> name) || checkEndLineWithSkipSpaces(in))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    vertexes_names.pushBack(name);
  }
  std::string name;
  if (!(in >> name) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  vertexes_names.pushBack(name);
  create(g, graph_name, count, vertexes_names);
}
void goltsov::mergeParsing(graphs_t& g, std::istream& in)
{
  std::string new_graph;
  std::string old_graph_1, old_graph_2;
  if (!(in >> new_graph) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> old_graph_1) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> old_graph_2) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  merge(g, new_graph, old_graph_1, old_graph_2);
}
void goltsov::extractParsing(graphs_t& g, std::istream& in)
{
  std::string new_graph;
  std::string old_graph;
  size_t count;
  if (!(in >> new_graph) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> old_graph) || checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!(in >> count) || checkEndLineWithSkipSpaces(in))
  {
    if (count == 0)
    {
      extract(g, new_graph, old_graph, count, goltsov::Vector< std::string > {});
      return;
    }
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::Vector< std::string > vertexes_names;
  for (size_t i = 0; i < count - 1; ++i)
  {
    std::string name;
    if (!(in >> name) || checkEndLineWithSkipSpaces(in))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
    vertexes_names.pushBack(name);
  }
  std::string name;
  if (!(in >> name) || !checkEndLineWithSkipSpaces(in))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  vertexes_names.pushBack(name);
  extract(g, new_graph, old_graph, count, vertexes_names);
}
void goltsov::read_graphs(std::istream& in, graphs_t& graphs)
{
  std::string name_graph;
  size_t count_vertexes;
  while (in >> name_graph >> count_vertexes)
  {
    graphs[name_graph];
    for (size_t i = 0; i < count_vertexes; ++i)
    {
      std::string name_vertex1, name_vertex2;
      size_t weight;
      if (!(in >> name_vertex1 >> name_vertex2 >> weight))
      {
        throw std::runtime_error("Bad input file");
      }
      graphs[name_graph][{name_vertex1, name_vertex2}].pushBack(weight);
    }
  }
}
void goltsov::graphs(graphs_t& graphs)
{
  goltsov::Vector< std::string > name_graphs;
  for (graphs_iter_t it = graphs.begin(); it != graphs.end(); ++it)
  {
    name_graphs.pushBack(it->first);
  }
  detail::sortStringVector(name_graphs);
  if (name_graphs.getSize())
  {
    std::cout << name_graphs[0];
  }
  for (size_t i = 1; i < name_graphs.getSize(); ++i)
  {
    std::cout << '\n' << name_graphs[i];
  }
}
void goltsov::vertexes(graphs_t& graphs, std::string name_graph)
{
  if (!graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::Vector< std::string > name_vertexes;
  for (pairs_vertexes_iter_t it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
  {
    name_vertexes.pushBack(it->first.first);
    name_vertexes.pushBack(it->first.second);
  }
  detail::sortStringVector(name_vertexes);
  if (name_vertexes.getSize())
  {
    std::cout << name_vertexes[0];
  }
  for (size_t i = 1; i < name_vertexes.getSize(); ++i)
  {
    if (name_vertexes[i] != name_vertexes[i - 1])
    {
      std::cout << '\n' << name_vertexes[i];
    }
  }
}
void goltsov::outbound(graphs_t& graphs, std::string name_graph, std::string name_vertex)
{
  if (!graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  bool is_has_name = false;
  goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > > name_weight_vertexes;
  for (pairs_vertexes_iter_t it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
  {
    if (it->first.first == name_vertex)
    {
      name_weight_vertexes.pushBack({it->first.second, it->second});
      is_has_name = true;
    }
    else if (it->first.second == name_vertex)
    {
      is_has_name = true;
    }
  }
  if (!is_has_name)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  detail::sortPairStringSizeTVector(name_weight_vertexes);
  if (name_weight_vertexes.getSize() && name_weight_vertexes[0].second.getSize())
  {
    std::cout << name_weight_vertexes[0].first;
    for (size_t j = 0; j < name_weight_vertexes[0].second.getSize(); ++j)
    {
      std::cout << " " << name_weight_vertexes[0].second[j];
    }
  }
  for (size_t i = 1; i < name_weight_vertexes.getSize(); ++i)
  {
    if (name_weight_vertexes[i].second.getSize())
    {
      std::cout << '\n';
      std::cout << name_weight_vertexes[i].first;
      for (size_t j = 0; j < name_weight_vertexes[i].second.getSize(); ++j)
      {
        std::cout << " " << name_weight_vertexes[i].second[j];
      }
    }
  }
}
void goltsov::inbound(graphs_t& graphs, std::string name_graph, std::string name_vertex)
{
  if (!graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  bool is_has_name = false;
  goltsov::Vector< std::pair< std::string, goltsov::Vector< size_t > > > name_weight_vertexes;
  for (pairs_vertexes_iter_t it = graphs[name_graph].begin(); it != graphs[name_graph].end(); ++it)
  {
    if (it->first.second == name_vertex)
    {
      name_weight_vertexes.pushBack({it->first.first, it->second});
      is_has_name = true;
    }
    else if (it->first.first == name_vertex)
    {
      is_has_name = true;
    }
  }
  if (!is_has_name)
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  detail::sortPairStringSizeTVector(name_weight_vertexes);
  if (name_weight_vertexes.getSize() && name_weight_vertexes[0].second.getSize())
  {
    std::cout << name_weight_vertexes[0].first;
    for (size_t j = 0; j < name_weight_vertexes[0].second.getSize(); ++j)
    {
      std::cout << " " << name_weight_vertexes[0].second[j];
    }
  }
  for (size_t i = 1; i < name_weight_vertexes.getSize(); ++i)
  {
    if (name_weight_vertexes[i].second.getSize())
    {
      std::cout << '\n';
      std::cout << name_weight_vertexes[i].first;
      for (size_t j = 0; j < name_weight_vertexes[i].second.getSize(); ++j)
      {
        std::cout << " " << name_weight_vertexes[i].second[j];
      }
    }
  }
}
void goltsov::bind(graphs_t& graphs, std::string name_graph, std::string name_vertex1,
  std::string name_vertex2, size_t weight)
{
  if (!graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  graphs[name_graph][{name_vertex1, name_vertex2}].pushBack(weight);
}
void goltsov::cut(graphs_t& graphs, std::string name_graph, std::string name_vertex1,
  std::string name_vertex2, size_t weight)
{
  if (!graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!graphs[name_graph].contains({name_vertex1, name_vertex2}))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  for (size_t i = 0; i < graphs[name_graph][{name_vertex1, name_vertex2}].getSize(); ++i)
  {
    if (graphs[name_graph][{name_vertex1, name_vertex2}][i] == weight)
    {
      graphs[name_graph][{name_vertex1, name_vertex2}].erase(i);
      return;
    }
  }
  throw std::runtime_error("<INVALID COMMAND>");
}
void goltsov::create(graphs_t& graphs, std::string name_graph, size_t count, goltsov::Vector< std::string > vertexes_names)
{
  if (graphs.contains(name_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  graphs[name_graph];
  for (size_t i = 0; i < count; ++i)
  {
    graphs[name_graph][{vertexes_names[i], vertexes_names[0]}];
  }
}
void goltsov::merge(graphs_t& graphs, std::string name_new_graph, std::string name_graph1, std::string name_graph2)
{
  if (graphs.contains(name_new_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!graphs.contains(name_graph1))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!graphs.contains(name_graph2))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  for (pairs_vertexes_iter_t it = graphs[name_graph1].begin(); it != graphs[name_graph1].end(); ++it)
  {
    graphs[name_new_graph][it->first].pushBackRange(
      graphs[name_graph1][it->first].begin(), graphs[name_graph1][it->first].getSize());
  }
  for (pairs_vertexes_iter_t it = graphs[name_graph1].begin(); it != graphs[name_graph1].end(); ++it)
  {
    graphs[name_new_graph][it->first].pushBackRange(
      graphs[name_graph2][it->first].begin(), graphs[name_graph2][it->first].getSize());
  }
}
void goltsov::extract(graphs_t& graphs, std::string name_new_graph, std::string name_old_graph,
  size_t count_vertexes, goltsov::Vector< std::string > vertexes)
{
  if (graphs.contains(name_new_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  if (!graphs.contains(name_old_graph))
  {
    throw std::runtime_error("<INVALID COMMAND>");
  }
  goltsov::HashTable< std::string, std::string, goltsov::Sha1Hasher< std::string >,
    std::equal_to< std::string > > name_vertexes;
  for (pairs_vertexes_iter_t it = graphs[name_old_graph].begin(); it != graphs[name_old_graph].end(); ++it)
  {
    name_vertexes[it->first.first];
    name_vertexes[it->first.second];
  }
  for (size_t i = 0; i < count_vertexes; ++i)
  {
    if (!name_vertexes.contains(vertexes[i]))
    {
      throw std::runtime_error("<INVALID COMMAND>");
    }
  }
  for (size_t i = 0; i < count_vertexes; ++i)
  {
    for (size_t j = 0; j < count_vertexes; ++j)
    {
      if (i != j && graphs[name_old_graph].contains({vertexes[i], vertexes[j]}))
      {
        graphs[name_new_graph][{vertexes[i], vertexes[j]}].pushBackRange(
          graphs[name_old_graph][{vertexes[i], vertexes[j]}].begin(),
          graphs[name_old_graph][{vertexes[i],vertexes[j]}].getSize());
      }
    }
  }
}
