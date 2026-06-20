#include "calculator.hpp"


void malashenko::Calculator::getInfixData(std::istream& in)
{
  malashenko::detail::getInfixData(in, infixAllData_);
}

bool malashenko::Calculator::isInfixAllDataEmpty()
{
  return infixAllData_.empty();
}


void malashenko::Calculator::convertInfixToPostfix()
{
  infixData_ = infixAllData_.top();
  infixAllData_.pop();
  malashenko::detail::converInfixToPostfix(infixData_, postfixData_);
}

void  malashenko::Calculator::calculateFromPostfix()
{
  res_ = malashenko::detail::calculate(postfixData_);
}

void malashenko::Calculator::pushToOut()
{
  out_.push_back(res_);

}

void malashenko::Calculator::printOutList(std::ostream& out)
{
  LIter< lli_t > it = out_.begin();
  out << *it;
  ++it;
  for (; it != out_.end(); ++it)
  {
    out << ' ' << *it;
  }
}


