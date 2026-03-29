#include "arifmetic_operations.hpp"

long long int hvostov::addWithOverflowCheck(long long int left, long long int right)
{
  if ((right > 0 && left > LLMAX - right) ||
      (right < 0 && left < LLMIN - right)) {
    throw std::overflow_error("Addition overflow!");
  }
  return left + right;
}

long long int hvostov::subtractWithOverflowCheck(long long int left, long long int right)
{
  if ((right < 0 && left > LLMAX + right) ||
      (right > 0 && left < LLMIN + right)) {
    throw std::overflow_error("Subtraction overflow!");
  }
  return left - right;
}

long long int hvostov::multiplyWithOverflowCheck(long long int left, long long int right)
{
  if (left != 0 && right != 0) {
    if ((left > 0 && right > 0 && left > LLMAX / right) ||
        (left > 0 && right < 0 && right < LLMIN / left) ||
        (left < 0 && right > 0 && left < LLMIN / right) ||
        (left < 0 && right < 0 && right < LLMAX / left)) {
      throw std::overflow_error("Multiplication overflow!");
    }
  }
  return left * right;
}

long long int hvostov::divideWithOverflowCheck(long long int left, long long int right)
{
  if (right == 0) {
    throw std::logic_error("Division by zero!");
  }
  if (left == LLMIN && right == -1) {
    throw std::overflow_error("Division overflow!");
  }
  return left / right;
}

long long int hvostov::moduloWithOverflowCheck(long long int left, long long int right)
{
  if (right == 0) {
    throw std::logic_error("Modulo by zero!");
  }
  if (left == LLMIN && right == -1) {
    throw std::overflow_error("Modulo overflow!");
  }
  long long result = left % right;
  if (result < 0) {
    result += right;
  }
  return result;
}

long long int hvostov::shiftLeftWithOverflowCheck(long long int left, long long int right)
{
  if (right < 0) {
    throw std::logic_error("Negative shift amount!");
  }
  int bitWidth = sizeof(long long int) * CHAR_BIT;
  if (right >= bitWidth) {
    throw std::overflow_error("Shift overflow: shift amount exceeds bit width!");
  }
  if (left == 0) {
    return 0;
  }
  if (left > 0) {
    if (left > (LLMAX >> right)) {
      throw std::overflow_error("Left shift overflow!");
    }
  } else if (left < 0) {
    if (left < (LLMIN >> right)) {
      throw std::overflow_error("Left shift overflow!");
    }
  }
  return left << right;
}
