#ifndef DATASET_INPUT_HPP
#define DATASET_INPUT_HPP

#include "dataset.hpp"
#include <fstream>
#include <iostream>
#include <limits>
#include <string>

namespace zubarev
{
  void inputDataset(std::istream& in, DatasetTable& datasets);
}

#endif
