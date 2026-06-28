#include "utils.hpp"
#include <stdexcept>
#include <limits>
#include <cmath>
#include "stack.hpp"

namespace levkin {
  const long long MAX = std::numeric_limits< long long >::max();
  const long long MIN = std::numeric_limits< long long >::min();

  long long add(long long a, long long b)
  {
    if ((b > 0 && a > MAX - b) || (b < 0 && a < MIN - b)) {
      throw std::overflow_error("addition overflow");
    }
    return a + b;
  }

  long long subtract(long long a, long long b)
  {
    if ((b > 0 && a < MIN + b) || (b < 0 && a > MAX + b)) {
      throw std::overflow_error("subtraction overflow");
    }
    return a - b;
  }

  long long multiply(long long a, long long b)
  {
    if (a == 0 || b == 0) {
      return 0;
    }
    if (a > 0 && b > 0 && a > MAX / b) {
      throw std::overflow_error("overflow");
    }
    if (a < 0 && b < 0 && a < MAX / b) {
      throw std::overflow_error("overflow");
    }
    if (a > 0 && b < 0 && b < MIN / a) {
      throw std::overflow_error("underflow");
    }
    if (a < 0 && b > 0 && a < MIN / b) {
      throw std::overflow_error("underflow");
    }
    return a * b;
  }

  long long divide(long long a, long long b)
  {
    if (a == MIN && b == -1) {
      throw std::overflow_error("division overflow");
    }
    if (b == 0) {
      throw std::logic_error("Division by zero");
    }
    return a / b;
  }

  long long reminder(long long a, long long b)
  {
    if (b == 0) {
      throw std::logic_error("division by zero");
    }
    long long res = a % b;
    if (res < 0) {
      res += std::abs(b);
    }
    return res;
  }

  long long exponent(long long base, long long exp)
  {
    if (exp < 0) {
      throw std::runtime_error("Negative exponent not supported for integers");
    }
    if (base == 0 && exp == 0) {
      return 1;
    }
    if (base == 0) {
      return 0;
    }
    long long result = 1;
    for (long long i = 0; i < exp; ++i) {
      result = multiply(result, base);
    }
    return result;
  }

  void processOps(
      Stack< long long >& nums,
      Stack< Operation >& ops,
      Stack< char >& symbols,
      char currentOp)
  {
    while (!symbols.empty() && symbols.top() != '(') {
      if (currentOp != '\0' && priority(symbols.top()) < priority(currentOp)) {
        break;
      }
      applyOp(nums, ops);
      symbols.pop();
    }
  }

  void applyOp(Stack< long long >& nums, Stack< Operation >& ops)
  {
    if (nums.size() < 2) {
      throw std::runtime_error("bad input\n");
    }
    long long rhs = nums.top();
    nums.pop();
    long long lhs = nums.top();
    nums.pop();
    Operation op = ops.top();
    ops.pop();

    nums.push(op(lhs, rhs));
  }

  unsigned short priority(char i)
  {
    unsigned short weight = 0;
    if (i == '+' || i == '-') {
      weight = 1;
    } else if (i == '%' || i == '*' || i == '/') {
      weight = 2;
    } else if (i == '^') {
      weight = 3;
    } else {
      throw std::runtime_error("wierd input (caught bad operation)\n");
    }
    return weight;
  }

  size_t getNextWord(const std::string& s, size_t start)
  {
    size_t shift = 0;
    while ((start + shift) < s.length() && s[start + shift] != ' ') {
      ++shift;
    }
    return shift + start;
  }

  Operation encodeOpOrThrow(const std::string& token)
  {
    char c = '\0';
    if (token == "**") {
      c = '^';
    } else if (token.size() == 1) {
      c = token[0];
    } else {
      throw std::runtime_error("don't know this operation\n");
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
    case '%':
      return &reminder;
    case '^':
      return &exponent;
    default:
      throw std::runtime_error("don't know this operation");
    }
  }

  Stack< long long > parse(std::istream& in)
  {
    Stack< long long > finalResults;
    std::string line;

    while (std::getline(in, line)) {
      if (line.empty()) {
        continue;
      }
      Stack< long long > numbers;
      Stack< Operation > operators;
      Stack< char > opSymbols;

      size_t pos = 0;
      while (pos < line.size()) {
        if (line[pos] == ' ') {
          pos++;
          continue;
        }
        size_t nextPos = getNextWord(line, pos);
        std::string token = line.substr(pos, nextPos - pos);

        try {
          size_t processedChars = 0;
          long long digit = std::stoll(token, &processedChars);
          if (processedChars == token.size()) {
            numbers.push(digit);
          } else {
            throw std::invalid_argument("not completely a number");
          }
        } catch (const std::invalid_argument&) {
          if (token == "(") {
            opSymbols.push('(');
          } else if (token == ")") {
            processOps(numbers, operators, opSymbols);
            if (opSymbols.empty()) {
              throw std::runtime_error("bad parenthesis\n");
            }
            opSymbols.pop();
          } else {
            char currentSymbol = (token == "**") ? '^' : token[0];
            processOps(numbers, operators, opSymbols, currentSymbol);
            opSymbols.push(currentSymbol);
            Operation op = encodeOpOrThrow(token);
            operators.push(op);
          }
        } catch (const std::out_of_range&) {
          throw std::overflow_error("number container overflow");
        }
        pos = nextPos;
      }

      while (!operators.empty()) {
        if (opSymbols.top() == '(') {
          throw std::runtime_error("Mismatched parenthesis");
        }
        applyOp(numbers, operators);
        opSymbols.pop();
      }

      if (numbers.size() != 1) {
        throw std::runtime_error("Invalid expression");
      }
      finalResults.push(numbers.top());
    }
    return finalResults;
  }
}