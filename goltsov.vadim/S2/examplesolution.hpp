#ifndef EXAMPLESOLUTION_HPP
#define EXAMPLESOLUTION_HPP
#include <string>
#include "myqueue.hpp"

namespace goltsov
{
  namespace detail
  {
    long long int priority(std::string a);
  }
  goltsov::Queue< std::string > converToPostfix(goltsov::Queue< std::string > a);
  long long int eval(goltsov::Queue< std::string > postfix);
}

#endif
