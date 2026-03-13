#include <iostream>
#include <limits>
#include <string>
#include "list.hpp"

namespace zharov {
  std::istream & getData(std::istream & in,
    List< std::pair< std::string, List< size_t > > > & data);
  std::ostream & printNums(std::ostream & out, List< List< size_t> > nums);
  std::ostream & printNames(std::ostream & out, List< std::pair< std::string,
    List< size_t > > > data);
  List< List< size_t> > transposeNums(List< std::pair< std::string, List< size_t > > > data);
  size_t getSize(List< std::pair< std::string, List< size_t > > > data);
  List< size_t> getSums(List< List < size_t > > nums);
  std::ostream & printSums(std::ostream & out, List< size_t > sums);
}

int main()
{
  zharov::List< std::pair< std::string, zharov::List< size_t > > > data;
  zharov::getData(std::cin, data);
  if (data.constBegin() == data.constEnd()) {
    std::cout << "0\n";
    return 0;
  }
  zharov::printNames(std::cout, data);
  if (zharov::getSize(data) == 0) {
    std::cout << "0\n";
    return 0;
  }
  auto transpose_nums = zharov::transposeNums(data);
  zharov::printNums(std::cout, transpose_nums);
  try {
    auto sums_list = zharov::getSums(transpose_nums);
    zharov::printSums(std::cout, sums_list);
  } catch (const std::overflow_error & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

std::istream & zharov::getData(std::istream & in,
  List< std::pair< std::string, List< size_t > > > & data)
{
  std::string name;
  while (in >> name) {
    size_t num;
    List< size_t > nums;
    while (in >> num) {
      nums.pushBack(num);
    }
    data.pushBack(make_pair(name, nums));
    in.clear();
  }
  return in;
}

std::ostream & zharov::printNums(std::ostream & out, List< List< size_t > > nums)
{
  bool first = true;
  for (auto it = nums.constBegin(); it != nums.constEnd(); ++it) {
    for (auto inner_it = it->constBegin(); inner_it != it->constEnd(); ++inner_it) {
      if (!first) {
        std::cout << ' ';
      }
      out << *inner_it;
      first = false;
    }
    first = true;
    out << "\n";
  }
  return out;
}

std::ostream & zharov::printNames(std::ostream & out, List< std::pair< std::string,
  List< size_t > > > data)
{
  bool first = true;
  for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
    if (!first) {
      std::cout << ' ';
    }
    out << it->first;
    first = false;
  }
  out << "\n";
  return out;
}

size_t zharov::getSize(List< std::pair< std::string, List< size_t > > > data)
{
  size_t res = 0;
  for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
    res = std::max(res, it->second.size());
  }
  return res;
}

zharov::List< zharov::List< size_t> > zharov::transposeNums(
  List< std::pair< std::string, List< size_t > > > data)
{
  List< List < size_t > > res;
  for (size_t i = 0; i < getSize(data); ++i) {
    List< size_t > inner;
    for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
      auto num_it = it->second.constBegin();
      if (it->second.size() > i) {
        for (size_t j = 0; j < i; ++j) {
          ++num_it;
        }
        inner.pushBack(*num_it);
      }
    }
    res.pushBack(inner);
  }
  return res;
}

zharov::List< size_t> zharov::getSums(List< List < size_t > > nums)
{
  List< size_t > res;
  constexpr size_t MAX = std::numeric_limits< size_t >::max();
  for (auto it = nums.constBegin(); it != nums.constEnd(); ++it) {
    size_t sum = 0;
    for (auto inner_it = it->constBegin(); inner_it != it->constEnd(); ++inner_it) {
      if (MAX - *inner_it < sum) {
        throw std::overflow_error("Overflow");
      }
      sum += *inner_it;
    }
    res.pushBack(sum);
  }
  return res;
}

std::ostream & zharov::printSums(std::ostream & out, List< size_t > sums)
{
  bool first = true;
  for (auto it = sums.constBegin(); it != sums.constEnd(); ++it) {
    if (!first) {
      out << ' ';
    }
    out << *it;
    first = false;
  }
  out << '\n';
  return out;
}
