#include "functions.hpp"

void lukashevich::getData(std::istream& in, List< pair_t >& res)
{
  std::string name;

  while (in >> name) {
    List< size_t > nums;
    size_t num = 0;

    while (in >> num) {
      nums.pushBack(num);
    }

    pair_t p(name, nums);
    res.pushBack(p);

    if (in.fail() && !in.eof()) {
      in.clear();
    }
  }
}

void lukashevich::printSeqName(std::ostream& out, const List< pair_t >& data)
{

  LCIter< pair_t > it = data.begin();
  LCIter< pair_t > end = data.end();

  if (it != end) {
  out << it->first;
  ++it;
  }

  while (it != end) {
    out << ' ' << it->first;
    ++it;
  }
}

size_t lukashevich::printOneRow(std::ostream& out, List< pair_t >& data)
{
  LIter< pair_t > it = data.begin();
  LIter< pair_t > end = data.end();

  bool first = true;
  bool overflow = false;
  size_t sum = 0;

  while (it != end)
  {
    if (!it->second.empty()) {
      size_t value = it->second.front();

      if (!first) {
        out << ' ';
      }

      out << value;

      if (!overflow) {
        if (sum > std::numeric_limits< size_t >::max() - value) {
          overflow = true;
        } else {
          sum += value;
        }
      }

      it->second.popFront();
      first = false;
    }

    ++it;
  }

  if (overflow) {
    throw std::overflow_error("overflow");
  }

  return sum;
}

bool lukashevich::hasNum(const List< pair_t >& data)
{
  LCIter< pair_t > it = data.begin();
  LCIter< pair_t > end = data.end();

  while (it != end) {
    if (!(it->second.empty())) {
      return true;
    }
    ++it;
  }

  return false;
}

void lukashevich::printAll(List< pair_t >& data, List< size_t >& sums)
{
  while (hasNum(data)) {
    size_t sum = printOneRow(std::cout, data);
    std::cout << '\n';
    sums.pushBack(sum);
  }
}

void lukashevich::printSum(std::ostream& out, const List< size_t >& sums)
{

  if (sums.empty()) {
    out << 0;
    return;
  }

  LCIter< size_t > it = sums.begin();
  LCIter< size_t > end = sums.end();

  out << *it;
  ++it;

  while (it != end) {
    out << ' ' << *it;
    ++it;
  }
}
