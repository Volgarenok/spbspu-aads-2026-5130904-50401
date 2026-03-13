#include <iostream>
#include <limits>
#include <string>
#include "list.hpp"

namespace zharov {
  std::istream & getData(std::istream & in,
    List< std::pair< std::string, List< int > > > & data);
  std::ostream & printNums(std::ostream & out, List< List< int> > nums);
  std::ostream & printNames(std::ostream & out, List< std::pair< std::string,
    List< int > > > data);
  List< List< int> > transposeNums(List< std::pair< std::string, List< int > > > data);
  size_t getSize(List< std::pair< std::string, List< int > > > data);
  std::ostream & printSums(std::ostream & out, List< List < int > > nums);
}

int main()
{
  zharov::List< std::pair< std::string, zharov::List< int > > > data;
  zharov::getData(std::cin, data);
  if (data.constBegin() == data.constEnd()) {
    std::cout << "0\n";
    return 0;
  }
  zharov::printNames(std::cout, data);
  if (zharov::getSize(data) == 0) {
    std::cout << "0\n";
    return 1;
  }
  auto transpose_nums = zharov::transposeNums(data);
  zharov::printNums(std::cout, transpose_nums);
  try {
    zharov::printSums(std::cout, transpose_nums);
  } catch (const std::overflow_error & e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

std::istream & zharov::getData(std::istream & in,
  List< std::pair< std::string, List< int > > > & data)
{
  std::string name;
  while (in >> name) {
    int num;
    List< int > nums;
    while (in >> num) {
      nums.pushBack(num);
    }
    data.pushBack(make_pair(name, nums));
    in.clear();
  }
  return in;
}

std::ostream & zharov::printNums(std::ostream & out, List< List< int> > nums)
{
  for (auto it = nums.constBegin(); it != nums.constEnd(); ++it) {
    for (auto inner_it = it->constBegin(); inner_it != it->constEnd(); ++inner_it) {
      out << *inner_it << ' ';
    }
    out << "\n";
  }
  return out;
}

std::ostream & zharov::printNames(std::ostream & out, List< std::pair< std::string,
  List< int > > > data)
{
  for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
    out << it->first << " ";
  }
  out << "\n";
  return out;
}

size_t zharov::getSize(List< std::pair< std::string, List< int > > > data)
{
  size_t res = 0;
  for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
    res = std::max(res, it->second.size());
  }
  return res;
}

zharov::List< zharov::List< int> > zharov::transposeNums(
  List< std::pair< std::string, List< int > > > data)
{
  List< List < int > > res;
  for (size_t i = 0; i < getSize(data); ++i) {
    List< int > inner;
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

std::ostream & zharov::printSums(std::ostream & out, List< List < int > > nums)
{
  constexpr int MAX = std::numeric_limits< int >::max();
  for (auto it = nums.constBegin(); it != nums.constEnd(); ++it) {
    int sum = 0;
    for (auto inner_it = it->constBegin(); inner_it != it->constEnd(); ++inner_it) {
      if (MAX - *inner_it < sum) {
        throw std::overflow_error("Overflow");
      }
      sum += *inner_it;
    }
    out << sum << ' ';
  }
  out << "\n";
  return out;
}
