#ifndef UTILS_S2
#define UTILS_S2

#include <cstddef>
#include <iostream>
#include <string>
namespace levkin {

  // size_t getNextWord(std::string s, size_t start);
  unsigned short priority(char i);
  size_t toDigit(std::string& s, size_t start, size_t len, bool& wasnotdigit);

  // void parse(std::istream& in)
  // {
  //   std::string line;
  //   while (std::getline(in, line)) {
  //     size_t pos = 0;

  //     size_t next_pos = getNextWord(line, pos);
  //   }
  // }
  // size_t getNextWord(std::string s, size_t start)
  // {
  //   size_t shift = 0;
  //   for (; s[start + shift] != ' '; ++shift) {
  //   }
  //   return shift;
  // }

}
#endif
