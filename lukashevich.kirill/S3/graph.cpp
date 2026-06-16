#include "graph.hpp"

#include <cstddef>
#include <ostream>
#include <stdexcept>
#include <string>
#include <utility>

namespace lukashevich {
  namespace detail {
    const size_t GRAPH_BUCKET_COUNT = 251;
    const size_t GRAPH_BUCKET_SIZE = 8;

    struct StringLess
    {
      bool operator()(const std::string & lhs, const std::string & rhs) const
      {
        return lhs < rhs;
      }
    };

    struct WeightLess
    {
      bool operator()(Weight lhs, Weight rhs) const
      {
        return lhs < rhs;
      }
    };

    struct EdgeInfo
    {
      std::string vertex_;
      WeightVector weights_;
    };

    struct EdgeInfoLess
    {
      bool operator()(const EdgeInfo & lhs, const EdgeInfo & rhs) const
      {
        return lhs.vertex_ < rhs.vertex_;
      }
    };

    template< class T, class Compare >
    void selectionSort(Vector< T > & values, Compare compare)
    {
      for (size_t i = 0; i < values.getSize(); ++i) {
        size_t min = i;

        for (size_t j = i + 1; j < values.getSize(); ++j) {
          if (compare(values[j], values[min])) {
            min = j;
          }
        }

        if (min != i) {
          std::swap(values[i], values[min]);
        }
      }
    }

    bool hasWeight(const WeightVector & weights, Weight weight)
    {
      for (size_t i = 0; i < weights.getSize(); ++i) {
        if (weights[i] == weight) {
          return true;
        }
      }

      return false;
    }

    void removeWeight(WeightVector & weights, Weight weight)
    {
      for (size_t i = 0; i < weights.getSize(); ++i) {
        if (weights[i] == weight) {
          weights.erase(i);
          return;
        }
      }

      throw std::logic_error("weight not found");
    }

    void printWeights(WeightVector weights, std::ostream & out)
    {
      selectionSort(weights, WeightLess());

      for (size_t i = 0; i < weights.getSize(); ++i) {
        out << ' ' << weights[i];
      }
    }

    void printEdgeInfo(Vector< EdgeInfo > & info, std::ostream & out)
    {
      selectionSort(info, EdgeInfoLess());

      for (size_t i = 0; i < info.getSize(); ++i) {
        out << info[i].vertex_;
        printWeights(info[i].weights_, out);
        out << '\n';
      }
    }
  }
}
