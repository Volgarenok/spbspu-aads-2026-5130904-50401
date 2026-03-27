#include "stack.hpp"
#include "queue.hpp"
#include <fstream>


int getPriority(char op) {
  if (op == '+' || op == '-') return 1;
  if (op == '*' || op == '/' || op == '%' || op == 'g') return 2;
  return 0;
}

void handleOperator(char s, madieva::Stack< std::string > & op, madieva::Queue< std::string > & post) {
  int prior = getPriority(s);

  while (!op.empty() && op.top() != "(" && getPriority(op.top()[0]) >= prior) {
      post.push(op.top());
      op.pop();
  }

  op.push(std::string(1, s));
}
void postfix(std::string line, madieva::Queue< std::string > & post)
{
  madieva::Stack< std::string > op;
  std::string number;
  std::string op_gcd;
  char s;
  for (size_t i = 0; i < line.length(); ++i) {
    s = line[i];
    if (s != ' ') {
      if (s == '(') {
        op.push(std::string(1, s));
      } else if (s == '*' || s == '/' || s == '%' || s == '+' || s == '-') {
        handleOperator(s, op, post);
      } else if (s == ')') {
        if (op.empty()) {
          throw std::runtime_error("Mismatched parentheses");
        }
        std::string temp = op.top();
        while (temp != "(") {
          post.push(temp);
          op.pop();
          if (op.empty()) {
            throw std::runtime_error("Mismatched parentheses");
          }
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
          throw std::runtime_error("Unknown token");
        } else {
          char gcd = 'g';
          handleOperator(gcd, op, post);
          op_gcd.clear();
        }
      }
    }
  }
  while (!op.empty()) {
    post.push(op.top());
    op.pop();
  }
}

main(int argc, char * argv[])
{
  namespace mad = madieva;
  std::ifstream file;
  std::istream * in = & std::cin;
  if (argc == 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "file not opeen\n";
      return 1;
    }
    in = & file;
  } else if (argc > 2) {
    std::cerr  << "not right size arguments\n";
  }
  mad::Stack< int > res;
  std::string line;
  while(std::getline(*in, line)) {
    bool empty = true;
    for (size_t i = 0; empty && i < line.size(); ++i) {
      if (line[i] != ' ' && line[i] != '\n') {
        empty = false;
      }
    }
    if (!empty) {
      mad::Queue< std::string > post;
      postfix(line, post);
      mad::Stack< int > res;
      // math (result)
    }
  }

}
