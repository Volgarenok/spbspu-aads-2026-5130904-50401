#ifndef CALCULATOR_HPP
#define CALCULATOR_HPP
#include "math_functions.hpp"

namespace malashenko
{
  class Calculator {
  public:
    void getInfixData(std::istream&);
    bool isInfixAllDataEmpty();
    void convertInfixToPostfix();
    void calculateFromPostfix();
    void pushToOut();
    void printOutList(std::ostream&);

  private:
    Stack< Queue< std::string > > infixAllData_;
    Queue< std::string > infixData_;
    Queue< std::string > postfixData_;
    List< std::string > out_;
    std::string res_;
  };
}

#endif
