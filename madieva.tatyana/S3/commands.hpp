#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph_storage.hpp"
#include <limits>

namespace madieva {
  template<typename T>
  bool read_token(std::istream& in, std::ostream& out, T& value) {
    if (!(in >> value)) {
      out << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      return false;
    }
    return true;
  }
  void cmd_graphs(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_vertexes(std::istream & in, std::ostream & out, GraphStorage & storage);
  List< size_t > sort_ascending_order(const List< size_t > & edge);
  void cmd_outbound(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_inbound(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_bind(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_cut(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_create(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_merge(std::istream & in, std::ostream & out, GraphStorage & storage);
  void cmd_extract(std::istream & in, std::ostream & out, GraphStorage & storage);




  void cmd_graphs(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.size()) {
      out << "\n";
      return;
    }
    List< std::string > list_name = storage.getAllNames();
    LIter< std::string > it = list_name.begin();
    for (size_t i = 0; i < storage.size(); ++i) {
      out << (*it) << '\n';
      ++it;
    }
  }

  void cmd_vertexes(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Graph & graph = storage.getGraph(name);
    List< std::string > vertex = graph.getVertices();
    if (!vertex.size()) {
      out << "\n";
      return;
    }
    LIter<std::string > it = vertex.begin();
    for (size_t i = 0; i < vertex.size(); ++i) {
      out << (*it) << "\n";
      ++it;
    }
  }

  List< size_t > sort_ascending_order(const List< size_t > & edge)
  {
    List< size_t > sort_edge = edge;
    for (size_t i = 0; i < sort_edge.size(); ++i) {
      LIter< size_t > it = sort_edge.begin();
      for (size_t j = 0; j < sort_edge.size() - i - 1; ++j) {
        LIter< size_t > next = it;
        ++next;
        if ((*it) > (*next)) {
          size_t temp = *it;
          *it = *next;
          *next = temp;
        }
        ++it;
      }
    }
    return sort_edge;
  }

  void cmd_outbound(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string a;
    if (!(in >> a)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Graph & graph = storage.getGraph(name);
    if (!graph.hasVertex(a)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    bool exists = false;
    List< std::string > list_v = graph.getVertices();
    LIter< std::string > it = list_v.begin();
    for (size_t i = 0; i < list_v.size(); ++i) {
      const std::string & b = (*it);
      std::pair<std::string, std::string> key(a, b);
      if (graph.getEdges().has(key)) {
        List< size_t > edge = graph.getEdges().get(key);
        out << b;
        List< size_t > sort_edge = sort_ascending_order(edge);
        LIter<size_t> e_it = sort_edge.begin();
        for (size_t j = 0; j < sort_edge.size(); ++j) {
          out << " " << *e_it;
          ++e_it;
        }
        out << "\n";
        exists = true;
      }
      ++it;
    }
    if (!exists) {
      out << "\n";
    }
  }

  void cmd_inbound(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!(in >> name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string b;
    if (!(in >> b)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Graph & graph = storage.getGraph(name);
    if (!graph.hasVertex(b)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    List< std::string > list_v = graph.getVertices();
    LIter< std::string > it = list_v.begin();
    for (size_t i = 0; i < list_v.size(); ++i) {
      const std::string & a = (*it);
      std::pair<std::string, std::string> key(a, b);
      if (graph.getEdges().has(key)) {
        List< size_t > edge = graph.getEdges().get(key);
        out << a;
        List< size_t > sort_edge = sort_ascending_order(edge);
        LIter<size_t> e_it = sort_edge.begin();
        for (size_t j = 0; j < sort_edge.size(); ++j) {
          out << " " << *e_it;
          ++e_it;
        }
        out << "\n";
      }
      ++it;
    }
  }

  void cmd_bind(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!read_token(in, out, name)) return;
    std::string a;
    if (!read_token(in, out, a)) return;
    std::string b;
    if (!read_token(in, out, b)) return;
    size_t edge = 0;
    if (!read_token(in, out, edge)) return;
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph & graph = storage.getGraph(name);
    graph.addEdge(a, b, edge);
  }

  void cmd_cut(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!read_token(in, out, name)) return;
    std::string a;
    if (!read_token(in, out, a)) return;
    std::string b;
    if (!read_token(in, out, b)) return;
    size_t edge = 0;
    if (!read_token(in, out, edge)) return;
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph & graph = storage.getGraph(name);
    if (!graph.hasVertex(a) || !graph.hasVertex(b)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    std::pair<std::string, std::string> key(a, b);
    if (!graph.getEdges().has(key)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    List< size_t > & list = graph.getEdges().get(key);
    LIter< size_t > it = list.begin();
    for (size_t i = 0; i < list.size(); ++i) {
      if ((*it) == edge) {
        list.erase(it);
        if (list.size() == 0) {
          graph.getEdges().drop(key);
        }
        return;
      }
      ++it;
    }
    out << "<INVALID COMMAND>\n";
  }

  void cmd_create(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string name;
    if (!read_token(in, out, name)) return;
    size_t count = 0;
    if (!read_token(in, out, count)) return;
    List<std::string> vertices;
    for (size_t i = 0; i < count; ++i) {
      std::string vertex;
      if (!read_token(in, out, vertex)) return;
      vertices.push_back(vertex);
    }
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    Graph new_graph(name);
    LIter<std::string> it = vertices.begin();
    for (size_t i = 0; i < vertices.size(); ++i) {
      new_graph.addVertex(*it);
      ++it;
    }
    storage.addGraph(name, new_graph);
  }

  void cmd_merge(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string new_name, name1, name2;
    if (!read_token(in, out, new_name)) return;
    if (!read_token(in, out, name1)) return;
    if (!read_token(in, out, name2)) return;
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (storage.hasGraph(new_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name1)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name2)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Graph & graph1 = storage.getGraph(name1);
    const Graph & graph2 = storage.getGraph(name2);
    Graph new_graph(new_name);
    List<std::string> vertex1 = graph1.getVertices();
    LIter<std::string> it1 = vertex1.begin();
    for (size_t i = 0; i < vertex1.size(); ++i) {
      new_graph.addVertex(*it1);
      ++it1;
    }
    List<std::string> vertex2 = graph2.getVertices();
    LIter<std::string> it2 = vertex2.begin();
    for (size_t i = 0; i < vertex2.size(); ++i) {
      new_graph.addVertex(*it2);
      ++it2;
    }
    const HashTable< std::pair< std::string, std::string >,
      List< size_t >, XXHash64, PairEqual > & edges1 = graph1.getEdges();
    HTCIter< std::pair< std::string, std::string >,
      List< size_t >, XXHash64, PairEqual > e_it1 = edges1.begin();
    for (; e_it1 != edges1.end(); ++e_it1) {
      const std::pair<std::string, std::string> & key = (*e_it1).first;
      const List<size_t> & edge = (*e_it1).second;
      LCIter<size_t> w_it = edge.begin();
      for (size_t i = 0; i < edge.size(); ++i) {
        new_graph.addEdge(key.first, key.second, *w_it);
        ++w_it;
      }
    }

    const HashTable< std::pair< std::string, std::string >,
      List< size_t >, XXHash64, PairEqual > & edges2 = graph2.getEdges();
    HTCIter< std::pair< std::string, std::string >,
      List< size_t >, XXHash64, PairEqual > e_it2 = edges2.begin();
    for (; e_it2 != edges2.end(); ++e_it2) {
      const std::pair<std::string, std::string> & key = (*e_it2).first;
      const List<size_t> & edge = (*e_it2).second;
      LCIter<size_t> w_it = edge.begin();
      for (size_t i = 0; i < edge.size(); ++i) {
        new_graph.addEdge(key.first, key.second, *w_it);
        ++w_it;
      }
    }
    storage.addGraph(new_name, new_graph);
  }

  void cmd_extract(std::istream & in, std::ostream & out, GraphStorage & storage)
  {
    std::string new_name, name;
    if (!read_token(in, out, new_name)) return;
    if (!read_token(in, out, name)) return;
    size_t count = 0;
    if (!read_token(in, out, count)) return;
    List< std::string > new_vertexes;
    for (size_t i = 0; i < count; ++i) {
      std::string vertex;
      if (!read_token(in, out, vertex)) return;
      new_vertexes.push_back(vertex);
    }
    std::string line;
    std::getline(in, line);
    if (!line.empty()) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (storage.hasGraph(new_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    if (!storage.hasGraph(name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }
    const Graph & graph = storage.getGraph(name);
    LIter<std::string> it = new_vertexes.begin();
    for (size_t i = 0; i < new_vertexes.size(); ++i) {
      if (!graph.hasVertex(*it)) {
        out << "<INVALID COMMAND>\n";
        return;
      }
      ++it;
    }
    Graph new_graph(new_name);

    LIter<std::string> v_it = new_vertexes.begin();
    for (size_t i = 0; i < new_vertexes.size(); ++i) {
      new_graph.addVertex(*v_it);
      ++v_it;
    }


    const HashTable< std::pair< std::string, std::string >, List< size_t >,
      XXHash64, PairEqual > & edges = graph.getEdges();
    HTCIter< std::pair< std::string, std::string >, List< size_t >,
      XXHash64, PairEqual > e_it = edges.begin();

    for (; e_it != edges.end(); ++e_it) {
      const std::pair< std::string, std::string > & key = (*e_it).first;
      const List< size_t > & weights = (*e_it).second;
      bool a_exists = false;
      bool b_exists = false;
      LIter< std::string > check_from = new_vertexes.begin();
      for (size_t i = 0; i < new_vertexes.size(); ++i) {
        if (*check_from == key.first) a_exists = true;
        if (*check_from == key.second) b_exists = true;
        ++check_from;
      }
      if (a_exists && b_exists) {
        LCIter<size_t> w_it = weights.begin();
        for (size_t i = 0; i < weights.size(); ++i) {
          new_graph.addEdge(key.first, key.second, *w_it);
          ++w_it;
        }
      }
    }
    storage.addGraph(new_name, new_graph);
  }
}

#endif
