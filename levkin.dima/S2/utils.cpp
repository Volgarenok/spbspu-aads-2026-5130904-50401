#include "utils.hpp"
#include "stack.hpp"
namespace levkin {

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
    return (a >= b) ? (a - b) : 0;
  }

  long long multiply(long long a, long long b)
  {
    if ((a > 0 && b > 0 && a > MAX / b) || (a < 0 && b < 0 && a < MAX / b) ||
        (a > 0 && b < 0 && b < MIN / a) || (a < 0 && b > 0 && a < MIN / b)) {
      throw std::overflow_error("multiplying overflow");
    }
    return a * b;
  }

  long long divide(long long a, long long b)
  {
    if (a == MIN && b == -1) {
      throw std::overflow_error("division overflow");
    }
    if (b == 0)
      throw std::logic_error("Division by zero");
    return a / b;
  }

  long long reminder(long long a, long long b)
  {
    if (a == MIN && b == -1) {
      throw std::overflow_error("division overflow");
    }
    if (b == 0)
      throw std::runtime_error("Division by zero");
    return a % b;
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

  long long exponent(long long base, long long exp)
  {
    if (exp < 0) {
      throw std::runtime_error("Negative exponent not supported for integers");
    }
    if (base == 0 && exp == 0)
      return 1;
    if (base == 0)
      return 0;

    long long result = 1;
    for (long long i = 0; i < exp; ++i) {
      result = multiply(result, base);
    }
    return result;
  }

  void applyOp(Stack< long long >& nums, Stack< Operation >& ops)
  {
    if (nums.size() < 2) {
      throw std::runtime_error("bad input\n");
    }
    long long rhs = nums.drop();
    long long lhs = nums.drop();
    Operation op = ops.drop();

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

  long long toDigit(std::string& s, size_t start, size_t end, bool& isdigit)
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

  Stack< long long > parse(std::istream& in)
  {
    Stack< long long > finalResults;

    std::string line;
    while (std::getline(in, line)) {
      if (line.empty())
        continue;
      Stack< long long > numbers;
      Stack< Operation > operators;
      Stack< char > opSymbols;

      size_t pos = 0;
      while (pos < line.size()) {
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
          char currentSymbol = line[pos];

          if (currentSymbol == '(') {
            opSymbols.push('(');
          } else if (currentSymbol == ')') {
            processOps(numbers, operators, opSymbols);
            if (opSymbols.empty())
              throw std::runtime_error("bad parenthesis\n");
            opSymbols.pop();
          } else {

            if (next_pos - pos == 2 && line[pos] == '*' &&
                line[pos + 1] == '*') {
              currentSymbol = '^';
            }
            processOps(numbers, operators, opSymbols, currentSymbol);
            opSymbols.push(currentSymbol);
            Operation op = encodeOpOrThrow(line, pos, next_pos);
            operators.push(op);
          }
        }
        pos = next_pos;
      }
      while (!operators.empty()) {
        if (opSymbols.top() == '(')
          throw std::runtime_error("Mismatched parenthesis");
        applyOp(numbers, operators);
        opSymbols.pop();
      }

      if (numbers.size() != 1)
        throw std::runtime_error("Invalid expression");
      finalResults.push(numbers.top());
    }

    return finalResults;
  }

  Operation encodeOpOrThrow(std::string& s, size_t start, size_t end)
  {
    char c;
    if (end - start == 2 && '*' == s[start] && '*' == s[start + 1]) {
      c = '^';
    } else if (end - start == 1) {
      c = s[start];
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

}
