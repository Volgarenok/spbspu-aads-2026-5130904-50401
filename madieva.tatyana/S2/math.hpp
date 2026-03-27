#include "stack.hpp"
#include "queue.hpp"
#include <fstream>

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

bool priority(char & s, madieva::Stack< std::string > & op)  // s priority op
{

}

void postfix(std::string line, madieva::Queue< std::string > & post)
{
  madieva::Stack< std::string > op;
  std::string number;
  std::string op_gcd;
  char s;
  for (size_t i = 0; i < line.length(); ++i) {
    s = line[i];
    try{
      if (s != ' ') {
        if (s == '(') {
          op.push(std::string(1, s));
        } else if (s == '*' || s == '/' || s == '%' || s == '+' || s == '-') {
          if (priority(s, op)) {
            op.push(std::string(1, s));
          } else {
            post.push(op.top());
            op.pop();
            op.push(std::string(1, s));
          }
        } else if (s == ')') {
          std::string temp = op.top();
          while (temp != "(") {
            post.push(temp);
            op.pop();
            temp = op.top();
          }
          op.pop();
        } else {
          if (std::isdigit(s)) {
            number += s;
          } else {
            op_gcd += s;
          }
        }
      } else {
        if (number.length()) {
          post.push(number);
          number.clear();
        } else if (op_gcd.length()) {
          if (op_gcd != "gcd") {
            throw std:://
          } else {
            char gcd = 'g';
            if (priority(gcd, op)) {

            }
          }
        }
      }
    } catch (const std::runtime_error & e) {
    }
  }
}
