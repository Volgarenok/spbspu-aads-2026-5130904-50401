#include "utils.hpp"

namespace levkin {
  unsigned short priority(char i)
  {
    unsigned short weight = 0;
    if (i == '+' || i == '-') {
      weight = 1;
    } else if (i == '%' || i == '*' || i == '/') {
      weight = 2;
    }
    return weight;
  }
  
  size_t toDigit(std::string& s, size_t start, size_t len, bool& wasnotdigit)
  {
    size_t result = 0;
    for (size_t i = 0; i < len; ++i) {
      char c = s[start + i] - '0';
      if (c >= 0 && c <= 9) {
        result = result * 10 + c;
      } else {
        wasnotdigit = true;
        return 0;
      }
    }
    wasnotdigit = false;
    return result;
  }
}
