#include "Grath.hpp"
#include "HIter.hpp"
#include "CHIter.hpp"

namespace petrov
{
  template< class T, class Comp >
  void sort(topit::Vector<T>& vec, Comp comp)
  {
    for (size_t i = 1; i < vec.getSize(); ++i)
    {
      T key = vec[i];
      size_t j = i;
      while (j > 0 && comp(key, vec[j-1]))
      {
        vec[j] = vec[j-1];
        --j;
      }
      vec[j] = key;
    }
  }

  Grath::Grath():
    edges_(),
    vertices_()
  {}

  Grath::Grath(const Grath& other):
    edges_(other.edges_),
    vertices_(other.vertices_)
  {}

  Grath::Grath(Grath&& other) noexcept:
    edges_(std::move(other.edges_)),
    vertices_(std::move(other.vertices_))
  {}

  void Grath::addVertex(const std::string& name)
  {
    if (!vertices_.has(name))
    {
      vertices_.add(name, true);
    }
  }

  void Grath::addEdge(const std::string& from, const std::string& to, size_t w)
  {
    addVertex(from);
    addVertex(to);

    EdgeKey key{from, to};
    if (edges_.has(key))
    {
      edges_.get(key).pushBack(w);
    }
    else
    {
      Weight weights;
      weights.pushBack(w);
      edges_.add(key, weights);
    }
  }

  void Grath::removeVertex(const std::string& name)
  {
    if (!hasVertex(name))
    {
      throw std::runtime_error("Vertex not found\n");
    }
    vertices_.drop(name);
  }

  void Grath::removeEdge(const std::string& from, const std::string& to, size_t w)
  {
    if (hasEdge(from, to))
    {
      Weight& wght = edges_.get({from, to});
      for (size_t i = 0; i < wght.getSize(); ++i)
      {
        if (wght[i] == w)
        {
          wght.erase(i);
          if (wght.getSize() == 0) {
            edges_.drop({from, to});
          }
          return;
        }
      }
      throw std::runtime_error("Edge with such weight not found\n");
    }
    else
    {
      throw std::runtime_error("Edge not found\n");
    }
  }

  bool Grath::hasVertex(const std::string& name) const
  {
    return vertices_.has(name);
  }

  bool Grath::hasEdge(const std::string& from, const std::string& to) const
  {
    return edges_.has({from, to});
  }

  topit::Vector< std::string > Grath::getVertices() const
  {
    CHIter< std::string, bool > it = vertices_.begin();
    topit::Vector< std::string > res;
    for (; it != vertices_.end(); ++it)
    {
      res.pushBack(it->key_);
    }
    sort(res, Less< std::string >());
    return res;
  }

  topit::Vector< EdgeVec > Grath::getInputEdges(const std::string& name) const
  {
    topit::Vector< EdgeVec > res;
    CHIter< EdgeKey, Weight > it = edges_.begin();
    for (; it != edges_.end(); ++it) {
      if (it->key_.second == name) {
        Weight sortedWeights = it->value_;
        sort(sortedWeights, Less< size_t >());
        res.pushBack({it->key_.first, sortedWeights});
      }
    }
    sort(res, [](const EdgeVec& a, const EdgeVec& b) { return a.first < b.first; });
    return res;
  }

  topit::Vector< EdgeVec > Grath::getOutputEdges(const std::string& name) const
  {
    topit::Vector< EdgeVec > res;
    CHIter< EdgeKey, Weight > it = edges_.begin();
    for (; it != edges_.end(); ++it) {
      if (it->key_.first == name) {
        Weight sortedWeights = it->value_;
        sort(sortedWeights, Less< size_t >());
        res.pushBack({it->key_.second, sortedWeights});
      }
    }
    sort(res, [](const EdgeVec& a, const EdgeVec& b) { return a.first < b.first; });
    return res;
  }

  void Grath::swap(Grath& other) noexcept
  {
    edges_.swap(other.edges_);
    vertices_.swap(other.vertices_);
  }
}
