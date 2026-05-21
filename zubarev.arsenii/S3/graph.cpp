#include "graph.hpp"
namespace zubarev
{
  void GraphTable::graphs(std::ostream& out) const
  {
    topit::Vector< std::string > names;

    for (auto it = edge_data_.begin(); it != edge_data_.end(); ++it) {
      names.pushBack(it->key_);
    }
    if (names.getSize() == 0) {
      out << '\n';
      return;
    }
    zubarev::sort(names.begin(), names.end(), [](const std::string& a, const std::string& b) { return a < b; });
    for (auto it = names.begin(); it != names.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::vertexes(const std::string& graph_name, std::ostream& out) const
  {

    if (!vertex_data_.has(graph_name)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    VertexList verts = vertex_data_.at(graph_name);

    if (verts.getSize() == 0) {
      out << '\n';
      return;
    }

    zubarev::sort(verts.begin(), verts.end(), [](const std::string& a, const std::string& b) { return a < b; });

    for (auto it = verts.begin(); it != verts.end(); ++it) {
      out << *it << '\n';
    }
  }

  void GraphTable::outbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {
    if (!edge_data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    Equaler< std::string > eq;

    const auto& graph = edge_data_.at(graph_name);
    const auto& verts = vertex_data_.at(graph_name);
    bool find = false;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
      if (eq(*it, vertex)) {
        find = true;
        break;
      }
    }

    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.first, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.second;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    zubarev::sort(results.begin(), results.end(), compare);
    if (results.getSize() == 0) {
      out << '\n';
      return;
    }
    for (auto it = results.begin(); it != results.end(); ++it) {
      out << (*it).first;
      for (auto w = (*it).second.begin(); w != (*it).second.end(); ++w) {
        out << " ";
        out << *w;
      }
      out << '\n';
    }
  }

  void GraphTable::inbound(const std::string& graph_name, const std::string& vertex, std::ostream& out) const
  {
    if (!edge_data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    Equaler< std::string > eq;

    const auto& graph = edge_data_.at(graph_name);
    const auto& verts = vertex_data_.at(graph_name);
    bool find = false;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
      if (eq(*it, vertex)) {
        find = true;
        break;
      }
    }

    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }

    topit::Vector< std::pair< std::string, topit::Vector< size_t > > > results;

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      if (eq(it->key_.second, vertex)) {
        std::pair< std::string, topit::Vector< size_t > > tmp;
        tmp.first = it->key_.first;
        tmp.second = it->val_;
        results.pushBack(tmp);
      }
    }
    zubarev::sort(results.begin(), results.end(), compare);
    if (results.getSize() == 0) {
      out << '\n';
      return;
    }
    for (auto it = results.begin(); it != results.end(); ++it) {
      out << (*it).first;
      for (auto w = (*it).second.begin(); w != (*it).second.end(); ++w) {
        out << " ";
        out << *w;
      }
      out << '\n';
    }
  }

  void GraphTable::bind(const std::string& graph_name,
                        const std::pair< std::string, std::string >& edge,
                        size_t weight,
                        std::ostream& out)
  {
    if (!vertex_data_.has(graph_name)) {

      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    Equaler< std::string > eq_;
    auto& edges = edge_data_.at(graph_name);
    auto& verts = vertex_data_.at(graph_name);

    bool flag_from = false;
    bool flag_to = false;
    for (auto it = verts.begin(); it != verts.end(); ++it) {
      if (eq_(*it, edge.first)) {
        flag_from = true;
      }
      if (eq_(*it, edge.second)) {
        flag_to = true;
      }
    }
    if (!flag_from) {
      verts.pushBack(edge.first);
    }
    if (!flag_to) {
      verts.pushBack(edge.second);
    }

    edges[edge].pushBack(weight);
  }

  void GraphTable::cut(const std::string& graph_name,
                       const std::pair< std::string, std::string >& edge,
                       size_t weight,
                       std::ostream& out)
  {
    if (!edge_data_.has(graph_name)) {

      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    auto& graph = edge_data_.at(graph_name);
    if (!graph.has(edge)) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    auto& weights = graph.at(edge);
    bool find = false;

    for (size_t i = 0; i < weights.getSize(); ++i) {
      if (weights[i] == weight) {
        weights.erase(i);
        find = true;
        break;
      }
    }
    if (!find) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (weights.getSize() == 0) {
      graph.drop(edge);
    }
  }

  bool GraphTable::create(const std::string& graph_name,
                          size_t count,
                          const topit::Vector< std::string >& vertices,
                          std::ostream& out)
  {
    if (edge_data_.has(graph_name)) {
      out << "<INVALID COMMAND>" << "\n";
      return false;
    }
    if (count != vertices.getSize()) {
      out << "<INVALID COMMAND>" << "\n";
      return false;
    }

    Equaler< std::string > eq;
    for (size_t i = 0; i < count; ++i) {
      for (size_t j = i + 1; j < count; ++j) {
        if (eq(vertices[i], vertices[j])) {
          out << "<INVALID COMMAND>\n";
          return false;
        }
      }
    }

    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;
    EdgeTable new_table;

    edge_data_.add(graph_name, new_table);

    VertexList new_vertices;
    for (size_t i = 0; i < count; ++i) {
      new_vertices.pushBack(vertices[i]);
    }
    vertex_data_.add(graph_name, new_vertices);
    return true;
  }

  void GraphTable::merge(const std::string& new_name,
                         const std::string& source1,
                         const std::string& source2,
                         std::ostream& out)
  {

    if (!(edge_data_.has(source1) && edge_data_.has(source2))) {
      out << "<INVALID COMMAND>" << "\n";
      return;
    }
    if (!create(new_name, 0, {}, out)) {
      return;
    }
    using EdgeTable = HashTable< EdgeKey, Weights, SipHash, Equaler< EdgeKey > >;

    EdgeTable new_table = edge_data_.at(new_name);
    const EdgeTable& source1_table = edge_data_[source1];
    const EdgeTable& source2_table = edge_data_[source2];

    for (auto it = source1_table.begin(); it != source1_table.end(); ++it) {
      Weights weights(it->val_);
      for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
        edge_data_.at(new_name)[it->key_].pushBack(*vit);
      }
    }

    for (auto it = source2_table.begin(); it != source2_table.end(); ++it) {
      Weights weights(it->val_);
      for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
        edge_data_.at(new_name)[it->key_].pushBack(*vit);
      }
    }

    auto& new_verts = vertex_data_.at(new_name);

    const auto& verts1 = vertex_data_.at(source1);
    const auto& verts2 = vertex_data_.at(source2);

    Equaler< std::string > eq;

    for (auto it = verts1.begin(); it != verts1.end(); ++it) {
      bool found = false;

      for (auto jt = new_verts.begin(); jt != new_verts.end(); ++jt) {
        if (eq(*it, *jt)) {
          found = true;
          break;
        }
      }

      if (!found) {
        new_verts.pushBack(*it);
      }
    }

    for (auto it = verts2.begin(); it != verts2.end(); ++it) {
      bool found = false;

      for (auto jt = new_verts.begin(); jt != new_verts.end(); ++jt) {
        if (eq(*it, *jt)) {
          found = true;
          break;
        }
      }

      if (!found) {
        new_verts.pushBack(*it);
      }
    }
  }

  void GraphTable::extract(const std::string& new_name,
                           const std::string& source,
                           size_t count,
                           const topit::Vector< std::string >& vertices,
                           std::ostream& out)
  {
    if (!edge_data_.has(source)) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    if (count != vertices.getSize()) {
      out << "<INVALID COMMAND>\n";
      return;
    }

    Equaler< std::string > eq_;

    const auto& source_verts = vertex_data_.at(source);

    for (size_t i = 0; i < count; ++i) {
      bool find = false;

      for (auto it = source_verts.begin(); it != source_verts.end(); ++it) {
        if (eq_(*it, vertices[i])) {
          find = true;
          break;
        }
      }

      if (!find) {
        out << "<INVALID COMMAND>\n";
        return;
      }
    }

    if (!create(new_name, count, vertices, out)) {
      return;
    }

    const auto& graph = edge_data_.at(source);

    for (auto it = graph.begin(); it != graph.end(); ++it) {
      const auto& from = it->key_.first;
      const auto& to = it->key_.second;

      bool flag_from = false;
      bool flag_to = false;

      for (size_t i = 0; i < count; ++i) {
        if (eq_(from, vertices[i])) {
          flag_from = true;
        }

        if (eq_(to, vertices[i])) {
          flag_to = true;
        }
      }

      if (flag_from && flag_to) {
        Weights weights(it->val_);

        for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
          edge_data_.at(new_name)[it->key_].pushBack(*vit);
        }
      }
    }
  }
}
