#include <iostream>
#include <limits>
#include <string>
#include <list.hpp>

namespace
{
  using NumList = zharov::List< size_t >;
  using NumMatrix = zharov::List< NumList >;
  using DataEntry = std::pair< std::string, NumList >;
  using DataList = zharov::List< DataEntry >;

  std::istream& getData(std::istream& in, DataList& data);
  std::ostream& printNums(std::ostream& out, NumMatrix nums);
  std::ostream& printNames(std::ostream& out, DataList data);
  NumMatrix transposeNums(DataList data);
  size_t getSize(DataList data);
  NumList getSums(NumMatrix nums);
  std::ostream& printSums(std::ostream& out, NumList sums);
}

int main()
{
  DataList data;
  getData(std::cin, data);
  if (data.cbegin() == data.cend())
  {
    std::cout << "0\n";
    return 0;
  }
  printNames(std::cout, data);
  std::cout << "\n";
  if (getSize(data) == 0)
  {
    std::cout << "0\n";
    return 0;
  }
  NumMatrix transpose_nums = transposeNums(data);
  printNums(std::cout, transpose_nums);
  std::cout << "\n";
  try
  {
    NumList sums_list = getSums(transpose_nums);
    printSums(std::cout, sums_list);
    std::cout << "\n";
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
}

namespace
{
  std::istream& getData(std::istream& in, DataList& data)
  {
    std::string name;
    while (in >> name)
    {
      size_t num;
      NumList nums;
      while (in >> num)
      {
        nums.pushBack(num);
      }
      data.pushBack(make_pair(name, nums));
      in.clear();
    }
    return in;
  }

  std::ostream& printNums(std::ostream& out, NumMatrix nums)
  {
    auto it = nums.cbegin();
    auto inner_it = it->cbegin();
    out << *inner_it;
    ++inner_it;
    for (; inner_it != it->cend(); ++inner_it)
    {
      out << ' ';
      out << *inner_it;
    }
    ++it;
    for (; it != nums.cend(); ++it)
    {
      out << "\n";
      auto inner_it = it->cbegin();
      out << *inner_it;
      ++inner_it;
      for (; inner_it != it->cend(); ++inner_it)
      {
        out << ' ';
        out << *inner_it;
      }
    }
    return out;
  }

  std::ostream& printNames(std::ostream& out, DataList data)
  {
    auto it = data.cbegin();
    out << it->first;
    ++it;
    for (; it != data.cend(); ++it)
    {
      std::cout << ' ';
      out << it->first;
    }
    return out;
  }

  size_t getSize(DataList data)
  {
    size_t res = 0;
    for (auto it = data.cbegin(); it != data.cend(); ++it)
    {
      res = std::max(res, it->second.size());
    }
    return res;
  }

  NumMatrix transposeNums(DataList data)
  {
    NumMatrix res;
    for (size_t i = 0; i < getSize(data); ++i)
    {
      NumList inner;
      for (auto it = data.cbegin(); it != data.cend(); ++it)
      {
        auto num_it = it->second.cbegin();
        if (it->second.size() > i)
        {
          for (size_t j = 0; j < i; ++j)
          {
            ++num_it;
          }
          inner.pushBack(*num_it);
        }
      }
      res.pushBack(inner);
    }
    return res;
  }

  NumList getSums(NumMatrix nums)
  {
    NumList res;
    constexpr size_t MAX = std::numeric_limits< size_t >::max();
    for (auto it = nums.cbegin(); it != nums.cend(); ++it)
    {
      size_t sum = 0;
      for (auto inner_it = it->cbegin(); inner_it != it->cend(); ++inner_it)
      {
        if (MAX - *inner_it < sum)
        {
          throw std::overflow_error("Overflow");
        }
        sum += *inner_it;
      }
      res.pushBack(sum);
    }
    return res;
  }

  std::ostream& printSums(std::ostream& out, NumList sums)
  {
    auto it = sums.cbegin();
    out << *it;
    ++it;
    for (; it != sums.cend(); ++it)
    {
      out << ' ';
      out << *it;
    }
    return out;
  }
}
