#include <functional>
#include "graph.hpp"

bool zubarev::GraphTable::compare(const std::pair< std::string, zubarev::Vector< size_t > >& a,
                                  const std::pair< std::string, zubarev::Vector< size_t > >& b)
{
  return a.first < b.first;
}

void zubarev::GraphTable::graphs(std::ostream& out) const
{
  zubarev::Vector< std::string > names;

  for (auto it = edge_data_.begin(); it != edge_data_.end(); ++it) {
    names.pushBack(it->key);
  }
  if (names.getSize() == 0) {
    out << '\n';
    return;
  }
  zubarev::sort(names.begin(), names.end(), std::less<>{});
  for (auto it = names.begin(); it != names.end(); ++it) {
    out << *it << '\n';
  }
}

void zubarev::GraphTable::vertexes(std::ostream& out, const std::string& graph_name) const
{
  if (!vertex_data_.contains(graph_name)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  const VertexList& verts = vertex_data_.at(graph_name);

  if (verts.getSize() == 0) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  VertexList sorted = verts;
  zubarev::sort(sorted.begin(), sorted.end(), std::less< std::string >{});

  bool first = true;
  for (auto it = sorted.begin(); it != sorted.end(); ++it) {
    if (!first) {
      out << '\n';
    }
    out << *it;
    first = false;
  }
}

void zubarev::GraphTable::outbound(std::ostream& out, const std::string& graph_name, const std::string& vertex) const
{
  if (!edge_data_.contains(graph_name)) {
    out << "<INVALID COMMAND>" << "\n";
    return;
  }
  std::equal_to< std::string > eq;

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
  zubarev::Vector< std::pair< std::string, zubarev::Vector< size_t > > > results;

  for (auto it = graph.begin(); it != graph.end(); ++it) {
    if (eq(it->key.first, vertex)) {
      std::pair< std::string, zubarev::Vector< size_t > > tmp;
      tmp.first = it->key.second;
      tmp.second = it->val;
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

void zubarev::GraphTable::inbound(std::ostream& out, const std::string& graph_name, const std::string& vertex) const
{
  if (!edge_data_.contains(graph_name)) {
    out << "<INVALID COMMAND>" << "\n";
    return;
  }
  std::equal_to< std::string > eq;

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

  zubarev::Vector< std::pair< std::string, zubarev::Vector< size_t > > > results;

  for (auto it = graph.begin(); it != graph.end(); ++it) {
    if (eq(it->key.second, vertex)) {
      std::pair< std::string, zubarev::Vector< size_t > > tmp;
      tmp.first = it->key.first;
      tmp.second = it->val;
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

void zubarev::GraphTable::bind(std::ostream& out,
                               const std::string& graph_name,
                               const std::pair< std::string, std::string >& edge,
                               size_t weight)
{
  if (!vertex_data_.contains(graph_name)) {

    out << "<INVALID COMMAND>" << "\n";
    return;
  }
  auto& edges = edge_data_.at(graph_name);
  auto& verts = vertex_data_.at(graph_name);

  bool need_from = !verts.contains(edge.first);
  bool need_to = !verts.contains(edge.second);

  if (need_from) {
    verts.pushBack(edge.first);
  }

  if (need_to) {
    verts.pushBack(edge.second);
  }

  edges[edge].pushBack(weight);
}

void zubarev::GraphTable::cut(std::ostream& out,
                              const std::string& graph_name,
                              const std::pair< std::string, std::string >& edge,
                              size_t weight)
{
  if (!edge_data_.contains(graph_name)) {

    out << "<INVALID COMMAND>" << "\n";
    return;
  }
  auto& graph = edge_data_.at(graph_name);
  if (!graph.contains(edge)) {
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
    graph.erase(edge);
  }
}

bool zubarev::GraphTable::create(std::ostream& out,
                                 const std::string& graph_name,
                                 size_t count,
                                 const zubarev::Vector< std::string >& vertexes)
{
  if (edge_data_.contains(graph_name)) {
    out << "<INVALID COMMAND>" << "\n";
    return false;
  }
  if (count != vertexes.getSize()) {
    out << "<INVALID COMMAND>" << "\n";
    return false;
  }

  std::equal_to< std::string > eq;
  for (size_t i = 0; i < count; ++i) {
    for (size_t j = i + 1; j < count; ++j) {
      if (eq(vertexes[i], vertexes[j])) {
        out << "<INVALID COMMAND>\n";
        return false;
      }
    }
  }

  using EdgeTable = HashTable< EdgeKey, Weights, SipHash, std::equal_to< EdgeKey > >;
  EdgeTable new_table;

  edge_data_.insert(graph_name, new_table);

  VertexList new_vertices;
  for (size_t i = 0; i < count; ++i) {
    new_vertices.pushBack(vertexes[i]);
  }
  vertex_data_.insert(graph_name, new_vertices);
  return true;
}

void zubarev::GraphTable::merge(std::ostream& out,
                                const std::string& new_name,
                                const std::string& source1,
                                const std::string& source2)
{

  if (!(edge_data_.contains(source1) && edge_data_.contains(source2))) {
    out << "<INVALID COMMAND>" << "\n";
    return;
  }
  if (!create(out, new_name, 0, {})) {
    return;
  }
  using EdgeTable = HashTable< EdgeKey, Weights, SipHash, std::equal_to< EdgeKey > >;

  EdgeTable new_table = edge_data_.at(new_name);
  const EdgeTable& source1_table = edge_data_[source1];
  const EdgeTable& source2_table = edge_data_[source2];

  for (auto it = source1_table.begin(); it != source1_table.end(); ++it) {
    Weights weights(it->val);
    for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
      edge_data_.at(new_name)[it->key].pushBack(*vit);
    }
  }

  for (auto it = source2_table.begin(); it != source2_table.end(); ++it) {
    Weights weights(it->val);
    for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
      edge_data_.at(new_name)[it->key].pushBack(*vit);
    }
  }

  auto& new_verts = vertex_data_.at(new_name);

  const auto& verts1 = vertex_data_.at(source1);
  const auto& verts2 = vertex_data_.at(source2);

  std::equal_to< std::string > eq;

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

void zubarev::GraphTable::extract(std::ostream& out,
                                  const std::string& new_name,
                                  const std::string& source,
                                  size_t count,
                                  const zubarev::Vector< std::string >& vertexes)
{
  if (!edge_data_.contains(source)) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  if (count != vertexes.getSize()) {
    out << "<INVALID COMMAND>\n";
    return;
  }

  std::equal_to< std::string > eq_;

  const auto& source_verts = vertex_data_.at(source);

  for (size_t i = 0; i < count; ++i) {
    bool find = false;

    for (auto it = source_verts.begin(); it != source_verts.end(); ++it) {
      if (eq_(*it, vertexes[i])) {
        find = true;
        break;
      }
    }

    if (!find) {
      out << "<INVALID COMMAND>\n";
      return;
    }
  }

  if (!create(out, new_name, count, vertexes)) {
    return;
  }

  const auto& graph = edge_data_.at(source);

  for (auto it = graph.begin(); it != graph.end(); ++it) {
    const auto& from = it->key.first;
    const auto& to = it->key.second;

    bool flag_from = false;
    bool flag_to = false;

    for (size_t i = 0; i < count; ++i) {
      if (eq_(from, vertexes[i])) {
        flag_from = true;
      }

      if (eq_(to, vertexes[i])) {
        flag_to = true;
      }
    }

    if (flag_from && flag_to) {
      Weights weights(it->val);

      for (auto vit = weights.begin(); vit != weights.end(); ++vit) {
        edge_data_.at(new_name)[it->key].pushBack(*vit);
      }
    }
  }
}
