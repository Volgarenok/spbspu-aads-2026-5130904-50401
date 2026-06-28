#include "input.hpp"

#include <string>

namespace zubarev
{
  void inputDataset(std::istream& in, DatasetTable& datasets)
  {
    while (true) {
      std::string datasetName;
      if (!(in >> datasetName)) {
        break;
      }

      BSTree< size_t, std::string, Comparator< size_t > > dataset;
      while (true) {
        size_t key = 0;
        std::string value;
        if (in.peek() == '\n' || in.peek() == EOF) {
          break;
        }
        if (!(in >> key)) {
          break;
        }
        if (in.peek() == '\n' || in.peek() == EOF) {
          break;
        }
        if (!(in >> value)) {
          break;
        }

        dataset.push(key, value);

        if (in.peek() == '\n' || in.peek() == EOF) {
          break;
        }
      }
      datasets.insertDataset(datasetName, dataset);
    }
  }
}
