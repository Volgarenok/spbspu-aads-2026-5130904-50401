#include <iostream>
#include <utility>
#include <limits>
#include "list.hpp"

int main()
{
  namespace  kuz = kuznetsov;
  size_t MAX = std::numeric_limits< size_t >::max();
  using stringListpair = std::pair< std::string, kuz::List< size_t > >;
  auto list = kuz::List< stringListpair >();
  std::string name;

  while (std::cin >> name) {
    auto currSequence = kuz::List< size_t >();
    size_t t = 0;
    while (std::cin >> t) {
      currSequence.insert(currSequence.end(), t);
    }
    list.insert(list.end(), std::make_pair(name, currSequence));
    if (std::cin.bad()) {
      return 1;
    }
    std::cin.clear();
  }

  if (list.empty()) {
    std::cout << "0\n";
    return 0;
  }

  auto listBegin = list.cbegin();
  std::cout << (*listBegin).first;
  listBegin++;
  for (size_t i = 1; i < list.size(); ++i) {
    std::cout << ' ' << (*listBegin).first;
    listBegin++;
  }


  std::cout << '\n';
  size_t countRows = 0;
  listBegin = list.cbegin();
  do {
    countRows = std::max(countRows, (*listBegin).second.size());
    listBegin++;
  } while (listBegin != list.cbegin());

  if (countRows == 0) {
    std::cout << "0\n";
    return 0;
  }

  auto sums = kuz::List<size_t>();
  for (size_t i = 0; i < countRows; ++i) {
    sums.insert(sums.end(), 0);
  }

  auto sumIt = sums.begin();
  for (size_t row = 0; row < countRows; ++row) {
    bool first = true;
    listBegin = list.cbegin();
    do {
      auto& seq = (*listBegin).second;
      if (row < seq.size()) {
        auto it = seq.cbegin();
        for (size_t i = 0; i < row; ++i) {
          ++it;
        }
        if (!first) {
          std::cout << ' ';
        }
        std::cout << *it;
        if (MAX - *it < + *sumIt) {
          std::cout << '\n';
          std::cerr << "Overflow\n";
          return 1;
        }
        *sumIt += *it;
        first = false;
      }
      listBegin++;
    } while (listBegin != list.cbegin());
    std::cout << '\n';
    ++sumIt;
  }

  sumIt = sums.begin();
  bool first = true;
  do {
    if (!first) {
      std::cout << ' ';
    }
    std::cout << *sumIt;
    first = false;
    ++sumIt;
  } while (sumIt != sums.begin());
  std::cout << '\n';
  return 0;
}

