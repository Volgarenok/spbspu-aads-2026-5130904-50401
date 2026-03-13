#include <iostream>

#include "list.hpp"
#include "liter.hpp"
#include "lciter.hpp"

int main()
{
  using namespace chernov;
  using pStrList = std::pair< std::string, List< size_t > >;
  List< pStrList > lines;
  LIter< pStrList > lines_iter = lines.before_begin();

  std::string name;
  while (std::cin >> name) {
    List< size_t > sequence;
    LIter< size_t > seq_iter = sequence.before_begin();

    size_t num = 0;
    while (std::cin >> num) {
      seq_iter = sequence.insert_after(seq_iter, num);
    }

    lines_iter = lines.insert_after(lines_iter, {name, sequence});
    if (std::cin.bad()) {
      std::cerr << "bad input\n";
      return 1;
    }
    std::cin.clear();
  }

  if (!lines.empty()) {
    lines_iter = lines.begin();
    do {
      pStrList line = *lines_iter;
      std::cout << line.first << "\n";
      List< size_t > sequence = line.second;
      if (!sequence.empty()) {
        LIter< size_t > seq_iter = sequence.begin();
        do {
          std::cout << *seq_iter << " ";
        } while (++seq_iter != sequence.begin());
        std::cout << "\n";
      }
    } while (++lines_iter != lines.begin());
  }
}
