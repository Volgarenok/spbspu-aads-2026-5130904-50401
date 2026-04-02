#include "utils.hpp"
#include "stack.hpp"
namespace levkin {

  size_t add(size_t a, size_t b) { return a + b; }

  size_t subtract(size_t a, size_t b) { return (a >= b) ? (a - b) : 0; }

  size_t multiply(size_t a, size_t b) { return a * b; }

  size_t divide(size_t a, size_t b)
  {
    if (b == 0)
      throw std::runtime_error("Division by zero");
    return a / b;
  }

  size_t exponent(size_t a, size_t b)
  {
    size_t result = 1;
    for (size_t i = 0; i < b; ++i) {
      result *= a;
    }
    return result;
  }

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

  size_t toDigit(std::string& s, size_t start, size_t end, bool& isdigit)
  {
    size_t result = 0;
    for (size_t i = 0; start + i < end; ++i) {
      char c = s[start + i] - '0';
      if (c >= 0 && c <= 9) {
        result = result * 10 + c;
      } else {
        isdigit = false;
        return 0;
      }
    }
    isdigit = true;
    return result;
  }

  size_t getNextWord(const std::string& s, size_t start)
  {
    size_t shift = 0;
    while ((start + shift) < s.length() && s[start + shift] != ' ') {
      ++shift;
    }
    return shift + start;
  }

  void parse(std::istream& in)
  {
    Stack< int > results;

    std::string line;
    while (std::getline(in, line)) {
      Stack< size_t > numbers;
      Stack< Operation > operators;

      // int currResult = 0;
      size_t pos = 0;
      while (pos < line.length()) {
        if (line[pos] == ' ') {
          pos++;
          continue;
        }
        size_t next_pos = getNextWord(line, pos);

        bool isDigit;
        size_t digit = toDigit(line, pos, next_pos, isDigit);

        if (isDigit) {
          numbers.push(digit);
        } else {
          Operation op = encodeOpOrThrow(line, pos, next_pos);
          operators.push(op);
        }
      }
    }
  }

  Operation encodeOpOrThrow(std::string& s, size_t start, size_t end)
  {
    char c;
    if (end - start == 2 && '*' == s[start] && '*' == s[start + 1]) {
      c = '^';
    } else if (end - start == 1) {
      c = s[start];
    } else {
      throw std::runtime_error("don't know this operation");
    }

    switch (c) {
    case '+':
      return &add;
    case '-':
      return &subtract;
    case '*':
      return &multiply;
    case '/':
      return &divide;
    case '^':
      return &exponent;
    default:
      throw std::runtime_error("don't know this operation");
    }
  }

}
