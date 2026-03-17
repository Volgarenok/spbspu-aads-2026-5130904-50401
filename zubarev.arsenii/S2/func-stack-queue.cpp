#include "all.hpp"
#include <iostream>
namespace zubarev
{

  std::string input(std::istream& in, bool& error)
  {
    std::string str = "";
    char ch;
    while (in.peek() == ' ' || in.peek() == '\t' || in.peek() == '\n') {
      in.get();
    }
    while (in.peek() != '\n' && in.peek() != EOF) {
      ch = in.get();
      if (ch != ' ' && ch != '\t') {
        if (!isalpha(ch)) {
          str += ch;
        } else {
          error = true;
            std::cerr << "input: incorrect input" << '\n';
            return "";
        }

      }
    }
    if (in.peek() == '\n') {
      in.get();
    }

    if (str == "") {
      error = true;
    } else {
      error = false;
    }

    return str;
  }

  Queue<std::string> fromStrToQueue(std::string str)
  {
    Queue<std::string> infixQ;
    size_t num=0;
    while (num<str.size()) {
      std::string container="";
      while (std::isdigit(str[num])) {
        container+=str[num];
        num++;
      }
      infixQ.push(container);
    }

    return infixQ;
  }
  bool isdigit(const std::string& str)
  {
    if (str.empty()) {
      return false;
    }

    for (size_t i = 0; i < str.size(); ++i) {
      if (!std::isdigit(str[i])) {
        return false;
      }
    }

    return true;
  }
  Queue<char> fromInfixToPostfix(Queue<std::string> infixQ)
  {
    Stack<std::string> stack;
    Queue<std::string> postfixQ;
    bool isSign=false;
    while (!infixQ.empty()) {
      std::string el=infixQ.top();
      if (isdigit(el)) {
        postfixQ.push(el);
      } else {
        if (el!="(" && el != ")") {
          isSign=true;
        }
        stack.push(el);
      }

      infixQ.drop();
    }
    return
  }
}
