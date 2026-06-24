#include "Grath.hpp"
#include "HIter.hpp"
#include "CHIter.hpp"

template< class T, class Comp >
void petrov::detail::sort(petrov::Vector< T >& vec, Comp comp)
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

void petrov::Grath::addVertex(const std::string& name)
{
  if (!vertices_.has(name))
  {
    vertices_.add(name, true);
  }
}

void petrov::Grath::addEdge(const std::string& from, const std::string& to, size_t w)
{
  bool fromAdded = !hasVertex(from);
  bool toAdded = !hasVertex(to);

  try
  {
    addVertex(from);
    addVertex(to);

    EdgeKey key{from, to};
    if (edges_.has(key))
    {
      edges_.at(key).pushBack(w);
    }
    else
    {
      Weight weights;
      weights.pushBack(w);
      edges_.add(key, std::move(weights));
    }
  }
  catch (...)
  {
    if (toAdded && hasVertex(to))
    {
      vertices_.drop(to);
    }
    if (fromAdded && hasVertex(from))
    {
      vertices_.drop(from);
    }
    throw;
  }
}

void petrov::Grath::removeVertex(const std::string& name)
{
  if (!hasVertex(name))
  {
    throw std::runtime_error("Vertex not found\n");
  }
  vertices_.drop(name);
}

void petrov::Grath::removeEdge(const std::string& from, const std::string& to, size_t w)
{
  if (hasEdge(from, to))
  {
    Weight& wght = edges_.at({from, to});
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

bool petrov::Grath::hasVertex(const std::string& name) const
{
  return vertices_.has(name);
}

bool petrov::Grath::hasEdge(const std::string& from, const std::string& to) const
{
  return edges_.has({from, to});
}

petrov::Vector< std::string > petrov::Grath::getVertices() const
{
  CHIter< std::string, bool > it = vertices_.begin();
  petrov::Vector< std::string > res;
  for (; it != vertices_.end(); ++it)
  {
    res.pushBack(it->first);
  }
  sort(res, std::less< std::string >());
  return res;
}

petrov::Vector< petrov::EdgeVec > petrov::Grath::getInputEdges(const std::string& name) const
{
  petrov::Vector< EdgeVec > res;
  CHIter< EdgeKey, Weight > it = edges_.begin();
  for (; it != edges_.end(); ++it)
  {
    if (it->first.second == name)
    {
      Weight sortedWeights = it->second;
      sort(sortedWeights, std::less< size_t >());
      res.pushBack({it->first.first, sortedWeights});
    }
  }
  sort(res, detail::CompareEdgeVec());
  return res;
}

petrov::Vector< petrov::EdgeVec > petrov::Grath::getOutputEdges(const std::string& name) const
{
  petrov::Vector< EdgeVec > res;
  CHIter< EdgeKey, Weight > it = edges_.begin();
  for (; it != edges_.end(); ++it)
  {
    if (it->first.first == name)
    {
      Weight sortedWeights = it->second;
      sort(sortedWeights, std::less< size_t >());
      res.pushBack({it->first.second, sortedWeights});
    }
  }
  sort(res, detail::CompareEdgeVec());
  return res;
}

void petrov::Grath::swap(Grath& other) noexcept
{
  edges_.swap(other.edges_);
  vertices_.swap(other.vertices_);
}
